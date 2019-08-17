#ifndef SUMMARYTAB_HH
#define SUMMARYTAB_HH

#include <QWidget>
#include <QLabel>
#include <QString>

#include "qcustomplot.hh"

#include "SequenceRecords.hh"
#include "constants.hh"

class SummaryTab : public QWidget {
	Q_OBJECT

public:
	explicit SummaryTab( QWidget *parent=0 );

	~SummaryTab();
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
	QGridLayout* leftLayout;
	QLabel* fileLabel;
	QLabel* numLabel;
	QLabel* numLabel2;
	QLabel* numLabel3;
	QLabel* uniqueLabel;

	QLineEdit* line1;
	QLineEdit* line2;
	QLineEdit* line3;
	QLineEdit* line4;
	QLineEdit* line5;

	QWidget* filler;
	QWidget* leftPanel;
	QHBoxLayout* mainLayout;

	QCustomPlot* plot;
	QCPTextElement* plotTitle;
	QCPBars* plotBars;

	friend class TestMainWindow;
};

#endif // SUMMARYTAB_HH
