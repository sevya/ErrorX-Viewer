#ifndef CDRTAB_HH
#define CDRTAB_HH

#include <QWidget>
#include <QLabel>

#include "qcustomplot.hh"

#include "SequenceRecords.hh"
#include "constants.hh"

class CDRTab : public QWidget {
	Q_OBJECT

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
	QTabWidget* tabWidget;
	QVBoxLayout* cdr1Layout;
	QVBoxLayout* cdr2Layout;
	QVBoxLayout* cdr3Layout;

	QWidget* cdr1Parent;
	QWidget* cdr2Parent;
	QWidget* cdr3Parent;
	QVBoxLayout* mainLayout;

	QCustomPlot* cdr1Plot;
	QCPBars* cdr1Bars;
	QCPTextElement* cdr1Title;

	QCustomPlot* cdr2Plot;
	QCPBars* cdr2Bars;
	QCPTextElement* cdr2Title;

	QCustomPlot* cdr3Plot;
	QCPBars* cdr3Bars;
	QCPTextElement* cdr3Title;


	map<string,vector<int>> cdr_lengths;
};

#endif // CDRTAB_HH
