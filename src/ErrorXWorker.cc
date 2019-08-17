#include "ErrorXWorker.hh"

#include <QMessageBox>

#include "MainWindow.hh"

#include "errorx.hh"
#include "ErrorXOptions.hh"
#include "SequenceRecords.hh"

#include <signal.h> // kill

ErrorXWorker::ErrorXWorker( errorx::ErrorXOptionsSP const options, 
	// ProgressDialog* const progressDialog,
	QObject* parent ) : 
		QThread( parent ),
		options_( options )
		// progressDialog_( progressDialog )
	{}


void ErrorXWorker::run() {
	try {
		// Run full protocol
		errorx::SequenceRecordsSP records = run_protocol( *options_ );

		// TODO refactor this section to use signal/slot
		// Get MainWindow parent
		MainWindow* mainWindow = qobject_cast<MainWindow*>( parent() );
		// Set its sequence records object to the one we just made
		mainWindow->records( records );
		// All done - clean up progress window
		// progressDialog_->close();
	} catch ( exception & e ) {
		// progressDialog_->close();
		emit threadCancelled();
		emit throwException( QString::fromStdString(e.what()) );
	}
}

void ErrorXWorker::quitWorker() {
	emit threadCancelled();
	// progressDialog_->close();
	terminate();
}