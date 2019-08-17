#include "ErrorTab.hh"

#include <Qt>

#include <QPushButton>
#include <QStackedLayout>
#include <QGridLayout>

#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>

#include "qcustomplot.hh"

#include <iostream>

#include "SequenceRecords.hh"
#include "SequenceRecord.hh"

ErrorTab::ErrorTab( QWidget* parent ) :
	QWidget( parent ),
	records_( nullptr )
{
	init();
}

ErrorTab::~ErrorTab() {
	delete plot;
	delete plotTitle;
	delete plotBars;
}

void ErrorTab::init() {

	lay1 = new QVBoxLayout( this );
	beforeLabel = new QLabel( "Before correction", this );
	afterLabel  = new QLabel( "After correction" , this );
	lay1->addWidget( beforeLabel );
	lay1->addWidget( afterLabel );
	lay1->addStretch();
	widg1 = new QWidget( this );
	widg1->setLayout( lay1 );

	lay2 = new QVBoxLayout( this );
	beforeErrorRate = new QLabel( "0", this );
	afterErrorRate  = new QLabel( "0", this );

	lay2->addWidget( beforeErrorRate );
	lay2->addWidget( afterErrorRate  );
	lay2->addStretch();
	widg2 = new QWidget( this );
	widg2->setLayout( lay2 );

	lay3 = new QGridLayout( this );
	estimateTitle = new QLabel( "<b>Estimated error rate (errors/kb)</b>", this );
	lay3->addWidget( 
		estimateTitle,
		0, 0, 1, 2, 
		Qt::AlignCenter 
		);

	lay3->addWidget( widg1, 1, 0, Qt::AlignLeft );
	lay3->addWidget( widg2, 1, 1, Qt::AlignRight );

	leftPanel = new QWidget( this );
	leftPanel->setLayout( lay3 );

	initPlot();

	topLayout = new QHBoxLayout( this );
	topLayout->addWidget( leftPanel );
	topLayout->addWidget( plot );
	
	setLayout( topLayout );

}

void ErrorTab::initPlot() {
// Initialize plot
	plot = new QCustomPlot();
	QSizePolicy policy = plot->sizePolicy();
	policy.setHorizontalStretch( 2 );
	plot->setSizePolicy( policy );
	// add title
	QFont font;
	font.setBold( true );
	plot->plotLayout()->insertRow(0);
	plotTitle = new QCPTextElement( plot,  "Error histogram", font );
	plot->plotLayout()->addElement( 0, 0, plotTitle );
}

void ErrorTab::reinitPlot() {
	layout()->removeWidget( plot );
	delete plot;
	initPlot();
	layout()->addWidget( plot );
}

void ErrorTab::update() {

	// If there are no good records, return now
	if ( records_->good_records()==0 ) return;
	
	reinitPlot();

	// Get error rate and set fields
	double preErrorRate = records_->estimate_error_rate();
	beforeErrorRate->setText( QString::number(preErrorRate*1000, 'g', 2 ));
	double estimatedRecall = errorx::constants::OPTIMIZED_RECALL;
	double postErrorRate = preErrorRate * (1.0-estimatedRecall);
	afterErrorRate->setText( QString::number(postErrorRate*1000, 'g', 2 ));

	// Get error values
	vector<int> errors;
	vector<errorx::SequenceRecord*>::iterator it;
	for ( int ii = 0; ii < records_->size(); ++ii ) {
		if ( records_->get(ii)->isGood() ) {
			errors.push_back( records_->get(ii)->n_errors() );
		}
	}

	// Bin errors into a histogram
	int min = 0;
	int max = *(max_element( errors.begin(), errors.end() ));

	// This algorithm only works bc the min is at zero,
	// so I can easily infer the indices
	vector<int> bins( max-min+1, 0 );
	for ( size_t ii = 0; ii < errors.size(); ++ii ) {
		bins[ errors[ii] ]++;
	}

	QVector<double>  binCounts;
	QVector<QString> binNames;
	QVector<double>  binIndices;

	for ( size_t ii = 0; ii < bins.size(); ++ii ) {
		binNames   << QString::number( ii );
		binCounts  << bins[ii];
		binIndices << ii;
	}

	// make bars object
	plotBars = new QCPBars(
				plot->xAxis,
				plot->yAxis
				);
	// set my custom color
	QColor color( 25, 89, 240, 130 );
	plotBars->setPen( QPen(color ));
	plotBars->setBrush( color );

	// helps the edges of the bars look crisper
	plotBars->setAntialiased( false );

	// set up the axes
	QSharedPointer<QCPAxisTickerText> textTicker( new QCPAxisTickerText );
	textTicker->addTicks( binIndices, binNames );
	plot->xAxis->setTicker( textTicker );
	plot->xAxis->setLabel( "# Errors per sequence" );
	plot->yAxis->setLabel( "Counts" );

	// add data to plot
	plotBars->setData( binIndices, binCounts );

	plot->rescaleAxes( false );
	plot->xAxis->setRangeLower( -1 );
	plot->xAxis->setRangeUpper( bins.size() );
	plot->replot();
}

void ErrorTab::exportPDF( QString const & outputDir ) {
	plot->savePdf( outputDir + QDir::separator() + "error_profile.pdf" );
}
void ErrorTab::records( errorx::SequenceRecordsSP const records ) { records_ = records; }
errorx::SequenceRecordsSP ErrorTab::records() { return records_; }
