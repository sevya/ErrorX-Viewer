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
};

void TestMainWindow::testGUI() {
	MainWindow* main = new MainWindow();

	QCOMPARE( main->tabWidget->count(), 7 );
}


void TestMainWindow::testRunFromFileMenu() {
	MainWindow* main = new MainWindow();

	QTimer::singleShot( 1, this, &TestMainWindow::testFASTQDialog );
	main->newProjectAction->trigger();

	QCOMPARE( main->summaryTab_->line2->text(), "100" );
	QCOMPARE( main->summaryTab_->line3->text(), "79" );
	QCOMPARE( main->summaryTab_->line2->text(), "59" );
	QCOMPARE( main->summaryTab_->line2->text(), "79" );

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

