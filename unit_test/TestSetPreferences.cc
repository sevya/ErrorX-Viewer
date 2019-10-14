#include "TestSetPreferences.hh"
#include <QtTest/QtTest>
#include <QApplication>
#include <QWidgetList>
#include <QTimer>
#include "gui_util.hh"
#include "constants.hh"

#include "OptionsDialog.hh"
#include "ErrorXOptions.hh"

using namespace std;
using namespace errorx;

TestSetPreferences::TestSetPreferences() {};

void TestSetPreferences::testQSettings() {
	// Create new QSettings object with custom values
	QSettings* settings = new QSettings( "EndeavorBio", "ErrorX" );
	settings->setValue( "error_threshold" , 0.6 );
	settings->setValue( "correction" , 'X' );
	settings->setValue( "n_threads" , 2 );
	settings->setValue( "allow_nonproductive" , 0 );
	settings->sync();

	// MainWindow will read settings from file
	main = new MainWindow();

	// Connect action handlers to check tab correctness after processing
	connect( main, &MainWindow::signalUnitTest,
			 this, &TestSetPreferences::enterSettingsAcceptDone );

	// Trigger the preferences window to open
	QTimer::singleShot( 1, this, &TestSetPreferences::enterSettingsAccept );
	main->prefsAction->trigger();

	// Wait for preferences window to close
	while ( !executionDone ) QTest::qWait( 250 );
}


void TestSetPreferences::testQSettingsRestoreDefaults() {
	// Create new QSettings object with custom values
	QSettings* settings = new QSettings( "EndeavorBio", "ErrorX" );
	settings->setValue( "error_threshold" , 0.6 );
	settings->setValue( "correction" , 'X' );
	settings->setValue( "n_threads" , 2 );
	settings->setValue( "allow_nonproductive" , 0 );
	settings->sync();

	// MainWindow will read settings from file
	main = new MainWindow();

	// Connect action handlers to check tab correctness after processing
	connect( main, &MainWindow::signalUnitTest,
			 this, &TestSetPreferences::enterSettingsRestoreDefaultsDone );

	// Trigger the preferences window to open
	QTimer::singleShot( 1, this, &TestSetPreferences::enterSettingsRestoreDefaults );
	main->prefsAction->trigger();

	// Wait for preferences window to close
	while ( !executionDone ) QTest::qWait( 250 );
}

void TestSetPreferences::testQSettingsCancel() {
	// Create new QSettings object with custom values
	QSettings* settings = new QSettings( "EndeavorBio", "ErrorX" );
	settings->setValue( "error_threshold" , 0.6 );
	settings->setValue( "correction" , 'X' );
	settings->setValue( "n_threads" , 2 );
	settings->setValue( "allow_nonproductive" , 0 );
	settings->sync();

	// MainWindow will read settings from file
	main = new MainWindow();

	// Connect action handlers to check tab correctness after processing
	connect( main, &MainWindow::signalUnitTest,
			 this, &TestSetPreferences::enterSettingsCancelDone );

	// Trigger the preferences window to open
	QTimer::singleShot( 1, this, &TestSetPreferences::enterSettingsCancel );
	main->prefsAction->trigger();

	// Wait for preferences window to close
	while ( !executionDone ) QTest::qWait( 250 );
}

void TestSetPreferences::testOptionsFromQSettings() {
	QSettings* settings = new QSettings( "test", "test" );
	settings->setValue( "error_threshold" , 0.6 );
	settings->setValue( "correction" , 'X' );
	settings->setValue( "n_threads" , 2 );
	settings->setValue( "allow_nonproductive" , 0 );

	ErrorXOptionsSP options = gui_util::optionsFromSettings( settings );

	QCOMPARE( options->error_threshold(), 0.6 );
	QCOMPARE( options->correction(), 'X' );
	QCOMPARE( options->nthreads(), 2 );
	QCOMPARE( options->allow_nonproductive(), 0 );
}

void TestSetPreferences::enterSettingsAccept() {
	foreach ( QWidget* w, QApplication::topLevelWidgets() ) {
		OptionsDialog* options = qobject_cast<OptionsDialog*>( w );
		if ( options != nullptr ) {
			QCOMPARE( options->slider->value(), 1 );
			QCOMPARE( options->spinBox->value(), 2 );
			QCOMPARE( options->charLine->text(), "X" );
			QCOMPARE( options->comboBox->currentIndex(), 0 );

			options->slider->setValue( 3 );
			options->spinBox->setValue( 3 );
			options->charLine->setText( "P" );
			options->comboBox->setCurrentIndex( 1 );

			QTest::keyClick( options, Qt::Key_Enter );
		}
	}
}

void TestSetPreferences::enterSettingsRestoreDefaults() {
	foreach ( QWidget* w, QApplication::topLevelWidgets() ) {
		OptionsDialog* options = qobject_cast<OptionsDialog*>( w );
		if ( options != nullptr ) {
			QCOMPARE( options->slider->value(), 1 );
			QCOMPARE( options->spinBox->value(), 2 );
			QCOMPARE( options->charLine->text(), "X" );
			QCOMPARE( options->comboBox->currentIndex(), 0 );

			options->slider->setValue( 3 );
			options->spinBox->setValue( 3 );
			options->charLine->setText( "P" );
			options->comboBox->setCurrentIndex( 1 );

			// click on Restore Defaults button
			QTest::mouseClick( options->restoreDefaultsButton, 
								Qt::LeftButton, 
								Qt::NoModifier
								);
			// click on Enter button
			QTest::keyClick( options, Qt::Key_Enter );

		}
	}
}

void TestSetPreferences::enterSettingsCancel() {
	foreach ( QWidget* w, QApplication::topLevelWidgets() ) {
		OptionsDialog* options = qobject_cast<OptionsDialog*>( w );
		if ( options != nullptr ) {

			options->slider->setValue( 3 );
			options->spinBox->setValue( 3 );
			options->charLine->setText( "P" );
			options->comboBox->setCurrentIndex( 1 );

			// locate the Cancel button then click on it
			QList<QAbstractButton*> buttons = options->buttonBox->buttons();
			for ( auto it = buttons.begin(); it != buttons.end(); ++it ) {
				if ( options->buttonBox->buttonRole( *it ) == 
					QDialogButtonBox::RejectRole ) {

					QTest::mouseClick( *it, 
										Qt::LeftButton, 
										Qt::NoModifier 
										// buttonPosition
										);
				}
			}
			
		}
	}
}

void TestSetPreferences::enterSettingsAcceptDone() {
	executionDone = 1;

	QCOMPARE( main->options_->error_threshold(), 0.9 );
	QCOMPARE( main->options_->nthreads(), 3 );
	QCOMPARE( main->options_->correction(), 'P' );
	QCOMPARE( main->options_->allow_nonproductive(), 1 );
}

void TestSetPreferences::enterSettingsRestoreDefaultsDone() {
	executionDone = 1;

	QCOMPARE( main->options_->error_threshold(), 
				constants::OPTIMIZED_THRESHOLD );
	QCOMPARE( (unsigned int)main->options_->nthreads(), 
				thread::hardware_concurrency() );
	QCOMPARE( main->options_->correction(), 'N' );
	QCOMPARE( main->options_->allow_nonproductive(), 1 );
}

void TestSetPreferences::enterSettingsCancelDone() {
	executionDone = 1;

	QCOMPARE( main->options_->error_threshold(), 0.6 );
	QCOMPARE( main->options_->nthreads(), 2 );
	QCOMPARE( main->options_->correction(), 'X' );
	QCOMPARE( main->options_->allow_nonproductive(), 0 );
}

void TestSetPreferences::cleanupTestCase() {
	enterSettingsRestoreDefaults();
}

// QTEST_MAIN( TestSetPreferences )
// #include "TestSetPreferences.moc"

