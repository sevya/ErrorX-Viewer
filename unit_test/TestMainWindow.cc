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

	connect( main, &MainWindow::verifySummaryTab,
			 this, &TestMainWindow::checkSummaryTab );

	// Set up options
	main->options_->nthreads( -1 );
	main->options_->error_threshold( errorx::constants::OPTIMIZED_THRESHOLD );
	main->options_->correction( 'N' );
	main->options_->allow_nonproductive( 1 );

	QTimer::singleShot( 1, this, &TestMainWindow::testFASTQDialog );
	main->newProjectAction->trigger();

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );
}

void TestMainWindow::checkSummaryTab() {

	executionDone = 1;

	QCOMPARE( main->summaryTab_->line2->text(), "100" );
	QCOMPARE( main->summaryTab_->line3->text(), "79" );
	QCOMPARE( main->summaryTab_->line4->text(), "59" );
	QCOMPARE( main->summaryTab_->line5->text(), "79" );

	auto it = main->summaryTab_->plotBars->data()->begin();
	QCOMPARE( it->mainKey(), 0 );
	QCOMPARE( it->mainValue(), 100 );

	it++;
	QCOMPARE( it->mainKey(), 1 );
	QCOMPARE( it->mainValue(), 79 );

	it++;
	QCOMPARE( it->mainKey(), 2 );
	QCOMPARE( it->mainValue(), 59 );

	it++;
	QCOMPARE( it->mainKey(), 4 );
	QCOMPARE( it->mainValue(), 79 );

	QCOMPARE( 0, 1 );
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


QTEST_MAIN( TestMainWindow )
#include "TestMainWindow.moc"

