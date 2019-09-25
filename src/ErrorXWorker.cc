#include "ErrorXWorker.hh"

#include <QMessageBox>

#include "MainWindow.hh"

#include "errorx.hh"
#include "ErrorXOptions.hh"
#include "SequenceRecords.hh"

#include <signal.h> // kill

ErrorXWorker::ErrorXWorker( errorx::ErrorXOptionsSP const options, 
	QObject* parent ) : 
		QThread( parent ),
		options_( options )
	{}


void ErrorXWorker::run() {
	try {
		// Run full protocol
		errorx::SequenceRecordsSP records = run_protocol( *options_ );

		// Get MainWindow parent
		MainWindow* mainWindow = qobject_cast<MainWindow*>( parent() );
		// Set its sequence records object to the one we just made
		mainWindow->records( records );

	} catch ( exception & e ) {

		emit threadCancelled();
		emit throwException( QString::fromStdString(e.what()) );
	}
}

void ErrorXWorker::quitWorker() {
	emit threadCancelled();
	terminate();
}