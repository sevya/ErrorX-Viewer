#include "TestTSV.hh"

#include <QtTest/QtTest>
#include <QApplication>
#include <QWidgetList>
#include <QTimer>

#include "MainWindow.hh"
#include "gui_util.hh"

using namespace std;

TestTSV::TestTSV() {};

void TestTSV::testRunFromFileMenuTSV() {

	main = new MainWindow();
	// Connect action handlers to check tab correctness after processing
	connect( main, &MainWindow::signalUnitTest,
			 this, &TestTSV::verifyTabCorrectness );
	
	// Set up options
	main->options_->nthreads( -1 );
	main->options_->error_threshold( errorx::constants::OPTIMIZED_THRESHOLD );
	main->options_->correction( 'N' );
	main->options_->allow_nonproductive( 1 );
	main->options_->species( "human" );
	main->options_->igtype( "Ig" );


	QTimer::singleShot( 1, this, &TestTSV::testTSVDialog );
	main->newProjectAction->trigger();

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );


	// Run the protocol again and make sure it's consistent
	QTimer::singleShot( 1, this, &TestTSV::testTSVDialog );
		main->newProjectAction->trigger();

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );
}

void TestTSV::verifyTabCorrectness() {
	executionDone = 1;
	
	checkSummaryTab();
	checkDataTab();
	checkTableExport();
	checkTableCopy();
	checkErrorTab();
	checkTSVTabsEliminated();
}


void TestTSV::checkSummaryTab() {

	QCOMPARE( main->summaryTab_->line2->text(), "250" );
	QCOMPARE( main->summaryTab_->line3->text(), "250" );
	QCOMPARE( main->summaryTab_->line4->text(), "N/A" );
	QCOMPARE( main->summaryTab_->line5->text(), "1" );

	auto it = main->summaryTab_->plotBars->data()->begin();
	QCOMPARE( it->mainKey(), 1 );
	QCOMPARE( it->mainValue(), 250 );

	it++;
	QCOMPARE( it->mainKey(), 2 );
	QCOMPARE( it->mainValue(), 250 );

	it++;
	QCOMPARE( it->mainKey(), 3 );
	QCOMPARE( it->mainValue(), 0 );

	it++;
	QCOMPARE( it->mainKey(), 4 );
	QCOMPARE( it->mainValue(), 1 );
}

void TestTSV::checkDataTab() {
	QTableWidget* table = main->dataTab_->mainTable;

	// First, check the short table and make sure it's correct
	ifstream in ( "correct_table_short_tsv.tsv" );

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
				QString::fromStdString( tokens[token_it++] ), 
				item->text() 
				);
		}
	}

	// Next, activate the full table and make sure it's correct
	main->dataTab_->checkBox->setChecked( true );
	table = main->dataTab_->mainTable;
	in = ifstream( "correct_table_long_tsv.tsv" );


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

void TestTSV::checkTableExport() {	
	// Export first with full_data selected
	main->dataTab_->checkBox->setChecked( true );
	main->dataTab_->exportTable( "data_table_export.tsv" );
	QCOMPARE( gui_util::checksum( "data_table_export.tsv" ), 
			  gui_util::checksum( "correct_table_long_tsv.tsv" ));
	QFile file( "data_table_export.tsv" );
	file.remove();
	
	// Export first with full_data selected
	main->dataTab_->checkBox->setChecked( false );
	main->dataTab_->exportTable( "data_table_export.tsv" );
	QCOMPARE( gui_util::checksum( "data_table_export.tsv" ),
			  gui_util::checksum( "correct_table_short_tsv.tsv" ));
	file.remove();
}

void TestTSV::checkTableCopy() {	
	// Copy first with full_data selected
	main->dataTab_->checkBox->setChecked( true );
	main->dataTab_->copyAll();

	gui_util::clipboardToFile( "data_table_copy.tsv" );

	QCOMPARE( gui_util::checksum( "data_table_copy.tsv" ), 
			  gui_util::checksum( "correct_table_long_tsv.tsv" ));
	QFile file( "data_table_copy.tsv" );
	file.remove();
	
	// Copy now with full_data not selected
	main->dataTab_->checkBox->setChecked( false );
	main->dataTab_->copyAll();

	gui_util::clipboardToFile( "data_table_copy.tsv" );

	QCOMPARE( gui_util::checksum( "data_table_copy.tsv" ), 
			  gui_util::checksum( "correct_table_short_tsv.tsv" ));
	file.remove();
}

void TestTSV::checkErrorTab() {

	QCOMPARE( main->errorTab_->beforeErrorRate->text(), "54" );
	QCOMPARE( main->errorTab_->afterErrorRate->text(), "32" );

	auto it = main->errorTab_->plotBars->data()->begin();
	QCOMPARE( it->mainKey(), 0 );
	QCOMPARE( it->mainValue(), 250 );
}
 
void TestTSV::checkTSVTabsEliminated() {
	QVERIFY( main->welcomeTab_->isEnabled() );
	QVERIFY( main->summaryTab_->isEnabled() );
	QVERIFY( main->welcomeTab_->isEnabled() );
	QVERIFY( !main->geneTab_->isEnabled() );
	QVERIFY( !main->clonotypeTab_->isEnabled() );
	QVERIFY( main->errorTab_->isEnabled() );
	QVERIFY( main->dataTab_->isEnabled() );
	QVERIFY( main->paramsTab_->isEnabled() );
}

void TestTSV::testTSVDialog() {
	foreach ( QWidget* w, QApplication::topLevelWidgets() ) {
		ConfirmFile* confirm = qobject_cast<ConfirmFile*>( w );
		if ( confirm != nullptr ) {
			confirm->setFile( "testing/ExampleInput250.tsv" );

			QCOMPARE( confirm->fileEdit->text(), "testing/ExampleInput250.tsv" );
			QVERIFY( !confirm->radioButtonFASTQ->isChecked() );
			QVERIFY( confirm->radioButtonTSV->isChecked() );

			QTest::keyClick( confirm, Qt::Key_Enter );
		}
	}
}


// QTEST_MAIN( TestTSV )
// #include "TestTSV.moc"

