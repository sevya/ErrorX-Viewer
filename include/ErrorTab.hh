#ifndef ERRORTAB_HH
#define ERRORTAB_HH

#include <QWidget>
#include <QLabel>

#include "qcustomplot.hh"

#include "SequenceRecords.hh"
#include "constants.hh"

class ErrorTab : public QWidget {
	Q_OBJECT

#ifdef UNITTEST
#include "../unit_test/friendship.hh"
#endif

public:
	explicit ErrorTab( QWidget *parent=0 );

	~ErrorTab();
	void update();

	void records( errorx::SequenceRecordsSP const records );
	errorx::SequenceRecordsSP records();

signals:
public slots:
	void exportPDF( QString const & outputDir );



private:
	void init();
	void initPlot();
	void reinitPlot();

	errorx::SequenceRecordsSP records_;
	QVBoxLayout* lay1;
	QLabel* beforeLabel;
	QLabel* afterLabel;
	QWidget* widg1;
	QVBoxLayout* lay2;
	QWidget* widg2;
	QGridLayout* lay3;
	QLabel* beforeErrorRate;
	QLabel* afterErrorRate;
	QLabel* estimateTitle;
	QWidget* leftPanel;
	QHBoxLayout* topLayout;

	QCustomPlot* plot;
	QCPTextElement* plotTitle;
	QCPBars* plotBars;
};

#endif // ERRORTAB_HH
