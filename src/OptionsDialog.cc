#include "OptionsDialog.hh"
 
// Qt include
#include <QSettings>
#include <QString>
#include <QChar>
#include <Qt>
#include <QPushButton>
#include <QGridLayout>
#include <QMenuBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QAction>
#include <QMimeData>
#include <QUrl>
#include <QMessageBox>
#include <QComboBox>
#include <QProgressDialog>
#include <QSlider>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QLineEdit>

// Std include
#include <thread>
#include <iostream>

// ErrorX include
#include "constants.hh"
 
OptionsDialog::OptionsDialog( QSettings* settings, QDialog* parent  ) :
	QDialog( parent ),
	settings_( settings )
{

	setWindowTitle( "ErrorX preferences" );
	setWindowModality( Qt::ApplicationModal );
	
	mainLayout = new QVBoxLayout();
	mainLayout->setSizeConstraint( QLayout::SetFixedSize ); // Set non-resizable
	setLayout( mainLayout );

	initErrorThreshold(); // Build UI for error threshold
	
	lowerParent = new QWidget();
	lowerLayout = new QGridLayout();
	lowerParent->setLayout( lowerLayout );

	initThreads(); // Build UI for # threads
	initChars();   // Build UI for correction char
	initNonProd(); // Build UI for allow nonproductive

	mainLayout->addWidget( lowerParent );

	initButtonBox(); // Build UI for OK/cancel button box
}
 
OptionsDialog::~OptionsDialog() {}
 
void OptionsDialog::initErrorThreshold() {
	using namespace errorx::constants;

	label1 = new QLabel( "How conservative to be when predicting errors?");
	label1->setAlignment( Qt::AlignCenter | Qt::AlignVCenter );
	layout()->addWidget( label1 );

	slider = new QSlider( Qt::Horizontal );
	slider->setRange( 0, 4 );
	slider->setSingleStep( 1 );
	slider->setTickPosition( QSlider::TicksBelow );
	slider->setTickInterval( 1 );

	// Set value based on settings file
	double threshold = settings_->value( "error_threshold" ).toDouble();
	for ( size_t ii = 0; ii < OPTIMIZED_THRESHOLD_VECTOR.size(); ++ii ) {
		if ( threshold <= OPTIMIZED_THRESHOLD_VECTOR[ ii ] ) {
			slider->setValue( ii );
			break;
		}
	}

	label0 = new QLabel( "More\nconservative" );
	label0->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
	label2 = new QLabel( "Default" );
	label2->setAlignment( Qt::AlignCenter | Qt::AlignVCenter );
	label3 = new QLabel( "More\naggressive" );
	label3->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

	sliderLayout = new QGridLayout();
	sliderLayout->addWidget( slider, 0, 0, 1, 5 );
	sliderLayout->addWidget( label0, 1, 0 );
	sliderLayout->addWidget( label2, 1, 2 );
	sliderLayout->addWidget( label3, 1, 4 );
	sliderParent = new QWidget();
	sliderParent->setLayout( sliderLayout );

	layout()->addWidget( sliderParent );
}

void OptionsDialog::initThreads() {
	label4 = new QLabel( "Number of threads to use" );
	label4->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

	spinBox = new QSpinBox();
	spinBox->setMinimum( 1 );
	spinBox->setMaximum( std::thread::hardware_concurrency() );
	spinBox->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

	// Set value based on settings file
	int nthreads = settings_->value( "n_threads" ).toInt();
	if ( nthreads == -1 ) nthreads = std::thread::hardware_concurrency();
	spinBox->setValue( nthreads );
	
	lowerLayout->addWidget( label4, 0, 0, Qt::AlignLeft | Qt::AlignVCenter );
	lowerLayout->addWidget( spinBox, 0, 1, Qt::AlignRight | Qt::AlignVCenter );
}

void OptionsDialog::initChars() {
	label5 = new QLabel( "Character to use for errors" );
	label5->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
	charLine = new QLineEdit();
	charLine->setMaxLength( 1 );
	charLine->setFixedWidth( 20 );
	charLine->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

	// Set value based on settings file
	charLine->setText( settings_->value( "correction" ).toChar() );

	lowerLayout->addWidget( label5, 1, 0, Qt::AlignLeft | Qt::AlignVCenter );
	lowerLayout->addWidget( charLine, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
}

void OptionsDialog::initNonProd() {
	label6 = new QLabel( "Allow non-productive sequences?" );
	label6->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
	comboBox = new QComboBox();
	comboBox->addItem( "No" );
	comboBox->addItem( "Yes" );
	
	// Fit combo box widgth so Yes isn't cut off
	comboBox->setMinimumContentsLength( 3 );
	
	// Set value based on settings file
	bool nonprod = settings_->value( "allow_nonproductive" ).toBool();
	comboBox->setCurrentIndex( nonprod ); // Set to Yes if nonprod==1, else No

	lowerLayout->addWidget( label6, 2, 0, Qt::AlignLeft | Qt::AlignVCenter );
	lowerLayout->addWidget( comboBox, 2, 1, Qt::AlignLeft | Qt::AlignVCenter );
}

void OptionsDialog::initButtonBox() {
	// Add buttons that accept or reject
	buttonBox = new QDialogButtonBox( QDialogButtonBox::Cancel | QDialogButtonBox::Ok );
	restoreDefaultsButton = buttonBox->addButton( QDialogButtonBox::RestoreDefaults );

	connect( buttonBox, &QDialogButtonBox::accepted, this, &OptionsDialog::acceptDialog );
	connect( buttonBox, &QDialogButtonBox::rejected, this, &OptionsDialog::rejectDialog );
	connect( restoreDefaultsButton, &QPushButton::released, this, &OptionsDialog::restoreDefaults );

	layout()->addWidget( buttonBox );
 
}

void OptionsDialog::acceptDialog() {
	using namespace errorx::constants;
	
 	settings_->setValue( "n_threads", spinBox->value() );
	settings_->setValue( "correction", charLine->text().at(0) );
	settings_->setValue( "allow_nonproductive", (bool)comboBox->currentIndex() );
 
 	int errorThresholdIndex = slider->value();
	// TODO optimize thresholds
	double threshold = OPTIMIZED_THRESHOLD_VECTOR[ errorThresholdIndex ];
	settings_->setValue( "error_threshold", threshold );
 
	// Save settings to disk
	settings_->sync();

	accept();
}
 
void OptionsDialog::rejectDialog() {
	reject();
}

void OptionsDialog::restoreDefaults() {
	slider->setValue( 2 );
	spinBox->setValue( std::thread::hardware_concurrency() );
	charLine->setText( "N" );
	comboBox->setCurrentIndex( 1 );
}