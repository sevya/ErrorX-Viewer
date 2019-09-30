#include <QtTest/QtTest>
#include <QApplication>
#include <QWidgetList>
#include <QTimer>

#include "MainWindow.hh"

class TestFasta: public QObject {
	Q_OBJECT

public:
	TestFasta();

private slots:
	void testGUI();
	void testRunFromFileMenu();

public slots:
	void testFASTADialog();
	void verifyTabCorrectness();

private:
	MainWindow* main;
	bool executionDone=0;

	void checkSummaryTab();
	void checkDataTab();
	void checkTableExport();
	void checkTableCopy();
	void checkClonotypeTab();
	void checkClonotypeTableExport();
	void checkClonotypeTableCopy();
	void checkGeneTab();
};