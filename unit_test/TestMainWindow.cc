#include <QtTest/QtTest>
#include <QApplication>
#include <QWidgetList>
#include <QTimer>

#include "MainWindow.hh"

class TestMainWindow: public QObject {
	Q_OBJECT

private slots:
	void testGUI();
	void testRunFromFileMenu();

public slots:
	void testFASTQDialog();
	void checkSummaryTab();

private:
	MainWindow* main;
	bool executionDone=0;
};

void TestMainWindow::testGUI() {
	main = new MainWindow();

	QCOMPARE( main->tabWidget->count(), 7 );
}


void TestMainWindow::testRunFromFileMenu() {
	main = new MainWindow();

	// Connect action handlers to check tab correctness after processing
	connect( main, &MainWindow::verifyTabValues,
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

void TestMainWindow::verifyTabCorrectness() {
	executionDone = 1;

	checkSummaryTab();
	checkDataTab();
	checkTableExport();
	checkClonotypeTab();
	checkClonotypeTableExport();
	checkErrorTab();
	checkGeneTab();
}

void TestMainWindow::checkSummaryTab() {

	QCOMPARE( main->summaryTab_->line2->text(), "100" );
	QCOMPARE( main->summaryTab_->line3->text(), "79" );
	QCOMPARE( main->summaryTab_->line4->text(), "48" );
	QCOMPARE( main->summaryTab_->line5->text(), "79" );

	auto it = main->summaryTab_->plotBars->data()->begin();
	QCOMPARE( it->mainKey(), 0 );
	QCOMPARE( it->mainValue(), 100 );

	it++;
	QCOMPARE( it->mainKey(), 1 );
	QCOMPARE( it->mainValue(), 79 );

	it++;
	QCOMPARE( it->mainKey(), 2 );
	QCOMPARE( it->mainValue(), 48 );

	it++;
	QCOMPARE( it->mainKey(), 3 );
	QCOMPARE( it->mainValue(), 79 );

	QCOMPARE( 0, 1 );
}

void TestMainWindow::checkDataTab() {
	QTableWidget* table = main->dataTab_->mainTable;

	// TODO create the correct table file
	//
	// First, check the short table and make sure it's correct
	ifstream in ( "correct_table_short.tsv" );
	string inputStr;
	int colCount = 5; // TODO set correct colCount

	// Get rid of the header
	for ( int ii = 0; ii < colCount; ++ii ) in >> inputStr;
	
	QTableWidgetItem* item;
	for ( int row_it = 0 ; row_it < table->rowCount(); ++row_it ) {
		for ( int col_it = 0; col_it < table->columnCount(); ++col_it ) {
			in >> inputStr;
			item = table->item( row_it, col_it );
			QCOMPARE( inputStr, item.text() );
		}
	}

	// Next, activate the full table and make sure it's correct
	main->dataTab_->checkBox->setSelected( true );
	main->dataTab_->mainTable;
	in ( "correct_table_long.tsv" );
	colCount = 53; // TODO set correct colCount

	// Get rid of the header
	for ( int ii = 0; ii < colCount; ++ii ) in >> inputStr;
	
	for ( int row_it = 0 ; row_it < table->rowCount(); ++row_it ) {
		for ( int col_it = 0; col_it < table->columnCount(); ++col_it ) {
			in >> inputStr;
			item = table->item( row_it, col_it );
			QCOMPARE( inputStr, item.text() );
		}
	}
}

void TestMainWindow::checkTableExport() {
	// TODO create correct data tables
	
	// Export first with full_data selected
	main->dataTab_->checkBox->setSelected( true );
	main->dataTab_->exportTable( "data_table_export.tsv" );
	QCOMPARE( checksum( "data_table_export.tsv" ), 
			  checksum( "correct_table_long.tsv" ));
	QFile file( "data_table_export.tsv" );
	file.remove();
	
	// Export first with full_data selected
	main->dataTab_->checkBox->setSelected( false );
	main->dataTab_->exportTable( "data_table_export.tsv" );
	QCOMPARE( checksum( "data_table_export.tsv" ),
			  checksum( "correct_table_long.tsv" ));
	file.remove();
}

void TestMainWindow::checkClonotypeTab() {
	QTableWidget* table = main->clonotypeTab_->cloneTable;

	// TODO create the correct table file
	ifstream in ( "correct_clonotype_table.tsv" );
	string inputStr;
	int colCount = 3;

	// Get rid of the header
	for ( int ii = 0; ii < colCount; ++ii ) in >> inputStr;

	QTableWidgetItem* item;
	for ( int row_it = 0 ; row_it < table->rowCount(); ++row_it ) {
		for ( int col_it = 0; col_it < table->columnCount(); ++col_it ) {
			in >> inputStr;
			item = table->item( row_it, col_it );
			QCOMPARE( inputStr, item.text() );
		}
	}
}

void TestMainWindow::checkClonotypeTableExport() {
	// Export clonotype table
	main->dataTab_->checkBox->setSelected( true );
	main->dataTab_->exportTable( "clonotype_table_export.tsv" );
	QCOMPARE( checksum( "clonotype_table_export.tsv" ),
			  checksum( "correct_clonotype_table.tsv" ));
	QFile file( "clonotype_table_export.tsv" );
	file.remove();
}

void TestMainWindow::checkErrorTab() {

	// TODO input correct values here
	QCOMPARE( main->errorTab_->beforeErrorRate->text(), "100" );
	QCOMPARE( main->errorTab_->afterErrorRate->text(), "100" );

	auto it = main->errorTab_->plotBars->data()->begin();
	std::vector<int> histogramValues = { /* TODO input histogram values */ };

	for ( int ii = 0; ii < histogramValues.size(); ++ii ) {
		QCOMPARE( it->mainKey(), ii );
		QCOMPARE( it->mainValue(), histogramValues[ ii ] );

		++it;
	}
}

void TestMainWindow::checkGeneTab() {

    auto it = main->geneTab_->vBars->data()->begin();
    std::map<string,int> valuesV = { /* TODO input V gene values */ };
	std::map<string,int> valuesJ = { /* TODO input J gene values */ };

    for ( int ii = 0; ii < valuesV.size(); ++ii ) {
        QCOMPARE( it->mainKey(), ii );
        QCOMPARE( it->mainValue(), valuesV[ ii ] );

        ++it;
    }

	it = main->geneTab_->jBars->data()->begin();
	for ( int ii = 0; ii < valuesJ.size(); ++ii ) {
        QCOMPARE( it->mainKey(), ii );
        QCOMPARE( it->mainValue(), valuesJ[ ii ] );

        ++it;
    }
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

QByteArray checksum( QString const & fileName ) {
	QCryptographicHash hash( QCryptographicHash::Sha1 );
	QFile file( fileName );

	if ( file.open( QIODevice::ReadOnly ) ) {
		hash.addData( file.readAll() );
	}

	// Retrieve the SHA1 signature of the file
	return hash.result();
}

QTEST_MAIN( TestMainWindow )
#include "TestMainWindow.moc"

