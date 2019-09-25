#include <QtTest/QtTest>
#include <QApplication>
#include <QWidgetList>
#include <QTimer>

#include "MainWindow.hh"
#include "gui_util.hh"

class TestProjectImport: public QObject {
	Q_OBJECT

public:
	TestProjectImport();

private slots:
	void testRun();

public slots:
	void verifyTabCorrectness();

private:
	void verifyImport();
	void checkSummaryTab();
	void checkDataTab();
	void checkClonotypeTab();
	void checkErrorTab();
	void checkGeneTab();

	MainWindow* main;
	bool executionDone=0;
};