#include "TestFasta.hh"
#include "gui_util.hh"

using namespace std;

TestFasta::TestFasta() {};

void TestFasta::testGUI() {
	main = new MainWindow();

	QCOMPARE( main->tabWidget->count(), 8 );
}

void TestFasta::testRunFromFileMenu() {

	main = new MainWindow();

	// Connect action handlers to check tab correctness after processing
	connect( main, &MainWindow::signalUnitTest,
			 this, &TestFasta::verifyTabCorrectness );
	
	// Set up options
	main->options_->nthreads( -1 );
	main->options_->error_threshold( errorx::constants::OPTIMIZED_THRESHOLD );
	main->options_->correction( 'N' );
	main->options_->allow_nonproductive( 1 );
	main->options_->species( "mouse" );
	main->options_->igtype( "Ig" );


	QTimer::singleShot( 1, this, &TestFasta::testFASTADialog );
	main->newProjectAction->trigger();

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );


	// Run the protocol again and make sure it's consistent
	QTimer::singleShot( 1, this, &TestFasta::testFASTADialog );
		main->newProjectAction->trigger();

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );
}


void TestFasta::verifyTabCorrectness() {
	executionDone = 1;

	checkSummaryTab();
	checkDataTab();
	checkTableExport();
	checkTableCopy();
	checkClonotypeTab();
	checkClonotypeTableExport();
	checkClonotypeTableCopy();
	QVERIFY( !main->errorTab_->isEnabled() );
	checkGeneTab();
}

void TestFasta::compareFiles( QString const & fileOne, QString const & fileTwo ) {
	ifstream in1( fileOne.toStdString() );
	ifstream in2( fileTwo.toStdString() );

	string line1;
	string line2;

	vector<string> tokens1;
	vector<string> tokens2;

	while (getline( in1, line1)) {
		getline( in2, line2 );
		tokens1 = errorx::util::tokenize_string<string>( line1, "\t" );	
		tokens2 = errorx::util::tokenize_string<string>( line2, "\t" );	

		QCOMPARE( tokens1.size(), tokens2.size() );

		for ( size_t ii = 0; ii < tokens1.size(); ++ii ) {
			QCOMPARE( tokens1[ ii ], tokens2[ ii ] );
		}
	}
}

void TestFasta::checkSummaryTab() {

	QCOMPARE( main->summaryTab_->line2->text(), "100" );
	QCOMPARE( main->summaryTab_->line3->text(), "100" );
	QCOMPARE( main->summaryTab_->line4->text(), "85" );
	QCOMPARE( main->summaryTab_->line5->text(), "99" );

	auto it = main->summaryTab_->plotBars->data()->begin();
	QCOMPARE( it->mainKey(), 1 );
	QCOMPARE( it->mainValue(), 100 );

	it++;
	QCOMPARE( it->mainKey(), 2 );
	QCOMPARE( it->mainValue(), 100 );

	it++;
	QCOMPARE( it->mainKey(), 3 );
	QCOMPARE( it->mainValue(), 85 );

	it++;
	QCOMPARE( it->mainKey(), 4 );
	QCOMPARE( it->mainValue(), 99 );
}

void TestFasta::checkDataTab() {
	QTableWidget* table = main->dataTab_->mainTable;

	// First, check the short table and make sure it's correct
	ifstream in( "correct_table_short_fasta.tsv" );
	string line;
	vector<string> tokens;
	int token_it;

	// Discard header
	getline( in, line );
	
	QTableWidgetItem* item;
	for ( int row_it = 0 ; row_it < table->rowCount(); ++row_it ) {
		getline( in, line );
		tokens = errorx::util::tokenize_string<string>( line, "\t" );
		token_it = 0;
		for ( int col_it = 0; col_it < table->columnCount(); ++col_it ) {
			item = table->item( row_it, col_it );
			// cout << tokens[ token_it ] << " : " << item->text().toStdString() << endl; // TODO remove
			QCOMPARE( 
				QString::fromStdString( tokens[token_it] ), 
				item->text() 
				);
			++token_it;
		}
	}

	// Next, activate the full table and make sure it's correct
	main->dataTab_->checkBox->setChecked( true );
	table = main->dataTab_->mainTable;
	in = ifstream( "correct_table_long_fasta.tsv" );

	// Discard header
	getline( in, line );

	for ( int row_it = 0 ; row_it < table->rowCount(); ++row_it ) {
		getline( in, line );
		tokens = errorx::util::tokenize_string<string>( line, "\t" );
		token_it = 0;
		for ( int col_it = 0; col_it < table->columnCount(); ++col_it ) {
			item = table->item( row_it, col_it );

			QCOMPARE( 
				QString::fromStdString( tokens[token_it++] ), 
				item->text() 
				);
		}
	}
}

void TestFasta::checkTableExport() {	
	// Export first with full_data selected
	main->dataTab_->checkBox->setChecked( true );
	main->dataTab_->exportTable( "data_table_export.tsv" );
	compareFiles( "data_table_export.tsv", "correct_table_long_fasta.tsv" );
	QFile file( "data_table_export.tsv" );
	file.remove();
	
	// Export first with full_data selected
	main->dataTab_->checkBox->setChecked( false );
	main->dataTab_->exportTable( "data_table_export.tsv" );
	compareFiles( "data_table_export.tsv", "correct_table_short_fasta.tsv" );

	file.remove();
}

void TestFasta::checkTableCopy() {	
	// Copy first with full_data selected
	main->dataTab_->checkBox->setChecked( true );
	main->dataTab_->copyAll();

	gui_util::clipboardToFile( "data_table_copy.tsv" );
	compareFiles( "data_table_copy.tsv", "correct_table_long_fasta.tsv" );

	QFile file( "data_table_copy.tsv" );
	file.remove();
	
	// Copy now with full_data not selected
	main->dataTab_->checkBox->setChecked( false );
	main->dataTab_->copyAll();

	gui_util::clipboardToFile( "data_table_copy.tsv" );
	compareFiles( "data_table_copy.tsv", "correct_table_short_fasta.tsv" );

	file.remove();
}

void TestFasta::checkClonotypeTab() {
	QTableWidget* table = main->clonotypeTab_->cloneTable;

	ifstream in ( "correct_clonotype_table.tsv" );
	string inputStr;
	int colCount = 3;

	// Get rid of the header, plus three extra fields
	// (to account for spaces in the header name)
	for ( int ii = 0; ii < colCount+3; ++ii ) in >> inputStr;

	QTableWidgetItem* item;
	for ( int row_it = 0 ; row_it < table->rowCount(); ++row_it ) {
		for ( int col_it = 0; col_it < table->columnCount(); ++col_it ) {
			in >> inputStr;
			item = table->item( row_it, col_it );
			QCOMPARE( QString::fromStdString(inputStr), item->text() );
		}
	}
}

void TestFasta::checkClonotypeTableExport() {
	// Export clonotype table
	main->clonotypeTab_->exportTable( "clonotype_table_export.tsv" );
	compareFiles( "clonotype_table_export.tsv", "correct_clonotype_table.tsv" );

	QFile file( "clonotype_table_export.tsv" );
	file.remove();
}

void TestFasta::checkClonotypeTableCopy() {
	// Copy clonotype table to clipboard
	main->clonotypeTab_->copyAll();
	gui_util::clipboardToFile( "clonotype_table_copy.tsv" );
	compareFiles( "clonotype_table_copy.tsv", "correct_clonotype_table.tsv" );

	QFile file( "clonotype_table_copy.tsv" );
	file.remove();
}

void TestFasta::checkGeneTab() {

	using namespace std;

	// Set up correct values for V gene histogram
	map<string,int> valuesV = { 
		make_pair( "IGHV3-2", 15 ),
        make_pair( "IGHV5-6", 9 ),
        make_pair( "IGHV1-7", 8 ),
        make_pair( "IGHV2-6-1", 7 ),
        make_pair( "IGHV1-4", 6 ),
        make_pair( "IGHV14-3", 5 ),
        make_pair( "IGHV9-3-1", 4 ),
        make_pair( "IGHV2-9", 4 ),
        make_pair( "IGHV9-4", 3 ),
        make_pair( "IGHV2-6-4", 3 )
		};

	// Get tick labels from histogram
	QMap<double, QString> ticks = main->geneTab_->vTicker->ticks();
	QCOMPARE( ticks.size(), (int)valuesV.size() );

	// Get bars data from histogram
	auto it = main->geneTab_->vBars->data()->begin();
	QCOMPARE( main->geneTab_->vBars->data()->size(), (int)valuesV.size() );
	map<string,int> histogramValues;

	// Set up map from histogram values
	for ( int ii = 0; ii < main->geneTab_->vBars->data()->size(); ++ii ) {
		histogramValues[ ticks.value( ii ).toStdString() ] = it->mainValue();
		++it;
	}


	for ( auto it = valuesV.begin(); it != valuesV.end(); ++it ) {
		// Verify that correct key is present in histogram values
		auto found = histogramValues.find( it->first );
		QVERIFY( found != histogramValues.end() );
		QCOMPARE( valuesV[ it->first ], histogramValues[ it->first ] );
	}


	// Do the same for J genes
	map<string,int> valuesJ = { 
		make_pair( "IGHJ2", 29 ),
                make_pair( "IGHJ4", 25 ),
                make_pair( "IGHJ3", 36 ),
                make_pair( "IGHJ1", 9 ),
                make_pair( "N/A", 1 )
	};

	// Get tick labels from histogram
	ticks = main->geneTab_->jTicker->ticks();
	QCOMPARE( ticks.size(), (int)valuesJ.size() );

	// Get bars data from histogram
	it = main->geneTab_->jBars->data()->begin();
	QCOMPARE( main->geneTab_->jBars->data()->size(), (int)valuesJ.size() );
	histogramValues.clear();

	// Set up map from histogram values
	for ( int ii = 0; ii < main->geneTab_->jBars->data()->size(); ++ii ) {
		histogramValues[ ticks.value( ii ).toStdString() ] = it->mainValue();
		++it;
	}


	for ( auto it = valuesJ.begin(); it != valuesJ.end(); ++it ) {
		// Verify that correct key is present in histogram values
		auto found = histogramValues.find( it->first );
		QVERIFY( found != histogramValues.end() );
		QCOMPARE( valuesJ[ it->first ], histogramValues[ it->first ] );
	}
}


void TestFasta::testFASTADialog() {
	foreach ( QWidget* w, QApplication::topLevelWidgets() ) {
		ConfirmFile* confirm = qobject_cast<ConfirmFile*>( w );
		if ( confirm != nullptr ) {
			confirm->setFile( "testing/100.fasta" );

			// Set the species to mouse
			confirm->radioButtonMouse->setChecked( true );

			QCOMPARE( confirm->fileEdit->text(), "testing/100.fasta" );
			QVERIFY( confirm->radioButtonFASTA->isChecked() );
			QVERIFY( !confirm->radioButtonFASTQ->isChecked() );
			QVERIFY( !confirm->radioButtonTSV->isChecked() );

			QTest::keyClick( confirm, Qt::Key_Enter );
		}
	}
}

// QTEST_MAIN( TestFasta )
// #include "TestFasta.moc"

