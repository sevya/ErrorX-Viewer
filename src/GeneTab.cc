#include "GeneTab.hh"

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

GeneTab::GeneTab( QWidget* parent ) :
	QWidget( parent ),
	records_( nullptr )
{
	init();
}

GeneTab::~GeneTab() {
	delete vGenePlot;
	delete vBars;
	delete vTitle;

	delete jGenePlot;
	delete jBars;
	delete jTitle;
}

void GeneTab::init() {
	createTabWidget();
}

void GeneTab::createTabWidget() {
	tabWidget = new QTabWidget( this );
	tabWidget->setTabPosition( QTabWidget::South );

	vGenePlot = new QCustomPlot();
	jGenePlot = new QCustomPlot();

	// These basically don't do anything, they just give the tabs a layout
	vlayout = new QVBoxLayout();
	jlayout = new QVBoxLayout(); 

	vparent = new QWidget( tabWidget );
	jparent = new QWidget( tabWidget );
	vparent->setLayout( vlayout );
	jparent->setLayout( jlayout );

	vlayout->addWidget( vGenePlot );
	jlayout->addWidget( jGenePlot );

	tabWidget->addTab( vparent, "V Genes" );
	tabWidget->addTab( jparent, "J Genes" );

	mainLayout = new QVBoxLayout();
	mainLayout->addWidget( tabWidget );
	setLayout( mainLayout );
}

void GeneTab::createAltWidget() {
	altWidget = new QWidget();
	altLayout = new QVBoxLayout();
	QLabel* text = new QLabel( "<b>Gene view is not available when using TSV input</b>", this );
	text->setAlignment(Qt::AlignCenter);
	altLayout->addWidget( text );
	altWidget->setLayout( altLayout );
	setLayout( altLayout );
}

void GeneTab::update() {
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
		updateVGene();
		updateJGene();
	}
}

void GeneTab::updateVGene() {
	if ( records_==nullptr ) return;

	// reinitVGenePlot();

	// Get V gene values
	map<string,int> vgeneMap = records_->vgene_counts();

	// If there are no valid vgenes, return
	if ( vgeneMap.empty() ) return;

	// Only show top N genes on the plot
	size_t N = 10;

	// Get a vector with v gene values sorted by values descending
	vector<pair<string,int>> vgeneVector = errorx::util::sort_map( vgeneMap, 0 );

	// If N is less than the length of the V gene vector, adjust accordingly
	N = ( N < vgeneVector.size() ) ? N : vgeneVector.size();

	QVector<double>  geneCounts;
	QVector<QString> geneNames;
	QVector<double>  geneIndices;
	
	for ( size_t ii = 0; ii < N; ++ii ) {
		geneNames   << QString::fromStdString( vgeneVector[ ii ].first );
		geneCounts  << vgeneVector[ ii ].second;
		geneIndices << ii;
	}

	// make bars object
	vBars = new QCPBars(
				vGenePlot->xAxis,
				vGenePlot->yAxis
				);
	// set my custom color
	QColor color( 25, 89, 240, 130 );
	vBars->setPen( QPen(color ));
	vBars->setBrush( color );

	// helps the edges of the bars look crisper
	vBars->setAntialiased( false );

	// set up the axes
	vTicker = QSharedPointer<QCPAxisTickerText>( new QCPAxisTickerText );
	vTicker->addTicks( geneIndices, geneNames );
	vGenePlot->xAxis->setTicker( vTicker );
	vGenePlot->xAxis->setTickLabelRotation( 90 );
	vGenePlot->yAxis->setLabel( "Counts" );

	// add data to plot
	vBars->setData( geneIndices, geneCounts );

	// add title
	QFont font;
	font.setBold( true );
	vGenePlot->plotLayout()->insertRow( 0 );
	vTitle = new QCPTextElement( vGenePlot, "V gene summary", font );
	vGenePlot->plotLayout()->addElement( 0, 0, vTitle );


	// Label the tops of the bars
	// TODO clean up these references
	QFont labelFont;
	labelFont.setPointSize( 10 );
	for ( size_t ii = 0; ii < N; ++ii ) {
		QCPItemText* textLabel = new QCPItemText( vGenePlot );
		textLabel->setFont( labelFont );

		textLabel->setPositionAlignment( Qt::AlignBottom|Qt::AlignHCenter );
		textLabel->position->setType( QCPItemPosition::ptPlotCoords );

		textLabel->position->setCoords( ii, vgeneVector[ii].second ); // place position at center/top of axis rect
		textLabel->setText( QString::number( vgeneVector[ii].second ));
	}

	vGenePlot->rescaleAxes( false );
	vGenePlot->yAxis->setRangeUpper( vgeneVector[0].second*1.2 );
	vGenePlot->xAxis->setRangeLower( -1 );
	vGenePlot->xAxis->setRangeUpper( N );
	vGenePlot->replot();
}


void GeneTab::updateJGene() {
	if ( records_==nullptr ) return;

	// reinitJGenePlot();
	
	// Get J gene values
	map<string,int> jgeneMap = records_->jgene_counts();
	// If there are no valid jgenes, return
	if ( jgeneMap.empty() ) return;

	// Only show top N genes on the plot
	size_t N = 10;

	// Get a vector with v gene values sorted by values descending
	vector<pair<string,int>> jgeneVector = errorx::util::sort_map( jgeneMap, 0 );

	// If N is less than the length of the V gene vector, adjust accordingly
	N = ( N < jgeneVector.size() ) ? N : jgeneVector.size();

	QVector<double>  geneCounts;
	QVector<QString> geneNames;
	QVector<double>  geneIndices;

	for ( size_t ii = 0; ii < N; ++ii ) {
		geneNames   << QString::fromStdString( jgeneVector[ ii ].first );
		geneCounts  << jgeneVector[ ii ].second;
		geneIndices << ii;
	}

	// make bars object
	jBars = new QCPBars(
				jGenePlot->xAxis,
				jGenePlot->yAxis
				);
	// set my custom color
	QColor color( 25, 89, 240, 130 );
	jBars->setPen( QPen(color ));
	jBars->setBrush( color );

	// helps the edges of the bars look crisper
	jBars->setAntialiased(false);

	// set up the axes
	jTicker = QSharedPointer<QCPAxisTickerText>( new QCPAxisTickerText );
	jTicker->addTicks( geneIndices, geneNames );
	jGenePlot->xAxis->setTicker( jTicker );
	jGenePlot->xAxis->setTickLabelRotation( 90 );
	jGenePlot->yAxis->setLabel( "Counts" );

	// add data to plot
	jBars->setData( geneIndices, geneCounts );

	// add title
	QFont font;
	font.setBold( true );
	jGenePlot->plotLayout()->insertRow( 0 );
	jTitle = new QCPTextElement( jGenePlot, "J gene summary", font );
	jGenePlot->plotLayout()->addElement( 0, 0, jTitle );

	// Label the tops of the bars
	// TODO clean up these references
	QFont labelFont;
	labelFont.setPointSize( 10 );
	for ( size_t ii = 0; ii < N; ++ii ) {
		QCPItemText* textLabel = new QCPItemText( jGenePlot );
		textLabel->setFont( labelFont );

		textLabel->setPositionAlignment( Qt::AlignBottom|Qt::AlignHCenter );
		textLabel->position->setType( QCPItemPosition::ptPlotCoords );

		textLabel->position->setCoords( ii, jgeneVector[ii].second ); // place position at center/top of axis rect
		textLabel->setText( QString::number( jgeneVector[ii].second ));
	}

	jGenePlot->rescaleAxes( false );
	jGenePlot->yAxis->setRangeUpper( jgeneVector[0].second*1.2 );
	jGenePlot->xAxis->setRangeLower( -1 );
	jGenePlot->xAxis->setRangeUpper( N );
	jGenePlot->replot();
}

void GeneTab::exportPDF( QString const & outputDir ) {
	vGenePlot->savePdf( outputDir + QDir::separator() + "vgene_summary.pdf", 640, 480 );
	jGenePlot->savePdf( outputDir + QDir::separator() + "jgene_summary.pdf", 640, 480 );
}

void GeneTab::records( errorx::SequenceRecordsSP const records ) { records_ = records; }
errorx::SequenceRecordsSP GeneTab::records() { return records_; }