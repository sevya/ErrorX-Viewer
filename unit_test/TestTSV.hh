#include "MainWindow.hh"

class TestTSV: public QObject {
	Q_OBJECT

public:
	TestTSV();

private slots:
	void testRunFromFileMenuTSV();

public slots:
	void testTSVDialog();
	void verifyTabCorrectness();

private:
	MainWindow* main;
	bool executionDone=0;

	void checkSummaryTab();
	void checkDataTab();
	void checkTableExport();
	void checkTableCopy();
	void checkErrorTab();
	void checkTSVTabsEliminated();

};