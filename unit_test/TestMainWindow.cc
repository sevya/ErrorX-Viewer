#include <QtTest/QtTest>
#include <QApplication>
#include <QWidgetList>
#include <QTimer>

#include "MainWindow.hh"
#include "gui_util.hh"

class TestMainWindow: public QObject {
	Q_OBJECT

private slots:
	void testGUI();
	void testChecksum();
	void testRunFromFileMenu();
	void testRunFromFileMenuTSV();
	void testBadRunFromFileMenu();

public slots:
	void testFASTQDialog();
	void testBadFASTQDialog();
	void testTSVDialog();
	void verifyTabCorrectness();

private:
	MainWindow* main;
	bool executionDone=0;
	bool fastq;
	bool good;
	void checkSummaryTab();
	void checkDataTab();
	void checkTableExport();
	void checkTableCopy();
	void checkClonotypeTab();
	void checkClonotypeTableExport();
	void checkClonotypeTableCopy();
	void checkErrorTab();
	void checkGeneTab();

	void checkSummaryTabTSV();
	void checkDataTabTSV();
	void checkTableExportTSV();
	void checkTableCopyTSV();
	void checkErrorTabTSV();
	void checkTSVTabsEliminated();

};

using namespace std;

void TestMainWindow::testGUI() {
	main = new MainWindow();

	QCOMPARE( main->tabWidget->count(), 7 );
}

void TestMainWindow::testChecksum() {
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

void TestMainWindow::testRunFromFileMenu() {
	fastq=1;
	good=1;
	main = new MainWindow();

	// Connect action handlers to check tab correctness after processing
	connect( main, &MainWindow::signalUnitTest,
			 this, &TestMainWindow::verifyTabCorrectness );
	
	// Set up options
	main->options_->nthreads( -1 );
	main->options_->error_threshold( errorx::constants::OPTIMIZED_THRESHOLD );
	main->options_->correction( 'N' );
	main->options_->allow_nonproductive( 1 );
	main->options_->species( "human" );
	main->options_->igtype( "Ig" );


	QTimer::singleShot( 1, this, &TestMainWindow::testFASTQDialog );
	main->newProjectAction->trigger();

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );


	// Run the protocol again and make sure it's consistent
	QTimer::singleShot( 1, this, &TestMainWindow::testFASTQDialog );
		main->newProjectAction->trigger();

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );
}

void TestMainWindow::testBadRunFromFileMenu() {
	fastq=1;
	good=0;
	main = new MainWindow();

	// Connect action handlers to check tab correctness after processing
	connect( main, &MainWindow::signalUnitTest,
			 this, &TestMainWindow::verifyTabCorrectness );
	
	// Set up options
	main->options_->nthreads( -1 );
	main->options_->error_threshold( errorx::constants::OPTIMIZED_THRESHOLD );
	main->options_->correction( 'N' );
	main->options_->allow_nonproductive( 1 );
	main->options_->species( "human" );
	main->options_->igtype( "Ig" );


	QTimer::singleShot( 1, this, &TestMainWindow::testBadFASTQDialog );
	main->newProjectAction->trigger();

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );
}

void TestMainWindow::testRunFromFileMenuTSV() {
	fastq=0;
	good=1;

	main = new MainWindow();
	// Connect action handlers to check tab correctness after processing
	connect( main, &MainWindow::signalUnitTest,
			 this, &TestMainWindow::verifyTabCorrectness );
	
	// Set up options
	main->options_->nthreads( -1 );
	main->options_->error_threshold( errorx::constants::OPTIMIZED_THRESHOLD );
	main->options_->correction( 'N' );
	main->options_->allow_nonproductive( 1 );
	main->options_->species( "human" );
	main->options_->igtype( "Ig" );


	QTimer::singleShot( 1, this, &TestMainWindow::testTSVDialog );
	main->newProjectAction->trigger();

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );


	// Run the protocol again and make sure it's consistent
	QTimer::singleShot( 1, this, &TestMainWindow::testFASTQDialog );
		main->newProjectAction->trigger();

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );
}

void TestMainWindow::verifyTabCorrectness() {
	executionDone = 1;

	cout << "got to verifyTabCorrectness with params " << fastq << " : " << good << endl; // TODO remove
	if ( !good ) {
		QCOMPARE( main->records_, nullptr );
		cout << "got here - is main->records nullptr? " << (main->records_==nullptr) << endl; // TODO remove
		return;
	}

	if ( fastq ) {
		checkSummaryTab();
		checkDataTab();
		checkTableExport();
		checkTableCopy();
		checkClonotypeTab();
		checkClonotypeTableExport();
		checkClonotypeTableCopy();
		checkErrorTab();
		checkGeneTab();
	} else {
		checkSummaryTabTSV();
		checkDataTabTSV();
		checkTableExportTSV();
		checkTableCopyTSV();
		checkErrorTabTSV();
		checkTSVTabsEliminated();
	}
}

void TestMainWindow::checkSummaryTab() {

	QCOMPARE( main->summaryTab_->line2->text(), "100" );
	QCOMPARE( main->summaryTab_->line3->text(), "79" );
	QCOMPARE( main->summaryTab_->line4->text(), "48" );
	QCOMPARE( main->summaryTab_->line5->text(), "79" );

	auto it = main->summaryTab_->plotBars->data()->begin();
	QCOMPARE( it->mainKey(), 1 );
	QCOMPARE( it->mainValue(), 100 );

	it++;
	QCOMPARE( it->mainKey(), 2 );
	QCOMPARE( it->mainValue(), 79 );

	it++;
	QCOMPARE( it->mainKey(), 3 );
	QCOMPARE( it->mainValue(), 48 );

	it++;
	QCOMPARE( it->mainKey(), 4 );
	QCOMPARE( it->mainValue(), 79 );
}

void TestMainWindow::checkDataTab() {
	QTableWidget* table = main->dataTab_->mainTable;

	// First, check the short table and make sure it's correct
	ifstream in ( "correct_table_short.tsv" );
	string inputStr;
	int colCount = 7;

	// Get rid of the header
	for ( int ii = 0; ii < colCount; ++ii ) in >> inputStr;
	
	QTableWidgetItem* item;
	for ( int row_it = 0 ; row_it < table->rowCount(); ++row_it ) {
		for ( int col_it = 0; col_it < table->columnCount(); ++col_it ) {
			in >> inputStr;
			item = table->item( row_it, col_it );
			QCOMPARE( QString::fromStdString( inputStr ), item->text() );
		}
	}

	// Next, activate the full table and make sure it's correct
	main->dataTab_->checkBox->setChecked( true );
	table = main->dataTab_->mainTable;
	in = ifstream( "correct_table_long.tsv" );
	colCount = 21;

	// Get rid of the header
	for ( int ii = 0; ii < colCount; ++ii ) in >> inputStr;
	
	for ( int row_it = 0 ; row_it < table->rowCount(); ++row_it ) {
		for ( int col_it = 0; col_it < table->columnCount(); ++col_it ) {
			in >> inputStr;
			item = table->item( row_it, col_it );
			QCOMPARE( QString::fromStdString( inputStr ), item->text() );
		}
	}
}

void TestMainWindow::checkTableExport() {	
	// Export first with full_data selected
	main->dataTab_->checkBox->setChecked( true );
	main->dataTab_->exportTable( "data_table_export.tsv" );
	QCOMPARE( gui_util::checksum( "data_table_export.tsv" ), 
			  gui_util::checksum( "correct_table_long.tsv" ));
	QFile file( "data_table_export.tsv" );
	file.remove();
	
	// Export first with full_data selected
	main->dataTab_->checkBox->setChecked( false );
	main->dataTab_->exportTable( "data_table_export.tsv" );
	QCOMPARE( gui_util::checksum( "data_table_export.tsv" ),
			  gui_util::checksum( "correct_table_short.tsv" ));
	file.remove();
}

void TestMainWindow::checkTableCopy() {	
	// Copy first with full_data selected
	main->dataTab_->checkBox->setChecked( true );
	main->dataTab_->copyAll();

	gui_util::clipboardToFile( "data_table_copy.tsv" );

	QCOMPARE( gui_util::checksum( "data_table_copy.tsv" ), 
			  gui_util::checksum( "correct_table_long.tsv" ));
	QFile file( "data_table_copy.tsv" );
	file.remove();
	
	// Copy now with full_data not selected
	main->dataTab_->checkBox->setChecked( false );
	main->dataTab_->copyAll();

	gui_util::clipboardToFile( "data_table_copy.tsv" );

	QCOMPARE( gui_util::checksum( "data_table_copy.tsv" ), 
			  gui_util::checksum( "correct_table_short.tsv" ));
	file.remove();
}

void TestMainWindow::checkClonotypeTab() {
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

void TestMainWindow::checkClonotypeTableExport() {
	// Export clonotype table
	main->clonotypeTab_->exportTable( "clonotype_table_export.tsv" );
	QCOMPARE( gui_util::checksum( "clonotype_table_export.tsv" ),
			  gui_util::checksum( "correct_clonotype_table.tsv" ));
	QFile file( "clonotype_table_export.tsv" );
	file.remove();
}

void TestMainWindow::checkClonotypeTableCopy() {
	// Copy clonotype table to clipboard
	main->clonotypeTab_->copyAll();
	gui_util::clipboardToFile( "clonotype_table_copy.tsv" );

	QCOMPARE( gui_util::checksum( "clonotype_table_copy.tsv" ),
			  gui_util::checksum( "correct_clonotype_table.tsv" ));

	QFile file( "clonotype_table_copy.tsv" );
	file.remove();
}

void TestMainWindow::checkErrorTab() {

	QCOMPARE( main->errorTab_->beforeErrorRate->text(), "37" );
	QCOMPARE( main->errorTab_->afterErrorRate->text(), "22" );

	auto it = main->errorTab_->plotBars->data()->begin();
	vector<int> histogramValues = { 3, 13, 6, 12, 7, 6, 4, 8, 6, 5, 3, 3, 0, 1, 0, 0, 2 };

	for ( size_t ii = 0; ii < histogramValues.size(); ++ii ) {
		QCOMPARE( it->mainKey(), ii );
		QCOMPARE( it->mainValue(), histogramValues[ ii ] );

		++it;
	}
}

void TestMainWindow::checkGeneTab() {

	using namespace std;

	auto it = main->geneTab_->vBars->data()->begin();
	vector<pair<string,int>> valuesV = { 
		make_pair( "IGHV3-66", 22 ),
		make_pair( "N/A", 21 ),
		make_pair( "IGHV3-23", 12 ),
		make_pair( "IGHV3-NL1", 6 ),
		make_pair( "IGHV3-53", 6 ),
		make_pair( "IGHV3-7", 4 ),
		make_pair( "IGHV3-48", 4 ),
		make_pair( "IGHV3-30", 4 ),
		make_pair( "IGHV3-11", 4 ),
		make_pair( "IGHV3-69-1", 3 )
		};

	vector<pair<string,int>> valuesJ = { 
		make_pair( "N/A", 83 ),
		make_pair( "IGHJ4", 11 ),
		make_pair( "IGHJ1", 6 )
	};

	for ( auto v_it = valuesV.begin(); v_it != valuesV.end(); ++v_it ) {
		QCOMPARE( it->mainValue(), v_it->second );

		++it;
	}

	it = main->geneTab_->jBars->data()->begin();

	for ( auto j_it = valuesJ.begin(); j_it != valuesJ.end(); ++j_it ) {
		QCOMPARE( it->mainValue(), j_it->second );

		++it;
	}
}


void TestMainWindow::checkSummaryTabTSV() {

	cout << "i'm in the TSV shit bitch" << endl; // TODO remove
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

void TestMainWindow::checkDataTabTSV() {
	QTableWidget* table = main->dataTab_->mainTable;

	// First, check the short table and make sure it's correct
	ifstream in ( "correct_table_short_tsv.tsv" );
	string inputStr;
	int colCount = 7;

	// Get rid of the header
	for ( int ii = 0; ii < colCount; ++ii ) in >> inputStr;
	
	QTableWidgetItem* item;
	for ( int row_it = 0 ; row_it < table->rowCount(); ++row_it ) {
		for ( int col_it = 0; col_it < table->columnCount(); ++col_it ) {
			in >> inputStr;
			item = table->item( row_it, col_it );
			QCOMPARE( QString::fromStdString( inputStr ), item->text() );
		}
	}

	// Next, activate the full table and make sure it's correct
	main->dataTab_->checkBox->setChecked( true );
	table = main->dataTab_->mainTable;
	in = ifstream( "correct_table_long.tsv" );
	colCount = 21;

	// Get rid of the header
	for ( int ii = 0; ii < colCount; ++ii ) in >> inputStr;
	
	for ( int row_it = 0 ; row_it < table->rowCount(); ++row_it ) {
		for ( int col_it = 0; col_it < table->columnCount(); ++col_it ) {
			in >> inputStr;
			item = table->item( row_it, col_it );
			QCOMPARE( QString::fromStdString( inputStr ), item->text() );
		}
	}
}

void TestMainWindow::checkTableExportTSV() {	
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

void TestMainWindow::checkTableCopyTSV() {	
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

void TestMainWindow::checkErrorTabTSV() {

	QCOMPARE( main->errorTab_->beforeErrorRate->text(), "54" );
	QCOMPARE( main->errorTab_->afterErrorRate->text(), "32" );

	auto it = main->errorTab_->plotBars->data()->begin();
	QCOMPARE( it->mainKey(), 6 );
	QCOMPARE( it->mainValue(), 250 );
}
 
void TestMainWindow::checkTSVTabsEliminated() {
	QCOMPARE( main->welcomeTab_->isEnabled(), 1 );
	QCOMPARE( main->summaryTab_->isEnabled(), 1 );
	QCOMPARE( main->welcomeTab_->isEnabled(), 1 );
	QCOMPARE( main->geneTab_->isEnabled(), 0 );
	QCOMPARE( main->clonotypeTab_->isEnabled(), 0 );
	QCOMPARE( main->errorTab_->isEnabled(), 1 );
	QCOMPARE( main->dataTab_->isEnabled(), 1 );
	QCOMPARE( main->paramsTab_->isEnabled(), 1 );
}

void TestMainWindow::testFASTQDialog() {
	foreach ( QWidget* w, QApplication::topLevelWidgets() ) {
		ConfirmFile* confirm = qobject_cast<ConfirmFile*>( w );
		if ( confirm != nullptr ) {
			confirm->setFile( "../100.fastq" );

			QCOMPARE( confirm->fileEdit->text(), "../100.fastq" );
			QVERIFY( confirm->radioButtonFASTQ->isChecked() );
			QVERIFY( !confirm->radioButtonTSV->isChecked() );

			QTest::keyClick( confirm, Qt::Key_Enter );
		}
	}
}

void TestMainWindow::testBadFASTQDialog() {
	foreach ( QWidget* w, QApplication::topLevelWidgets() ) {
		ConfirmFile* confirm = qobject_cast<ConfirmFile*>( w );
		if ( confirm != nullptr ) {
			confirm->setFile( "../bad.fastq" );

			QCOMPARE( confirm->fileEdit->text(), "../bad.fastq" );
			QVERIFY( confirm->radioButtonFASTQ->isChecked() );
			QVERIFY( !confirm->radioButtonTSV->isChecked() );

			QTest::keyClick( confirm, Qt::Key_Enter );
		}
	}
}

void TestMainWindow::testTSVDialog() {
	foreach ( QWidget* w, QApplication::topLevelWidgets() ) {
		ConfirmFile* confirm = qobject_cast<ConfirmFile*>( w );
		if ( confirm != nullptr ) {
			confirm->setFile( "../ExampleInput250.tsv" );

			QCOMPARE( confirm->fileEdit->text(), "../ExampleInput250.tsv" );
			QVERIFY( !confirm->radioButtonFASTQ->isChecked() );
			QVERIFY( confirm->radioButtonTSV->isChecked() );

			QTest::keyClick( confirm, Qt::Key_Enter );
		}
	}
}


QTEST_MAIN( TestMainWindow )
#include "TestMainWindow.moc"

