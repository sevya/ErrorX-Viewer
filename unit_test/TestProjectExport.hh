#include <QtTest/QtTest>
#include <QApplication>
#include <QWidgetList>
#include <QTimer>

#include "MainWindow.hh"
#include "gui_util.hh"

class TestProjectExport: public QObject {
	Q_OBJECT

public:
	TestProjectExport();

private slots:
	void testRunFromFileMenu();

public slots:
	void enterDialog();
	void verifyTabCorrectness();

private:
	void verifyExport();
	void compareFiles( QString const & fileOne, QString const & fileTwo );

	MainWindow* main;
	bool executionDone=0;
};