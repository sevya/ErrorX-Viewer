#include "SummaryTab.hh"

#include <Qt>

#include <QPushButton>
#include <QStackedLayout>
#include <QGridLayout>
#include <QDir>

#include <QTabWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

#include "qcustomplot.hh"

#include <iostream>

#include "SequenceRecords.hh"
#include "SequenceRecord.hh"

SummaryTab::SummaryTab( QWidget* parent ) :
	QWidget( parent ),
	records_( nullptr )
{
	init();
}

SummaryTab::~SummaryTab() {
	delete plot;
	delete plotTitle;
	delete plotBars;
}

void SummaryTab::init() {

	leftLayout = new QGridLayout( this );
	fileLabel = new QLabel( "Filename", this );
	leftLayout->addWidget( fileLabel, 0, 0 );
	line1 = new QLineEdit( "", this );
	line1->setReadOnly( true );
	line1->setAlignment( Qt::AlignRight );
	leftLayout->addWidget( line1, 0, 1 );

	numLabel = new QLabel( "Total # sequences", this );
	leftLayout->addWidget( numLabel, 1, 0 );
	line2 = new QLineEdit( "", this );
	line2->setReadOnly( true );
	line2->setAlignment( Qt::AlignRight );
	leftLayout->addWidget( line2, 1, 1 );

	numLabel2 = new QLabel( "# sequences parsed", this ); 
	leftLayout->addWidget( numLabel2, 2, 0 );
	line3 = new QLineEdit( "", this );
	line3->setReadOnly( true );
	line3->setAlignment( Qt::AlignRight );
	leftLayout->addWidget( line3, 2, 1 );

	numLabel3 = new QLabel( "# productive sequences", this );
	leftLayout->addWidget( numLabel3, 3, 0 );
	line4 = new QLineEdit( "", this );
	line4->setReadOnly( true );
	line4->setAlignment( Qt::AlignRight );
	leftLayout->addWidget( line4, 3, 1 );

	uniqueLabel = new QLabel( "# unique sequences", this );
	leftLayout->addWidget( uniqueLabel, 4, 0 );
	line5 = new QLineEdit( "", this );
	line5->setReadOnly( true );
	line5->setAlignment( Qt::AlignRight );
	leftLayout->addWidget( line5, 4, 1 );

	filler = new QWidget( this );
	leftLayout->addWidget( filler, 5, 0, 1, 2 );
	leftLayout->setRowStretch( 5, 0 );

	leftPanel = new QWidget( this );
	leftPanel->setLayout( leftLayout);

	initPlot();

	mainLayout = new QHBoxLayout( this );
	setLayout( mainLayout );

	mainLayout->addWidget( leftPanel );
	mainLayout->addWidget( plot );
}

void SummaryTab::initPlot() {
	// Inititalize plot
	plot = new QCustomPlot();
	QSizePolicy policy = plot->sizePolicy();
	policy.setHorizontalStretch( 1 );
	plot->setSizePolicy( policy );

	// add title
	QFont font;
	font.setBold( true );
	plot->plotLayout()->insertRow( 0 );
	plotTitle = new QCPTextElement( plot, "Results Summary", font );
	plot->plotLayout()->addElement( 0, 0, plotTitle );
}

void SummaryTab::reinitPlot() {
	layout()->removeWidget( plot );
	delete plot;
	initPlot();
	layout()->addWidget( plot );
}

void SummaryTab::update() {

	if ( records_==nullptr ) return;	
	reinitPlot();

	// Set these values in the line fields
	int no_sequences         = records_->size();
	int parsed_sequences     = records_->good_records();
	int productive_sequences = records_->productive_records();
	int unique_sequences     = records_->unique_nt_sequences( /*bool corrected=*/0 );

	errorx::ErrorXOptionsPtr options = records_->get_options();

	line1->setText( QString::fromStdString( options->infile() ));
	line2->setText( QString::number( no_sequences ));
	line3->setText( QString::number( parsed_sequences ));
	line5->setText( QString::number( unique_sequences ));		

	// TSV input does not have this fields
	if ( records_->get_options()->format() == "tsv" ) {
		line4->setText( "N/A" );
	} else {
		line4->setText( QString::number( productive_sequences ));
	}

	QVector<double>  counts;
	QVector<QString> labels;
	QVector<double>  indices;

	counts << no_sequences << parsed_sequences << productive_sequences << unique_sequences;
	labels << "Total" << "Parsed" << "Productive" << "Unique";
	indices << 1 << 2 << 3 << 4;

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
	plotBars->setAntialiased(false);

	// set up the axes
	QSharedPointer<QCPAxisTickerText> textTicker( new QCPAxisTickerText );
	textTicker->addTicks( indices, labels );
	plot->xAxis->setTicker( textTicker );
	// plot->xAxis->setTickLabelRotation( 90 );
	plot->xAxis->setLabel( "Sequences" );
	plot->yAxis->setLabel( "# Sequences" );

	// add data to plot
	plotBars->setData( indices, counts );


	// Label the tops of the bars
	// TODO clean up these references
	QFont labelFont;
	labelFont.setPointSize( 10 );
	QCPItemText* textLabel = new QCPItemText( plot );
	textLabel->setFont( labelFont );
	textLabel->setPositionAlignment( Qt::AlignBottom|Qt::AlignHCenter );
	textLabel->position->setType( QCPItemPosition::ptPlotCoords );
	textLabel->position->setCoords( 1, no_sequences ); // place position at center/top of axis rect
	textLabel->setText( QString::number( no_sequences ));


	QCPItemText* textLabel2 = new QCPItemText( plot );
	textLabel2->setFont( labelFont );
	textLabel2->setPositionAlignment( Qt::AlignBottom|Qt::AlignHCenter );
	textLabel2->position->setType( QCPItemPosition::ptPlotCoords );
	textLabel2->position->setCoords( 2, parsed_sequences ); // place position at center/top of axis rect
	textLabel2->setText( QString::number( parsed_sequences ));


	QCPItemText* textLabel3 = new QCPItemText( plot );
	textLabel3->setFont( labelFont );
	textLabel3->setPositionAlignment( Qt::AlignBottom|Qt::AlignHCenter );
	textLabel3->position->setType( QCPItemPosition::ptPlotCoords );
	textLabel3->position->setCoords( 3, productive_sequences ); // place position at center/top of axis rect
	textLabel3->setText( QString::number( productive_sequences ));


	QCPItemText* textLabel4 = new QCPItemText( plot );
	textLabel4->setFont( labelFont );
	textLabel4->setPositionAlignment( Qt::AlignBottom|Qt::AlignHCenter );
	textLabel4->position->setType( QCPItemPosition::ptPlotCoords );
	textLabel4->position->setCoords( 4, unique_sequences ); // place position at center/top of axis rect
	textLabel4->setText( QString::number( unique_sequences ));


	plot->rescaleAxes();
	// There are four columns so set the range 0-5
	plot->xAxis->setRangeLower( 0 );
	plot->xAxis->setRangeUpper( 5 );
	plot->yAxis->setRangeUpper( no_sequences*1.2 );
	plot->replot();
}

void SummaryTab::exportPDF( QString const & outputDir ) {
	plot->savePdf( outputDir + QDir::separator() + "summary.pdf" );
}

void SummaryTab::records( errorx::SequenceRecordsSP const records ) { records_ = records; }
errorx::SequenceRecordsSP SummaryTab::records() { return records_; }