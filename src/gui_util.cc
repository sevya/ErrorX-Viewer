#include "gui_util.hh"
#include "constants.hh"

#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QString>
#include <QCryptographicHash>
#include <QGuiApplication>
#include <QClipboard>
#include <QTextStream>
#include <QFont>
#include <QFontDatabase>

#include <QDebug>
#include <thread>
#include <iostream>

namespace gui_util {

void registerFont( QApplication const & app ) {
	QString fontPath = ":/images/LucidaGrande.ttf";
	int fontId = QFontDatabase::addApplicationFont( fontPath );
	if (fontId != -1) {
		QFont font("LucidaGrande");
		app.setFont(font);
	}
}

errorx::ErrorXOptionsSP optionsFromSettings( QSettings* const settings ) {
	using namespace errorx;

	ErrorXOptionsSP options( new ErrorXOptions( "tmp", "fastq" ));
	options->allow_nonproductive( 
		settings->value( "allow_nonproductive", 1 ).toBool() 
		);
	options->nthreads( 
		settings->value( "n_threads", std::thread::hardware_concurrency() ).toInt() 
		);
	options->error_threshold( 
		settings->value( "error_threshold", constants::OPTIMIZED_THRESHOLD ).toDouble() 
		);
	// TODO make this language independent
	options->correction( 
		settings->value( "correction", "N" ).toChar().toLatin1() 
		);

	// Set basepath based on OS and the way the app gets bundled
	QString basePath;
#if defined(_WIN32) || defined(_WIN64)
// TODO fill in here
#elif defined(__APPLE__) || defined(__MACH__) 
	basePath = QCoreApplication::applicationDirPath() +
		QDir::separator() +
		".." + 
		QDir::separator() +
		"Resources" + 
		QDir::separator();
#elif defined(__linux__)
	basePath = "/usr/local/src/errorx/";
#endif

	options->errorx_base( basePath.toStdString() );
	options->verbose( 0 );

	return options;
}

QByteArray checksum( QString const & fileName ) {
	QCryptographicHash hash( QCryptographicHash::Sha1 );
	QFile file( fileName );

	if ( file.open( QIODevice::ReadOnly ) ) {
		hash.addData( file.readAll() );
	}

	// Retrieve the SHA1 signature of the file
	return hash.result();
}

void clipboardToFile( QString const & fileName ) {
	QFile file( fileName );
	file.open( QIODevice::WriteOnly );
	QTextStream out( &file );
	out << QGuiApplication::clipboard()->text();
	file.close();
}

} // namespace gui_util
