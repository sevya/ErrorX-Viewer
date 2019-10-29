#include <QtTest/QtTest>
#include <QApplication>
#include <QWidgetList>
#include <QTimer>

#include "MainWindow.hh"

class TestFastq: public QObject {
	Q_OBJECT

public:
	TestFastq();

private slots:
	void testGUI();
	void testChecksum();
	void testRunFromFileMenu();

public slots:
	void testFASTQDialog();
	void verifyTabCorrectness();

private:
	MainWindow* main;
	bool executionDone=0;

	void compareFiles( QString const & fileOne, QString const & fileTwo );
	void checkSummaryTab();
	void checkDataTab();
	void checkTableExport();
	void checkTableCopy();
	void checkClonotypeTab();
	void checkClonotypeTableExport();
	void checkClonotypeTableCopy();
	void checkErrorTab();
	void checkGeneTab();
};