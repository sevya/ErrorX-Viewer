#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSettings>
#include <QIcon>
#include <QMessageBox>

#include "ProgressBar.hh"
#include "MainWindow.hh"
#include "ConfirmFile.hh"
#include "OptionsDialog.hh"

#include <functional>
#include <thread>
#include <unistd.h>
#include <iostream>

#include "ErrorXOptions.hh"
#include "gui_util.hh"

#include <QStyleFactory>

int main( int argc, char **argv ) {
	QApplication app( argc, argv );
	app.setWindowIcon( QIcon(":images/helix_icon.icns") );
	// app.setStyle(QStyleFactory::create("Windows"));
	// app.setStyle(QStyleFactory::create("Fusion"));
	// Load the embedded font.
#if defined(__linux__)
	gui_util::registerFont( app );
#endif
	MainWindow* main = new MainWindow();
	main->show();
	return app.exec(); 
}
