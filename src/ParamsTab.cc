#include "ParamsTab.hh"

#include <Qt>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QSize>

#include "SequenceRecords.hh"
#include "ErrorXOptions.hh"

#include <iostream>

ParamsTab::ParamsTab( QWidget* parent ) :
	QWidget( parent ),
	records_( nullptr )
{
	init();
}

ParamsTab::~ParamsTab() {}

void ParamsTab::init() {
	mainLayout = new QGridLayout( this );
	setLayout( mainLayout );

	label1 = new QLabel( "<b>Processing parameters</b>", this );
	label1->setAlignment( Qt::AlignCenter );
	label2 = new QLabel( "Format", this );
	label3 = new QLabel( "Species", this );
	label4 = new QLabel( "Receptor type", this );
	label5 = new QLabel( "Number of threads used", this );
	label6 = new QLabel( "Error threshold", this );
	label7 = new QLabel( "Correction character", this );
	label8 = new QLabel( "Allow nonproductive sequences", this );

	filler = new QWidget( this );
	mainLayout->addWidget( filler, 0, 0, 12, 1 );
	mainLayout->setColumnStretch( 0, 0 );

	mainLayout->addWidget( label1, 0, 1, 1, 2 );
	mainLayout->addWidget( label2, 1, 1 );
	mainLayout->addWidget( label3, 2, 1 );
	mainLayout->addWidget( label4, 3, 1 );
	mainLayout->addWidget( label5, 4, 1 );
	mainLayout->addWidget( label6, 5, 1 );
	mainLayout->addWidget( label7, 6, 1 );
	mainLayout->addWidget( label8, 7, 1 );

	formatLabel = new QLabel( "", this );
	formatLabel->setAlignment( Qt::AlignRight );
	speciesLabel = new QLabel( "", this );
	speciesLabel->setAlignment( Qt::AlignRight );
	receptorLabel = new QLabel( "", this );
	receptorLabel->setAlignment( Qt::AlignRight );
	threadLabel = new QLabel( "", this );
	threadLabel->setAlignment( Qt::AlignRight );
	errorLabel = new QLabel( "", this );
	errorLabel->setAlignment( Qt::AlignRight );
	correctionLabel = new QLabel( "", this );
	correctionLabel->setAlignment( Qt::AlignRight );
	nonprodLabel = new QLabel( "", this );
	nonprodLabel->setAlignment( Qt::AlignRight );

	mainLayout->addWidget( formatLabel, 1, 2 );
	mainLayout->addWidget( speciesLabel, 2, 2 );
	mainLayout->addWidget( receptorLabel, 3, 2 );
	mainLayout->addWidget( threadLabel, 4, 2 );
	mainLayout->addWidget( errorLabel, 5, 2 );
	mainLayout->addWidget( correctionLabel, 6, 2 );
	mainLayout->addWidget( nonprodLabel, 7, 2 );

	empty = new QWidget( this );
	empty->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

	mainLayout->addWidget( empty, 8, 0, 1, 4 );
	mainLayout->setRowStretch( 8, 0 );

	logo = new QLabel( this );
	logo->setAlignment( Qt::AlignCenter );
	QPixmap pix( ":/images/logo_horizontal_transparent.png" );
	pix = pix.scaled( QSize(250,250),  Qt::KeepAspectRatio );
	logo->setPixmap(pix);

	mainLayout->addWidget( logo, 9, 1, 1, 2 );

	finePrint = new QLabel( "Copyright (C) Alex Sevy, 2019\nalex@endeavorbio.com", this );
	finePrint->setAlignment( Qt::AlignCenter );
	mainLayout->addWidget( finePrint, 10, 1, 1, 2 );

	filler2 = new QWidget( this );
	mainLayout->addWidget( filler2, 0, 3, 12, 1 );
	mainLayout->setColumnStretch( 3, 0 );
}

void ParamsTab::update() {
	if ( records_==nullptr ) return;

	errorx::ErrorXOptionsPtr options = records_->get_options();

	formatLabel->setText( QString::fromStdString(options->format()) );
	speciesLabel->setText( QString::fromStdString(options->species()) );
	receptorLabel->setText( QString::fromStdString(options->igtype()) );
	threadLabel->setText( QString::number(options->nthreads()) );
	errorLabel->setText( QString::number(options->error_threshold(), 'f', 2) );
	correctionLabel->setText( QString(options->correction()) );
	nonprodLabel->setText( options->allow_nonproductive() ? 
		QString("True") :
		QString("False")
		);
}

void ParamsTab::records( errorx::SequenceRecordsSP const records ) { records_ = records; }
errorx::SequenceRecordsSP ParamsTab::records() { return records_; }