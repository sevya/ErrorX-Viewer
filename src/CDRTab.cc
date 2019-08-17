#include "CDRTab.hh"

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

CDRTab::CDRTab( QWidget* parent ) :
	QWidget( parent ),
	records_( nullptr )
{
	init();
}

CDRTab::~CDRTab() {
	delete cdr1Plot;
	delete cdr1Bars;
	delete cdr1Title;

	delete cdr2Plot;
	delete cdr2Bars;
	delete cdr2Title;

	delete cdr3Plot;
	delete cdr3Bars;
	delete cdr3Title;
}

void CDRTab::init() {
	tabWidget = new QTabWidget( this );
	tabWidget->setTabPosition( QTabWidget::South );

	cdr1Plot = new QCustomPlot();
	cdr2Plot = new QCustomPlot();
	cdr3Plot = new QCustomPlot();

	// These basically don't do anything, they just give the tabs a layout
	cdr1Layout = new QVBoxLayout(); // this );
	cdr2Layout = new QVBoxLayout(); // this );
	cdr3Layout = new QVBoxLayout(); // this );

	cdr1Parent = new QWidget( this );
	cdr2Parent = new QWidget( this );
	cdr3Parent = new QWidget( this );

	cdr1Parent->setLayout( cdr1Layout );
	cdr2Parent->setLayout( cdr2Layout );
	cdr3Parent->setLayout( cdr3Layout );

	cdr1Layout->addWidget( cdr1Plot );
	cdr2Layout->addWidget( cdr2Plot );
	cdr3Layout->addWidget( cdr3Plot );

	tabWidget->addTab( cdr1Parent, "CDR1" );
	tabWidget->addTab( cdr2Parent, "CDR2" );
	tabWidget->addTab( cdr3Parent, "CDR3" );

	mainLayout = new QVBoxLayout(); // this );
	mainLayout->addWidget( tabWidget );
	setLayout( mainLayout );
}

void CDRTab::reinitCDR1Plot() {
	cdr1Layout->removeWidget( cdr1Plot );
	delete cdr1Plot;
	cdr1Plot = new QCustomPlot();
	cdr1Layout->addWidget( cdr1Plot );
}

void CDRTab::reinitCDR2Plot() {
	cdr2Layout->removeWidget( cdr2Plot );
	delete cdr2Plot;
	cdr2Plot = new QCustomPlot();
	cdr2Layout->addWidget( cdr2Plot );
}

void CDRTab::reinitCDR3Plot() {
	cdr3Layout->removeWidget( cdr3Plot );
	delete cdr3Plot;
	cdr3Plot = new QCustomPlot();
	cdr3Layout->addWidget( cdr3Plot );
}


void CDRTab::update() {
	// TSV input does not have this tab
	if ( records_->get_options()->format() == "tsv" ) {
		setEnabled( false );
		// clear plots
		reinitCDR1Plot();
		reinitCDR2Plot();
		reinitCDR3Plot();
		return;
	} else {
		setEnabled( true );

		cdr_lengths.clear();
		cdr_lengths = records_->cdr_lengths();

		updateCDR1();
		updateCDR2();
		updateCDR3();
	}
}

void CDRTab::updateCDR1() {
	if ( records_==nullptr ) return;

	reinitCDR1Plot();

	// Get CDR1 lengths
	vector<int> lengths = cdr_lengths["CDR1"];

	// If there are no valid values, return
	if ( lengths.empty() ) return;

	// Bin the values by length
	map<int,float> bins = errorx::util::bin_values( lengths, /*normalized=*/0 );

	QVector<double>  binCounts;
	QVector<QString> binNames;
	QVector<double>  binIndices;

	map<int,float>::const_iterator bin_it;
	for ( bin_it = bins.begin(); bin_it != bins.end(); ++bin_it ) {
		binNames   << QString::number( bin_it->first );
		binIndices << bin_it->first;
		binCounts  << bin_it->second;
	}


	// make bars object
	cdr1Bars = new QCPBars(
				cdr1Plot->xAxis,
				cdr1Plot->yAxis
				);
	// set my custom color
	QColor color( 25, 89, 240, 130 );
	cdr1Bars->setPen( QPen(color ));
	cdr1Bars->setBrush( color );

	// helps the edges of the bars look crisper
	cdr1Bars->setAntialiased( false );

	// set up the axes
	QSharedPointer<QCPAxisTickerText> textTicker( new QCPAxisTickerText );
	textTicker->addTicks( binIndices, binNames );
	cdr1Plot->xAxis->setTicker( textTicker );
	cdr1Plot->xAxis->setLabel( "CDR1 Length" );
	cdr1Plot->yAxis->setLabel( "Counts" );

	// add data to plot
	cdr1Bars->setData( binIndices, binCounts );

	cdr1Plot->rescaleAxes( false );
	cdr1Plot->xAxis->setRangeLower( bins.begin()->first );
	cdr1Plot->xAxis->setRangeUpper( bins.begin()->second );
	cdr1Plot->replot();
}



void CDRTab::updateCDR2() {
	if ( records_==nullptr ) return;

	reinitCDR1Plot();

	// Get CDR1 lengths
	vector<int> lengths = cdr_lengths["CD2"];

	// If there are no valid values, return
	if ( lengths.empty() ) return;

	// Bin the values by length
	map<int,float> bins = errorx::util::bin_values( lengths, /*normalized=*/0 );

	QVector<double>  binCounts;
	QVector<QString> binNames;
	QVector<double>  binIndices;

	map<int,float>::const_iterator bin_it;
	for ( bin_it = bins.begin(); bin_it != bins.end(); ++bin_it ) {
		binNames   << QString::number( bin_it->first );
		binIndices << bin_it->first;
		binCounts  << bin_it->second;
	}


	// make bars object
	cdr2Bars = new QCPBars(
				cdr2Plot->xAxis,
				cdr2Plot->yAxis
				);
	// set my custom color
	QColor color( 25, 89, 240, 130 );
	cdr2Bars->setPen( QPen(color ));
	cdr2Bars->setBrush( color );

	// helps the edges of the bars look crisper
	cdr2Bars->setAntialiased( false );

	// set up the axes
	QSharedPointer<QCPAxisTickerText> textTicker( new QCPAxisTickerText );
	textTicker->addTicks( binIndices, binNames );
	cdr2Plot->xAxis->setTicker( textTicker );
	cdr2Plot->xAxis->setLabel( "CDR1 Length" );
	cdr2Plot->yAxis->setLabel( "Counts" );

	// add data to plot
	cdr2Bars->setData( binIndices, binCounts );

	cdr2Plot->rescaleAxes( false );
	cdr2Plot->xAxis->setRangeLower( bins.begin()->first );
	cdr2Plot->xAxis->setRangeUpper( bins.begin()->second );
	cdr2Plot->replot();
}


void CDRTab::updateCDR3() {
	if ( records_==nullptr ) return;

	reinitCDR1Plot();

	// Get CDR1 lengths
	vector<int> lengths = cdr_lengths["CDR3"];

	// If there are no valid values, return
	if ( lengths.empty() ) return;

	// Bin the values by length
	map<int,float> bins = errorx::util::bin_values( lengths, /*normalized=*/0 );

	QVector<double>  binCounts;
	QVector<QString> binNames;
	QVector<double>  binIndices;

	map<int,float>::const_iterator bin_it;
	for ( bin_it = bins.begin(); bin_it != bins.end(); ++bin_it ) {
		binNames   << QString::number( bin_it->first );
		binIndices << bin_it->first;
		binCounts  << bin_it->second;
	}


	// make bars object
	cdr3Bars = new QCPBars(
				cdr3Plot->xAxis,
				cdr3Plot->yAxis
				);
	// set my custom color
	QColor color( 25, 89, 240, 130 );
	cdr3Bars->setPen( QPen(color ));
	cdr3Bars->setBrush( color );

	// helps the edges of the bars look crisper
	cdr3Bars->setAntialiased( false );

	// set up the axes
	QSharedPointer<QCPAxisTickerText> textTicker( new QCPAxisTickerText );
	textTicker->addTicks( binIndices, binNames );
	cdr3Plot->xAxis->setTicker( textTicker );
	cdr3Plot->xAxis->setLabel( "CDR1 Length" );
	cdr3Plot->yAxis->setLabel( "Counts" );

	// add data to plot
	cdr3Bars->setData( binIndices, binCounts );

	cdr3Plot->rescaleAxes( false );
	cdr3Plot->xAxis->setRangeLower( bins.begin()->first );
	cdr3Plot->xAxis->setRangeUpper( bins.begin()->second );
	cdr3Plot->replot();
}


void CDRTab::exportPDF( QString const & outputDir ) {
	cdr1Plot->savePdf( outputDir + QDir::separator() + "cdr1_histogram.pdf" );
	cdr2Plot->savePdf( outputDir + QDir::separator() + "cdr2_histogram.pdf" );
	cdr3Plot->savePdf( outputDir + QDir::separator() + "cdr3_histogram.pdf" );
}

void CDRTab::records( errorx::SequenceRecordsSP const records ) { records_ = records; }
errorx::SequenceRecordsSP CDRTab::records() { return records_; }