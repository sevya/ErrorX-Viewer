// My headers
#include "MainWindow.hh"
#include "ProgressDialog.hh"
#include "ConfirmFile.hh"

#include "WelcomeTab.hh"
#include "SummaryTab.hh"
#include "GeneTab.hh"
#include "ClonotypeTab.hh"
#include "ErrorTab.hh"
#include "DataTab.hh"
#include "ParamsTab.hh"
#include "OptionsDialog.hh"
#include "ErrorXWorker.hh"
#include "gui_util.hh"

// ErrorX headers
#include "errorx.hh"
#include "ErrorXOptions.hh"
#include "SequenceRecords.hh"
#include "SequenceRecord.hh"
#include "constants.hh"
#include "exceptions.hh"

// Qt headers
#include <Qt>
#include <QCoreApplication>
#include <QPushButton>
#include <QGridLayout>
#include <QMenuBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QAction>
#include <QIcon>
#include <QMimeData>
#include <QUrl>
#include <QMessageBox>
#include <QInputDialog>
#include <QProgressDialog>
#include <QFileInfo>
#include <QSettings>
#include <QToolBar>
#include <QDesktopServices>

// Std headers
#include <iostream>
#include <functional>

MainWindow::MainWindow( QWidget* parent ) : 
	QWidget( parent ),
	records_( nullptr )
{

	resize( 1000, 650 );

	// Create main layout
	mainLayout = new QVBoxLayout( this );
	setLayout( mainLayout );
	setAcceptDrops( true );

	// Initialize menu bar
	initMenuBar();	
	// Initialize tool bar
	initToolBar();
	// Initialize tab widget
	initTabs();
	// Set up settings file
 	initSettings();
 	// Set up callbacks for progress bar
 	initProgressCallbacks();
}

// Initialize menu bar
void MainWindow::initMenuBar() {

	menuBar = new QMenuBar( this );
	
	// Set up file menu to go in menu bar
	fileMenu = new QMenu( "File", this );
	// Set up actions I want
	newProjectAction   = new QAction( "New Project", this );
	openProjectAction  = new QAction( "Open Project", this );
	saveAction		   = new QAction( "Save Project", this );
	exportPDFAction	   = new QAction( "Export Project Files", this );
	prefsAction		   = new QAction( "Preferences", this );

	saveAction->setEnabled( false );
	exportPDFAction->setEnabled( false );


	fileMenu->addAction( newProjectAction );
	fileMenu->addAction( openProjectAction );
	fileMenu->addAction( saveAction );
	fileMenu->addAction( exportPDFAction );
	fileMenu->addAction( prefsAction );

	// Connect signals to action handlers
	connect( newProjectAction, &QAction::triggered,
			 this, &MainWindow::newProject );
	connect( openProjectAction, &QAction::triggered,
			 this, &MainWindow::importProject );
	connect( saveAction, &QAction::triggered,
			 this, &MainWindow::exportProject );
	connect( exportPDFAction, &QAction::triggered,
			 this, &MainWindow::exportProjectToPDF );
	connect( prefsAction,&QAction::triggered,
		 	 this, &MainWindow::fileMenuPrefs );

	menuBar->addMenu( fileMenu );


	// Set up help menu
	helpMenu = new QMenu( "Help", this );
	// Set up actions I want
	documentationAction  = new QAction( "Documentation", this );
	contactAction		= new QAction( "Contact us", this );
	licenseAction		= new QAction( "Input license", this );

	helpMenu->addAction( documentationAction );
	helpMenu->addAction( contactAction );
	helpMenu->addSeparator();
	helpMenu->addAction( licenseAction );

	// Connect signals to action handlers
	connect( documentationAction, &QAction::triggered,
			 this, &MainWindow::showDocumentation );
	connect( contactAction, &QAction::triggered,
			 this, &MainWindow::showContact );
	connect( licenseAction, &QAction::triggered,
			 this, &MainWindow::inputLicense );

	menuBar->addMenu( helpMenu );

	layout()->addWidget( menuBar );
}

// Initialize tool bar
void MainWindow::initToolBar() {

	toolBar = new QToolBar( this );
	toolBar->setMovable( false );
	toolBar->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
	
	newProjectButton = new QAction( QIcon(":/images/new.png"), "New Project", this );
	toolBar->addAction( newProjectButton );

	importProjectButton = new QAction( QIcon(":/images/import.png"), "Import Project", this );
	toolBar->addAction( importProjectButton );

	saveProjectButton = new QAction( QIcon(":/images/save.png"), "Save Project", this );
	saveProjectButton->setEnabled( false );
	toolBar->addAction( saveProjectButton );

	exportProjectButton = new QAction( QIcon(":/images/download_pdf.png"), "Export Project Files", this );
	exportProjectButton->setEnabled( false );
	toolBar->addAction( exportProjectButton );

	layout()->addWidget( toolBar );

	// Connect signals to action handlers
	connect( newProjectButton, &QAction::triggered,
			 this, &MainWindow::newProject );

	connect( saveProjectButton, &QAction::triggered,
		 	 this, &MainWindow::exportProject );

	connect( importProjectButton, &QAction::triggered,
			 this, &MainWindow::importProject );

	connect( exportProjectButton, &QAction::triggered,
		 	 this, &MainWindow::exportProjectToPDF );
}

// Initialize top-level tab widget	
void MainWindow::initTabs() {
	tabWidget = new QTabWidget( this );

	// Make all the individual pages that will be the tabs
	welcomeTab_ = new WelcomeTab( this );
	tabWidget->addTab( welcomeTab_, "Welcome" );

	summaryTab_ = new SummaryTab( this );
	tabWidget->addTab( summaryTab_, "Summary" );

	geneTab_ = new GeneTab( this );
	tabWidget->addTab( geneTab_, "V-J gene usage" );

	clonotypeTab_ = new ClonotypeTab( this );
	tabWidget->addTab( clonotypeTab_, "Clonotypes" );

	errorTab_ = new ErrorTab( this );
	tabWidget->addTab( errorTab_, "Error profile" );

	dataTab_ = new DataTab( this );
	tabWidget->addTab( dataTab_, "Sequences" );

	paramsTab_ = new ParamsTab( this );
	tabWidget->addTab( paramsTab_, "Parameters" );


	// Add tab widget to main window
	layout()->addWidget( tabWidget );
}

// Initialize settings file if not already existing
void MainWindow::initSettings() {
	settings_ = new QSettings( "EndeavorBio", "ErrorX" );

	if ( !settings_->contains( "n_threads" )) {
		settings_->setValue( "n_threads" , -1 );
	}
	if ( !settings_->contains( "error_threshold" )) {
		settings_->setValue( "error_threshold", 
			errorx::constants::OPTIMIZED_THRESHOLD );
	}
	if ( !settings_->contains( "correction" )){
		settings_->setValue( "correction" ,'N' );
	}
	if ( !settings_->contains( "allow_nonproductive" )) {
		settings_->setValue( "allow_nonproductive", 1 );
	}

	settings_->sync();

	options_ = gui_util::optionsFromSettings( settings_ );
}

void MainWindow::initProgressCallbacks() {
	connect( this, &MainWindow::incrementProgressSignal, 
			 this, &MainWindow::incrementProgressSlot );
	connect( this, &MainWindow::resetProgressSignal, 
			 this, &MainWindow::resetProgressSlot );
	connect( this, &MainWindow::finishProgressSignal, 
			 this, &MainWindow::finishProgressSlot );
	connect( this, &MainWindow::setProgressMessageSignal, 
			 this, &MainWindow::setProgressMessageSlot );
}

void MainWindow::fileMenuPrefs() {
	OptionsDialog options( settings_ );
	int returnValue = options.exec();

	// if dialog was accepted, use the settings
	if ( returnValue==1 ) { 
		options_ = gui_util::optionsFromSettings( settings_ );
	}
}

void MainWindow::newProject() {
	go();
}

void MainWindow::importProject() {
	QString inputFile = QFileDialog::getOpenFileName(
		this,
		tr("Choose input file"),
		"", //directory
		"ErrorX TSV file (*.tsv)" // filters
	);
 
	if ( inputFile.isNull() ) return;

	populateFromExisting( inputFile );
}

void MainWindow::exportProject() {
	dataTab_->update( /*fullData = */true );
	dataTab_->selectCheckBox();
	dataTab_->exportTable();
}

void MainWindow::exportProjectToPDF() {
	QString outputDir = QFileDialog::getExistingDirectory( 
		this, 
		tr("Choose directory for project files"),
		"", //directory
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
	if ( outputDir.isNull() ) return;

	summaryTab_->exportPDF( outputDir );
	errorTab_->exportPDF( outputDir );
	geneTab_->exportPDF( outputDir );

	dataTab_->update( /*fullData = */true );
	dataTab_->selectCheckBox();
	dataTab_->exportTable( outputDir + QDir::separator() + "sequences.tsv" );

	clonotypeTab_->exportTable( outputDir + QDir::separator() + "clonotypes.tsv" );

	QMessageBox box( QMessageBox::NoIcon,
		 "Success!",
		 "Files exported succesfully!",
		 QMessageBox::Ok );

	box.exec();
}

bool MainWindow::isValidErrorXTSV( QString const & file ) {
	// validate that first column has correct headers
	vector<std::string> labels = { "SequenceID", "V_gene", "V_identity", "V_Evalue", "D_gene", "D_identity", "D_Evalue", "J_gene", "J_identity", "J_Evalue", "Strand", "Chain", "Productive", "CDR3_NT_sequence", "CDR3_AA_sequence", "Full_NT_sequence", "Full_GL_NT_sequence", "Full_AA_sequence", "Full_NT_sequence_corrected", "Full_AA_sequence_corrected", "N_errors" };

	std::ifstream in( file.toStdString() );
	std::string str;
	bool isValid = true;

	for ( vector<std::string>::const_iterator it = labels.begin();
		it != labels.end(); ++it ) {
		in >> str;
		if ( str != (*it) ) {
			isValid = false;
			break;
		}
	}
	in.close();
	return isValid;
}

void MainWindow::populateFromExisting( QString const & file ) {
	if ( !isValidErrorXTSV( file )) { 
		QMessageBox box( QMessageBox::Warning,
			"Error",
			"Input file is not a valid ErrorX TSV. "
			"Please check file and try again.",
			QMessageBox::Ok );
		box.exec();
	}

	using namespace std;
	using namespace errorx;
	string line;

	vector<SequenceRecordPtr> records_vector;
	vector<string> tokens;
	ifstream instream( file.toStdString() );

	getline( instream, line ); // discard first line

	while ( getline( instream, line ) ) {
		tokens = util::tokenize_string<string>( line, 
			"\t ", // delim
			1 //token_compress 
			);
		
		if ( tokens.size() != 21 ) { // 21 fields in ErrorX tsv
			QMessageBox box( QMessageBox::Warning,
				"Error",
				"Input file is not a valid ErrorX TSV. "
				"Please check file and try again.",
				QMessageBox::Ok );
			box.exec();

			return;
		} 

		records_vector.push_back( SequenceRecordPtr( new SequenceRecord( tokens )));
	}
	instream.close();

	try {
		options_->infile( file.toStdString() );
		records_ = SequenceRecordsSP( new SequenceRecords( records_vector, *options_ ));
	} catch ( exception & e ) {
		QMessageBox box( QMessageBox::Warning,
					 "Error",
					 e.what(),
					 QMessageBox::Ok );
 
		box.exec();
		return;
	}
	runProtocolDone();
}

void MainWindow::dragEnterEvent( QDragEnterEvent* e ) {
	// these lines are magic - don't take them out!
	if ( e->mimeData()->hasUrls() ) {
		e->acceptProposedAction();
	}
}

void MainWindow::dropEvent( QDropEvent* e ) {

	QUrl url = e->mimeData()->urls().at(0);
	QFileInfo fileName( url.toLocalFile() );
 
	if ( fileName.suffix() != "fastq" &&
		 fileName.suffix() != "fq"	&&
		 fileName.suffix() != "tsv" ) {
		QMessageBox box( QMessageBox::Warning,
					 "Error",
					 "Invalid file type: ErrorX only accepts files of type FASTQ or TSV (extensions .fastq, .fq, or .tsv)",
					 QMessageBox::Ok );
 
		box.exec();
		return;
	}
	e->acceptProposedAction();
	go( fileName.absoluteFilePath() );
}

void MainWindow::showDocumentation() {
	QDesktopServices::openUrl( 
		QUrl( "https://endeavorbio.com/2019/02/01/documentation/" )
		);
}

void MainWindow::showContact() {
	 QDesktopServices::openUrl( 
	 	QUrl( "mailto:alex@endeavorbio.com?subject=ErrorX Help" )
	 	);
}

void MainWindow::inputLicense() {
	QString license = QInputDialog::getText(
						this, 
						tr("Input License"),
						tr("Please input license key below"), 
						QLineEdit::Normal
						);

	if ( license.isEmpty() ) return;

	try {
		errorx::util::write_license( license.toStdString() );
		QMessageBox box( QMessageBox::NoIcon,
			 "Success!",
			 "Successfully wrote license file!",
			 QMessageBox::Ok 
			 );
		box.exec();
	} catch ( errorx::BadLicenseException & e ) {

		QMessageBox box( QMessageBox::Warning,
			 "Bad license",
			 e.what(),
			 QMessageBox::Ok 
			 );
		box.exec();
	}
}

void MainWindow::go( QString const & inputFile ) {
	using namespace errorx;
	using namespace std;
	namespace fs = boost::filesystem;
 		
	// ConfirmFile will set options for infile, file format, species, receptor type
	confirm = new ConfirmFile( options_, this );
	confirm->setFile( inputFile );
	int returnValue = confirm->exec();
	if ( returnValue==0 ) return;
	
	progressDialog = new ProgressDialog( this ); 

	std::function<void(int,int)> increment = std::bind(
				&MainWindow::incrementProgress,
				this,
				std::placeholders::_1,
				std::placeholders::_2
				);

	std::function<void(void)> reset = std::bind(
				&MainWindow::resetProgress,
				this
				);
	
	std::function<void(void)> finish = std::bind(
				&MainWindow::finishProgress,
				this
				);

	std::function<void(std::string)> message = std::bind(
				&MainWindow::setProgressMessage,
				this,
				std::placeholders::_1
				);
	

	options_->increment( increment );
	options_->reset( reset );
	options_->finish( finish );
	options_->message( message );

	progressDialog->show();

	runner = new ErrorXWorker( options_, this );

	connect( runner, &ErrorXWorker::finished, 
			 this, &MainWindow::runProtocolDone );

	// Connect cancel button from dialog to QThread::terminate
	// TODO: add control-c catching or cancellation to the protocol itself
	connect( progressDialog->getCancelButton(), &QPushButton::released, 
			 runner, &ErrorXWorker::quitWorker );

	connect( runner, &ErrorXWorker::throwException,
			 this, &MainWindow::exceptionMessage );
	connect( runner, &ErrorXWorker::threadCancelled,
			 this, &MainWindow::resetRecords );

	runner->start();
}

void MainWindow::exceptionMessage( QString const e ) {
	QMessageBox box( QMessageBox::Warning,
					 "Error",
					 e,
					 QMessageBox::Ok );
 
	box.exec();
}

void MainWindow::resetRecords() {
	// this is called when thread is cancelled
	records_ = nullptr;
}

void MainWindow::runProtocolDone() {
	progressDialog->close();
	// delete progressDialog;
	// If the operation is cancelled mid-run records_ will be null
	// in that case exit now
	if ( records_==nullptr ) return;

	records_->count_clonotypes();

	// initialize all tabs
	summaryTab_->records( records_ );
	summaryTab_->update();

	geneTab_->records( records_ );
	geneTab_->update();

	errorTab_->records( records_ );
	errorTab_->update();

	dataTab_->records( records_ );
	dataTab_->update();

	clonotypeTab_->records( records_ );
	clonotypeTab_->update();

	paramsTab_->records( records_ );
	paramsTab_->update();

	// Show the hidden windows that only get activated after run
	activateHiddenWindows();

	emit verifySummaryTab();
}

void MainWindow::activateHiddenWindows() {
	// remove the welcome tab if it's still present
	if ( tabWidget->count() == 7 ) tabWidget->removeTab( 0 );

	saveAction->setEnabled( true );
	saveProjectButton->setEnabled( true );

	exportPDFAction->setEnabled( true );
	exportProjectButton->setEnabled( true );
}


///////////////// Progress bar related fxns //////////////////////////
void MainWindow::incrementProgressSlot( int value, int total ) { 

	QProgressBar* bar = progressDialog->getBar();

	int max = progressDialog->getMaximum();
	if ( max != total ) {
		progressDialog->setMaximum( total );
		bar->setMaximum( total );
	}

	int newValue = progressDialog->getValue() + value;

	progressDialog->setValue( newValue );
	bar->setValue( newValue );

	progressDialog->updateLowerText();
}

void MainWindow::resetProgressSlot() {

	QProgressBar* bar = progressDialog->getBar();
	progressDialog->setValue( 0 );
	bar->setValue( 0 );

	progressDialog->updateLowerText();
}

void MainWindow::finishProgressSlot() {

	QProgressBar* bar = progressDialog->getBar();
	
	progressDialog->setValue( progressDialog->getMaximum() );
	bar->setValue( progressDialog->getMaximum() );

	progressDialog->updateLowerText();
}

void MainWindow::setProgressMessageSlot( QString message ) {
	// Progress is reset when IGBlast is done and sequence correction is starting
	// Update the label to say that
	progressDialog->getTopLabel()->setText( message );
}

void MainWindow::incrementProgress( int value, int total ) { 
	emit incrementProgressSignal( value, total );
}

void MainWindow::resetProgress() {
	emit resetProgressSignal();
}

void MainWindow::finishProgress() {
	emit finishProgressSignal();
}

void MainWindow::setProgressMessage( std::string message ) {
	emit setProgressMessageSignal( QString::fromStdString( message ));
}

///////////////// END Progress bar related fxns ///////////////////////


errorx::SequenceRecordsSP MainWindow::records() { return records_; }
void MainWindow::records( errorx::SequenceRecordsSP const records ) { records_ = records; }
