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


int main( int argc, char **argv ) {
	QApplication app( argc, argv );
	app.setWindowIcon( QIcon(":images/helix_icon.icns") );

	MainWindow* main = new MainWindow();
	main->show();
	return app.exec(); 
}