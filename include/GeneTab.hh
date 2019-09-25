#ifndef GENETAB_HH
#define GENETAB_HH

#include <QWidget>
#include <QLabel>

#include "qcustomplot.hh"

#include "SequenceRecords.hh"
#include "constants.hh"

class GeneTab : public QWidget {
	Q_OBJECT

#ifdef UNITTEST
#include "../unit_test/friendship.hh"
#endif

public:
	explicit GeneTab( QWidget *parent=0 );

	~GeneTab();
	void update();

	void records( errorx::SequenceRecordsSP const records );
	errorx::SequenceRecordsSP records();

signals:
public slots:
	void exportPDF( QString const & outputDir );


private:
	void init();
	void reinitVGenePlot();
	void reinitJGenePlot();

	void updateVGene();
	void updateJGene();

	errorx::SequenceRecordsSP records_;
	QTabWidget* tabWidget;
	QVBoxLayout* vlayout;
	QVBoxLayout* jlayout;
	QWidget* vparent;
	QWidget* jparent;
	QVBoxLayout* mainLayout;

	QCustomPlot* vGenePlot;
	QCPBars* vBars;
	QCPTextElement* vTitle;
	QSharedPointer<QCPAxisTickerText> vTicker;

	QCustomPlot* jGenePlot;
	QCPBars* jBars;
	QCPTextElement* jTitle;
	QSharedPointer<QCPAxisTickerText> jTicker;
};

#endif // GENETAB_HH
