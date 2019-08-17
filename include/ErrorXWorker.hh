#ifndef ERRORXWORKER_HH
#define ERRORXWORKER_HH

#include <QThread>

#include "ErrorXOptions.hh"
// #include "ProgressDialog.hh"

#include <exception>

class ErrorXWorker : public QThread {
	Q_OBJECT	

public:
	ErrorXWorker( errorx::ErrorXOptionsSP const options, 
		// ProgressDialog* const progressDialog,
		QObject* parent=nullptr
		);

signals:
	void throwException( QString const e );
	void threadCancelled();

public slots:
	void quitWorker();

private:
	void run();

	errorx::ErrorXOptionsSP const options_;
	// ProgressDialog* const progressDialog_;
};

#endif // ERRORXWORKER_HH