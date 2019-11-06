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
	createTabWidget();
}

void CDRTab::createTabWidget() {
	tabWidget = new QTabWidget( this );
	tabWidget->setTabPosition( QTabWidget::South );

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

	cdr1Plot = new QCustomPlot();
	cdr2Plot = new QCustomPlot();
	cdr3Plot = new QCustomPlot();

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

void CDRTab::createAltWidget() {
	altWidget = new QWidget();
	altLayout = new QVBoxLayout();
	QLabel* text = new QLabel( "<b>CDR view is not available when using TSV input</b>", this );
	text->setAlignment(Qt::AlignCenter);
	altLayout->addWidget( text );
	altWidget->setLayout( altLayout );
	setLayout( altLayout );
}

void CDRTab::update() {
	if ( records_==nullptr ) return;
	
	// Remove whatever is currently in the layout
	qDeleteAll( this->children() );

	// TSV input does not have this tab
	if ( records_->get_options()->format() == "tsv" ) {
		setEnabled( false );

		// Add in the alternative layout
		createAltWidget();
	} else {
		setEnabled( true );

		// Add in the TabWidget regular layout
		createTabWidget();

		// Update plots
		cdrLengths = records_->cdr_lengths();

		updateCDR1();
		updateCDR2();
		updateCDR3();
	}
}

void CDRTab::updateCDR1() {
	if ( records_==nullptr ) return;

	// reinitCDR1Plot();

	// Get CDR1
	map<int,float> bins = errorx::util::bin_values( cdrLengths[ "CDR1" ],
							/*normalized=*/0 );

	// If there are no valid CDRs, return
	if ( bins.empty() ) return;

	QVector<double>  binCounts;
	QVector<QString> binNames;
	QVector<double>  binIndices;

	map<int,float>::const_iterator bin_it;
	int ii = 0;

	for ( bin_it = bins.begin(); bin_it != bins.end(); ++bin_it ) {
		binNames   << QString::number( bin_it->first );
		binCounts  << bin_it->second;
		binIndices << ii;
		ii++;
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
	cdr1Plot->xAxis->setLabel( "Length" );
	cdr1Plot->yAxis->setLabel( "Counts" );

	// add data to plot
	cdr1Bars->setData( binIndices, binCounts );

	// add title
	QFont font;
	font.setBold( true );
	cdr1Plot->plotLayout()->insertRow( 0 );
	cdr1Title = new QCPTextElement( cdr1Plot, "CDR1 lengths", font );
	cdr1Plot->plotLayout()->addElement( 0, 0, cdr1Title );

	
	cdr1Plot->rescaleAxes( false );
	cdr1Plot->xAxis->setRangeLower( -1 );
	cdr1Plot->xAxis->setRangeUpper( bins.size() );
	cdr1Plot->replot();
}

void CDRTab::updateCDR2() {
	if ( records_==nullptr ) return;

	// reinitCDR2Plot();

	// Get CDR2
	map<int,float> bins = errorx::util::bin_values( cdrLengths[ "CDR2" ],
							/*normalized=*/0 );

	// If there are no valid CDRs, return
	if ( bins.empty() ) return;

	QVector<double>  binCounts;
	QVector<QString> binNames;
	QVector<double>  binIndices;

	map<int,float>::const_iterator bin_it;
	int ii = 0;

	for ( bin_it = bins.begin(); bin_it != bins.end(); ++bin_it ) {
		binNames   << QString::number( bin_it->first );
		binCounts  << bin_it->second;
		binIndices << ii;
		ii++;
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
	cdr2Plot->xAxis->setLabel( "Length" );
	cdr2Plot->yAxis->setLabel( "Counts" );

	// add data to plot
	cdr2Bars->setData( binIndices, binCounts );

	// add title
	QFont font;
	font.setBold( true );
	cdr2Plot->plotLayout()->insertRow( 0 );
	cdr2Title = new QCPTextElement( cdr2Plot, "CDR2 lengths", font );
	cdr2Plot->plotLayout()->addElement( 0, 0, cdr2Title );

	cdr2Plot->rescaleAxes( false );
	cdr2Plot->xAxis->setRangeLower( -1 );
	cdr2Plot->xAxis->setRangeUpper( bins.size() );
	cdr2Plot->replot();
}


void CDRTab::updateCDR3() {
	if ( records_==nullptr ) return;

	// reinitCDR3Plot();

	// Get CDR3
	map<int,float> bins = errorx::util::bin_values( cdrLengths[ "CDR3" ],
							/*normalized=*/0 );

	// If there are no valid CDRs, return
	if ( bins.empty() ) return;

	QVector<double>  binCounts;
	QVector<QString> binNames;
	QVector<double>  binIndices;

	map<int,float>::const_iterator bin_it;
	int ii = 0;

	for ( bin_it = bins.begin(); bin_it != bins.end(); ++bin_it ) {
		binNames   << QString::number( bin_it->first );
		binCounts  << bin_it->second;
		binIndices << ii;
		ii++;
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
	cdr3Plot->xAxis->setLabel( "Length" );
	cdr3Plot->yAxis->setLabel( "Counts" );

	// add data to plot
	cdr3Bars->setData( binIndices, binCounts );

	// add title
	QFont font;
	font.setBold( true );
	cdr3Plot->plotLayout()->insertRow( 0 );
	cdr3Title = new QCPTextElement( cdr3Plot, "CDR3 lengths", font );
	cdr3Plot->plotLayout()->addElement( 0, 0, cdr3Title );


	cdr3Plot->rescaleAxes( false );
	cdr3Plot->xAxis->setRangeLower( -1 );
	cdr3Plot->xAxis->setRangeUpper( bins.size() );
	cdr3Plot->replot();
}

void CDRTab::exportPDF( QString const & outputDir ) {
	
	cdr1Plot->savePdf( outputDir + QDir::separator() + "cdr1_lengths.pdf", 680, 480 );
	cdr2Plot->savePdf( outputDir + QDir::separator() + "cdr2_lengths.pdf", 640, 480 );
	cdr3Plot->savePdf( outputDir + QDir::separator() + "cdr3_lengths.pdf", 640, 480 );
}

void CDRTab::records( errorx::SequenceRecordsSP const records ) { records_ = records; }
errorx::SequenceRecordsSP CDRTab::records() { return records_; }