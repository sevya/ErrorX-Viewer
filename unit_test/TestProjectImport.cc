#include "TestProjectImport.hh"

#include "gui_util.hh"

#include "SequenceRecords.hh"
#include "errorx.hh"

using namespace std;
using namespace errorx;

TestProjectImport::TestProjectImport() {};

void TestProjectImport::testRun() {
	main = new MainWindow();

	// Connect action handlers to check tab correctness after processing
	connect( main, &MainWindow::signalUnitTest,
			 this, &TestProjectImport::verifyTabCorrectness );
	
	// Set up options
	main->options_->nthreads( -1 );
	main->options_->error_threshold( errorx::constants::OPTIMIZED_THRESHOLD );
	main->options_->correction( 'N' );
	main->options_->allow_nonproductive( 1 );
	main->options_->species( "human" );
	main->options_->igtype( "Ig" );

	// Populate table from existing record
	main->populateFromExisting( "correct_table_long.tsv" );

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );
}

void TestProjectImport::verifyTabCorrectness() {
	executionDone = 1;

	// Check that records object is equal to what it should be
	ErrorXOptionsPtr options( new ErrorXOptions( *main->options_ ));
	options->infile( "testing/100.fastq" );
	options->format( "fastq" );
	
	// TODO implement this equality later on
	// SequenceRecordsPtr correct_records = run_protocol( *options );
	// QVERIFY( correct_records->equals( main->records_ ));

	// Make sure that tabs contain correct values
	checkSummaryTab();
	checkDataTab();
	checkClonotypeTab();
	checkErrorTab();
	checkGeneTab();

	// Unrelated to what we just imported, but make sure that the 
	// isValidErrorXTSV fxn works right
	QVERIFY( main->isValidErrorXTSV( "correct_table_long.tsv" ));
	QVERIFY( main->isValidErrorXTSV( "correct_table_long_tsv.tsv" ));

	QVERIFY( !main->isValidErrorXTSV( "correct_table_short.tsv" ));
	QVERIFY( !main->isValidErrorXTSV( "correct_table_short_tsv.tsv" ));
}

void TestProjectImport::checkSummaryTab() {

	int total_seqs      = 100;
	int parsed_seqs     = 100;
	int productive_seqs = 85;
	int unique_seqs     = 99;

	QCOMPARE( main->summaryTab_->line2->text(), QString::number(total_seqs) );
	QCOMPARE( main->summaryTab_->line3->text(), QString::number(parsed_seqs) );
	QCOMPARE( main->summaryTab_->line4->text(), QString::number(productive_seqs) );
	QCOMPARE( main->summaryTab_->line5->text(), QString::number(unique_seqs) );

	auto it = main->summaryTab_->plotBars->data()->begin();
	QCOMPARE( it->mainKey(), 1 );
	QCOMPARE( it->mainValue(), total_seqs );

	it++;
	QCOMPARE( it->mainKey(), 2 );
	QCOMPARE( it->mainValue(), parsed_seqs );

	it++;
	QCOMPARE( it->mainKey(), 3 );
	QCOMPARE( it->mainValue(), productive_seqs );

	it++;
	QCOMPARE( it->mainKey(), 4 );
	QCOMPARE( it->mainValue(), unique_seqs );
}

void TestProjectImport::checkDataTab() {
	QTableWidget* table = main->dataTab_->mainTable;

	// First, check the short table and make sure it's correct
	ifstream in( "correct_table_short.tsv" );
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
	in = ifstream( "correct_table_long.tsv" );

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

void TestProjectImport::checkClonotypeTab() {
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

void TestProjectImport::checkErrorTab() {

	QCOMPARE( main->errorTab_->beforeErrorRate->text(), "16" );
	QCOMPARE( main->errorTab_->afterErrorRate->text(),  "11" );

	auto it = main->errorTab_->plotBars->data()->begin();
	vector<int> histogramValues = { 18,32,19,11,4,5,1,3,1,3,1,1,0,0,0,0,0,0,0,0,0,1 };

	for ( size_t ii = 0; ii < histogramValues.size(); ++ii ) {
		QCOMPARE( it->mainKey(), ii );
		QCOMPARE( it->mainValue(), histogramValues[ ii ] );

		++it;
	}
}

void TestProjectImport::checkGeneTab() {

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
		make_pair( "IGHV2-6-4", 3 ),
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

// QTEST_MAIN( TestProjectImport )
// #include "TestProjectImport.moc"

