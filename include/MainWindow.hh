#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QAction>
#include <QMenuBar>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QSettings>
#include <QProgressDialog>
#include <QThread>
#include <QString>

#include "WelcomeTab.hh"
#include "SummaryTab.hh"
#include "GeneTab.hh"
#include "CDRTab.hh"
#include "ClonotypeTab.hh"
#include "ErrorTab.hh"
#include "DataTab.hh"
#include "ParamsTab.hh"
#include "ProgressDialog.hh"
#include "ErrorXWorker.hh"
#include "ConfirmFile.hh"


#include "ErrorXOptions.hh"
#include "SequenceRecords.hh"

#include <exception>

class MainWindow : public QWidget {
	Q_OBJECT

#ifdef UNITTEST
#include "../unit_test/friendship.hh"
#endif

public:
	explicit MainWindow( QWidget *parent = 0 );

	errorx::SequenceRecordsSP records();
	void records( errorx::SequenceRecordsSP const records );

	/////////////// Progress bar related fxns //////////////////////////
	void incrementProgress( int value, int total );
	void resetProgress();
	void finishProgress();
	void setProgressMessage( std::string message );
	/////////////// END Progress bar related fxns //////////////////////

signals:
	// Triggers functions in the unit tests to make sure that tabs
	// have the correct values
	void signalUnitTest();
	///////////////// Progress bar related fxns //////////////////////////
	void incrementProgressSignal( int value, int total );
	void resetProgressSignal();
	void finishProgressSignal();
	void setProgressMessageSignal( QString message );
	///////////////// END Progress bar related fxns //////////////////////
	

public slots:
	void fileMenuPrefs();
	void newProject();
	void importProject();
	void exportProject();
	void exportProjectToPDF();
	void showDocumentation();
	void showContact();
	void inputLicense();
	void exceptionMessage( QString const e );
	void runProtocolDone();
	// when a worker thread ends prematurely, I call this slot
	// to reset records to nullptr
	void resetRecords();

	///////////////// Progress bar related fxns //////////////////////////
	void incrementProgressSlot( int value, int total );
	void resetProgressSlot();
	void finishProgressSlot();
	void setProgressMessageSlot( QString message );
	///////////////// END Progress bar related fxns //////////////////////

protected:
	void dragEnterEvent( QDragEnterEvent* e ) override;
	void dropEvent( QDropEvent* e ) override;


private:
	void go( QString const & inputFile="tmp" );
	void initMenuBar();
	void initToolBar();
	void initTabs();
	void initSettings();
	void initProgressCallbacks();
	void activateHiddenWindows();
	void populateFromExisting( QString const & file );
	bool isValidErrorXTSV( QString const & file );


	errorx::ErrorXOptionsSP options_;
	errorx::SequenceRecordsSP records_;
	QSettings* settings_;

	QVBoxLayout* mainLayout;
	ProgressDialog* progressDialog;
	ErrorXWorker* runner;
	ConfirmFile* confirm;

	////////// Menu bar GUI elements //////////
	QMenuBar* menuBar;
	QMenu* fileMenu;
	QAction* newProjectAction;
	QAction* openProjectAction;
	QAction* saveAction;
	QAction* exportPDFAction;
	QAction* prefsAction;

	QMenu* helpMenu;
	QAction* documentationAction;
	QAction* contactAction;
	QAction* licenseAction;
	////////// END menu bar //////////

	////////// Toolbar GUI elements //////////
	QToolBar* toolBar;
	QAction* newProjectButton;
	QAction* importProjectButton;
	QAction* saveProjectButton;
	QAction* exportProjectButton;
	////////// END toolbar //////////

	////////// Tabs /////////////
	QTabWidget* tabWidget;
	WelcomeTab* welcomeTab_;
	SummaryTab* summaryTab_;
	GeneTab* geneTab_;
	CDRTab* cdrTab_;
	ClonotypeTab* clonotypeTab_;
	ErrorTab* errorTab_;
	DataTab* dataTab_;
	ParamsTab* paramsTab_;
	////////// END Tabs /////////

	

};

#endif // MAINWINDOW_H
