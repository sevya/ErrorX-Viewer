
#include <Qt>
#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QCoreApplication>
#include <QSizePolicy>
#include <QCloseEvent>

#include "ProgressDialog.hh"

#include <iostream>

ProgressDialog::ProgressDialog( QWidget* parent ) :
	QWidget( parent ),
	value_( 0 ),
	maximum_( 1 )
{
	setWindowModality( Qt::ApplicationModal );
	setWindowFlags( Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint );

	mainLayout = new QVBoxLayout( this );
	setLayout( mainLayout );

	topLabel = new QLabel( "Running ErrorX...", this );
	topLabel->setAlignment( Qt::AlignCenter );
	mainLayout->addWidget( topLabel );

	bar = new QProgressBar( this );
	bar->setMinimum( 0 );
	bar->setMaximum( maximum_ );
	bar->setValue( value_ );
	mainLayout->addWidget( bar );

	bottomLabel = new QLabel( "0 out of N samples processed", this );
	bottomLabel->setAlignment( Qt::AlignCenter );
	mainLayout->addWidget( bottomLabel );

	// Make cancel button
	cancelButton = new QPushButton( "Cancel", this );
	QSizePolicy pol = cancelButton->sizePolicy();
	pol.setHorizontalPolicy( QSizePolicy::Fixed );
	cancelButton->setSizePolicy( pol );
	// TODO make the cancel function work so I can enable this
	cancelButton->setEnabled( false );
	mainLayout->addWidget( cancelButton );
}

ProgressDialog::~ProgressDialog() {}

void ProgressDialog::updateLowerText() {
	bottomLabel->setText( 
		QString::number( value_ ) + 
		" out of " +
		QString::number( maximum_ ) +
		" processed" 
		);
}

QPushButton* ProgressDialog::getCancelButton() { return cancelButton; }
QProgressBar* ProgressDialog::getBar() { return bar; }
QLabel* ProgressDialog::getTopLabel() { return topLabel; }
QLabel* ProgressDialog::getBottomLabel() { return bottomLabel; }

int ProgressDialog::getValue() { return value_; }
void ProgressDialog::setValue( int a ) { value_ = a; }
int ProgressDialog::getMaximum() { return maximum_; }
void ProgressDialog::setMaximum( int a ) { maximum_ = a; }

