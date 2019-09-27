#include "ConfirmFile.hh"

// ErrorX includes 
#include "ErrorXOptions.hh"
#include "util.hh"

// Qt includes
#include <QFile>
#include <QString>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFont>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>

ConfirmFile::ConfirmFile( errorx::ErrorXOptionsSP options, QWidget* parent ) :
	QDialog( parent ), 
	options_( options )
{

	// Set up UI
	mainLayout = new QVBoxLayout( this );
	mainLayout->setSizeConstraint( QLayout::SetFixedSize );
	setLayout( mainLayout );

	// Add input file
	buildInputFileSection();

	// Build section to select the file type
	buildFileTypeSection();

	// Build section to select the species
	buildSpeciesSection();

	// Build section to select the receptor type
	buildReceptorTypeSection();

	// Add buttons that accept or reject
	buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this );
	connect( buttonBox, &QDialogButtonBox::accepted, this, &ConfirmFile::acceptDialog );
	connect( buttonBox, &QDialogButtonBox::rejected, this, &ConfirmFile::rejectDialog );

	mainLayout->addWidget( buttonBox );
	mainLayout->addStretch();
}
 
ConfirmFile::~ConfirmFile() {}

void ConfirmFile::buildInputFileSection() {
	inputWidget = new QWidget( this );
	inputLayout = new QGridLayout(); // this );
	inputWidget->setLayout( inputLayout );

	inputLabel = new QLabel( "Input file", this );
	inputLayout->addWidget( inputLabel, 0, 0 );
	fileEdit = new QLineEdit( "", this );
	fileEdit->setReadOnly( true );
	fileEdit->setMinimumWidth( 200 ); //default is 100
	inputLayout->addWidget( fileEdit, 1, 0 );

	selectFile = new QPushButton( "Choose file", this );
	QSizePolicy pol = selectFile->sizePolicy();
	pol.setHorizontalPolicy( QSizePolicy::Fixed );
	selectFile->setSizePolicy( pol );
	inputLayout->addWidget( selectFile, 2, 0, Qt::AlignRight );
	// inputLayout->addWidget( selectFile, 1, 1 );
	
	connect( selectFile, &QPushButton::released, this, &ConfirmFile::chooseFile );

	layout()->addWidget( inputWidget );
}

void ConfirmFile::buildFileTypeSection() {
	// Build section to select the file type
	extGroup = new QGroupBox( this );
	radioButtonFASTQ = new QRadioButton( "FASTQ" );
	radioButtonFASTA = new QRadioButton( "FASTA" );
	radioButtonTSV = new QRadioButton( "TSV" );
	// extGroup->setTitle( "File type?" );

	radioButtonFASTQ->setChecked( true );

	extLayout = new QHBoxLayout(); // this );
	extLayout->addWidget( radioButtonFASTQ );
	extLayout->addWidget( radioButtonFASTA );
	extLayout->addWidget( radioButtonTSV );
	extLayout->addStretch( 1 );
	extGroup->setLayout( extLayout );

	label1 = new QLabel( this );
	label1->setText( "File type?" );
	label1->setAlignment( Qt::AlignLeft );

	layout()->addWidget( label1 );
	layout()->addWidget( extGroup );
} 

void ConfirmFile::buildReceptorTypeSection() {
	igtypeGroup = new QGroupBox( this );
	radioButtonBCR = new QRadioButton( "B cell" );
	radioButtonTCR = new QRadioButton( "T cell" );
	radioButtonBCR->setChecked( true );

	igtypeLayout = new QHBoxLayout(); // this );
	igtypeLayout->addWidget( radioButtonBCR );
	igtypeLayout->addWidget( radioButtonTCR );
	igtypeLayout->addStretch( 1 );
	igtypeGroup->setLayout( igtypeLayout );

	label3 = new QLabel( this );
	label3->setText( "Receptor type?" );
	label3->setAlignment( Qt::AlignLeft );

	layout()->addWidget( label3 );
	layout()->addWidget( igtypeGroup );
}

void ConfirmFile::buildSpeciesSection() {
	speciesGroup = new QGroupBox( this );
	radioButtonHuman = new QRadioButton( "Human" );
	radioButtonMouse = new QRadioButton( "Mouse" );
	radioButtonHuman->setChecked( true );

	speciesLayout = new QHBoxLayout(); // this );
	speciesLayout->addWidget( radioButtonHuman );
	speciesLayout->addWidget( radioButtonMouse );
	speciesLayout->addStretch( 1 );
	speciesGroup->setLayout( speciesLayout );

	label2 = new QLabel( this );
	label2->setText( "Species?" );
	label2->setAlignment( Qt::AlignLeft );

	layout()->addWidget( label2 );
	layout()->addWidget( speciesGroup );
}

void ConfirmFile::acceptDialog() {
	if ( inputFile.isNull() || inputFile=="tmp" ) {
		QMessageBox box( QMessageBox::Warning,
			 "Error",
			 "Please provide an input file",
			 QMessageBox::Ok );
		box.exec();
		return;
	}

	options_->infile( inputFile.toStdString() );
	if ( radioButtonFASTQ->isChecked() ) options_->format( "fastq" );
	else if ( radioButtonFASTA->isChecked() ) options_->format( "fasta" );
	else if ( radioButtonTSV->isChecked() ) options_->format( "tsv" );

	options_->species( radioButtonHuman->isChecked() ? "human" : "mouse" );
	options_->igtype( radioButtonBCR->isChecked() ? "Ig" : "TCR" );
	accept();
}
 
void ConfirmFile::rejectDialog() {
	reject();
}

void ConfirmFile::chooseFile() {
	inputFile = QFileDialog::getOpenFileName(
		this,
		tr("Choose input file"),
		"", //directory
		"FASTQ, FASTA, or TSV files (*.fastq *.fq *.fasta *.fa *.tsv)" // filters
	);
 
	if ( inputFile.isNull() ) return;

	fileEdit->setText( inputFile );
	autofillExtension( inputFile );
}

void ConfirmFile::setFile( QString const & inFile ) {
	inputFile = inFile;
	QString text = ( inputFile=="tmp" ) ? "" : inputFile;
	fileEdit->setText( text );
	autofillExtension( inputFile );
}

void ConfirmFile::autofillExtension( QString const & infile ) {
	QStringList tokens = infile.split( ".", QString::SkipEmptyParts );
	QString ext = tokens.at( tokens.size()-1 );

	if ( ext=="tsv" ) {
		radioButtonTSV->setChecked( true );
	} else if ( ext=="fastq" || ext=="fq" ) {
		radioButtonFASTQ->setChecked( true );
	} else if ( ext=="fasta" || ext=="fa" ) {
		radioButtonFASTA->setChecked( true );
	}
}