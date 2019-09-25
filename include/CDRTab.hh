#ifndef CDRTab_HH
#define CDRTab_HH

#include <QWidget>
#include <QLabel>

#include "qcustomplot.hh"

#include "SequenceRecords.hh"
#include "constants.hh"

class CDRTab : public QWidget {
	Q_OBJECT

#ifdef UNITTEST
#include "../unit_test/friendship.hh"
#endif

public:
	explicit CDRTab( QWidget *parent=0 );

	~CDRTab();
	void update();

	void records( errorx::SequenceRecordsSP const records );
	errorx::SequenceRecordsSP records();

signals:
public slots:
	void exportPDF( QString const & outputDir );


private:
	void init();
	void reinitCDR1Plot();
	void reinitCDR2Plot();
	void reinitCDR3Plot();

	void updateCDR1();
	void updateCDR2();
	void updateCDR3();

	errorx::SequenceRecordsSP records_;
	map<string,vector<int>> cdrLengths;

	QTabWidget* tabWidget;
	
	QVBoxLayout* mainLayout;
	QVBoxLayout* cdr1Layout;
	QVBoxLayout* cdr2Layout;
	QVBoxLayout* cdr3Layout;

	QWidget* cdr1Parent;
	QWidget* cdr2Parent;
	QWidget* cdr3Parent;

	QCustomPlot* cdr1Plot;
	QCPBars* cdr1Bars;
	QCPTextElement* cdr1Title;

	QCustomPlot* cdr2Plot;
	QCPBars* cdr2Bars;
	QCPTextElement* cdr2Title;

	QCustomPlot* cdr3Plot;
	QCPBars* cdr3Bars;
	QCPTextElement* cdr3Title;
};

#endif // CDRTab_HH
