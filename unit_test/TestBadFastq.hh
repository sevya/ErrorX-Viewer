#include <QtTest/QtTest>
#include <QApplication>
#include <QWidgetList>
#include <QTimer>

#include "MainWindow.hh"
#include "gui_util.hh"

class TestBadFastq: public QObject {
	Q_OBJECT

public:
	TestBadFastq();

private slots:
	void testBadRunFromFileMenu();

public slots:
	void testBadFASTQDialog();
	void verifyTabCorrectness();

private:
	MainWindow* main;
	bool executionDone=0;
};