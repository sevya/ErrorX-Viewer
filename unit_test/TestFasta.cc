#include "TestFasta.hh"
#include "gui_util.hh"

using namespace std;

TestFasta::TestFasta() {};

void TestFasta::testGUI() {
	main = new MainWindow();

	QCOMPARE( main->tabWidget->count(), 8 );
}

void TestFasta::testChecksum() {
	QString testStr = "this is just a test\nof the checksum function\tto make sure it's equivalent";
	
	QFile fileOne( "fileOne.txt" );
	fileOne.open( QIODevice::WriteOnly );
	QTextStream outOne( &fileOne );
	outOne << testStr;
	fileOne.close();

	QFile fileTwo( "fileTwo.txt" );
	fileTwo.open( QIODevice::WriteOnly );
	QTextStream outTwo( &fileTwo );
	outTwo << testStr;
	fileTwo.close();

	QCOMPARE( 
		gui_util::checksum( "fileOne.txt" ), 
		gui_util::checksum( "fileTwo.txt" ) 
		);

	fileOne.remove();
	fileTwo.remove();
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
	main->options_->species( "human" );
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

void TestFasta::checkSummaryTab() {

	QCOMPARE( main->summaryTab_->line2->text(), "100" );
	QCOMPARE( main->summaryTab_->line3->text(), "79" );
	QCOMPARE( main->summaryTab_->line4->text(), "37" );
	QCOMPARE( main->summaryTab_->line5->text(), "79" );

	auto it = main->summaryTab_->plotBars->data()->begin();
	QCOMPARE( it->mainKey(), 1 );
	QCOMPARE( it->mainValue(), 100 );

	it++;
	QCOMPARE( it->mainKey(), 2 );
	QCOMPARE( it->mainValue(), 79 );

	it++;
	QCOMPARE( it->mainKey(), 3 );
	QCOMPARE( it->mainValue(), 37 );

	it++;
	QCOMPARE( it->mainKey(), 4 );
	QCOMPARE( it->mainValue(), 79 );
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
	QCOMPARE( gui_util::checksum( "data_table_export.tsv" ), 
			  gui_util::checksum( "correct_table_long_fasta.tsv" ));
	QFile file( "data_table_export.tsv" );
	file.remove();
	
	// Export first with full_data selected
	main->dataTab_->checkBox->setChecked( false );
	main->dataTab_->exportTable( "data_table_export.tsv" );
	QCOMPARE( gui_util::checksum( "data_table_export.tsv" ),
			  gui_util::checksum( "correct_table_short_fasta.tsv" ));
	file.remove();
}

void TestFasta::checkTableCopy() {	
	// Copy first with full_data selected
	main->dataTab_->checkBox->setChecked( true );
	main->dataTab_->copyAll();

	gui_util::clipboardToFile( "data_table_copy.tsv" );

	QCOMPARE( gui_util::checksum( "data_table_copy.tsv" ), 
			  gui_util::checksum( "correct_table_long_fasta.tsv" ));
	QFile file( "data_table_copy.tsv" );
	file.remove();
	
	// Copy now with full_data not selected
	main->dataTab_->checkBox->setChecked( false );
	main->dataTab_->copyAll();

	gui_util::clipboardToFile( "data_table_copy.tsv" );

	QCOMPARE( gui_util::checksum( "data_table_copy.tsv" ), 
			  gui_util::checksum( "correct_table_short_fasta.tsv" ));
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
	QCOMPARE( gui_util::checksum( "clonotype_table_export.tsv" ),
			  gui_util::checksum( "correct_clonotype_table.tsv" ));
	QFile file( "clonotype_table_export.tsv" );
	file.remove();
}

void TestFasta::checkClonotypeTableCopy() {
	// Copy clonotype table to clipboard
	main->clonotypeTab_->copyAll();
	gui_util::clipboardToFile( "clonotype_table_copy.tsv" );

	QCOMPARE( gui_util::checksum( "clonotype_table_copy.tsv" ),
			  gui_util::checksum( "correct_clonotype_table.tsv" ));

	QFile file( "clonotype_table_copy.tsv" );
	file.remove();
}

void TestFasta::checkGeneTab() {

	using namespace std;

	// Set up correct values for V gene histogram
	map<string,int> valuesV = { 
		make_pair("IGHV3-66", 22),
		make_pair("IGHV3-23", 12),
		make_pair("IGHV3-53", 6),
		make_pair("IGHV3-NL1", 6),
		make_pair("IGHV3-11", 4),
		make_pair("IGHV3-48", 4),
		make_pair("IGHV3-7", 4),
		make_pair("IGHV3-30", 4),
		make_pair("IGHV3-15", 3),
		make_pair("IGHV3-69-1", 3)
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
		make_pair("N/A", 62),
		make_pair("IGHJ4", 11),
		make_pair("IGHJ1", 6)
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
			confirm->setFile( "../100.fasta" );

			QCOMPARE( confirm->fileEdit->text(), "../100.fasta" );
			QVERIFY( confirm->radioButtonFASTA->isChecked() );
			QVERIFY( !confirm->radioButtonFASTQ->isChecked() );
			QVERIFY( !confirm->radioButtonTSV->isChecked() );

			QTest::keyClick( confirm, Qt::Key_Enter );
		}
	}
}

// QTEST_MAIN( TestFasta )
// #include "TestFasta.moc"

