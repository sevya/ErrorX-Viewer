#include <QtTest/QtTest>
#include <QApplication>
#include <QWidgetList>
#include <QTimer>

#include "MainWindow.hh"
#include "gui_util.hh"

class TestSetPreferences: public QObject {
	Q_OBJECT

public:
	TestSetPreferences();

private slots:
	void testQSettings();
	void testQSettingsRestoreDefaults();
	void testQSettingsCancel();
	void testOptionsFromQSettings();
	
public slots:
	void enterSettingsAccept();
	void enterSettingsAcceptDone();
	void enterSettingsRestoreDefaults();
	void enterSettingsRestoreDefaultsDone();
	void enterSettingsCancel();
	void enterSettingsCancelDone();
	// void enterSettingsReject();

private:
	MainWindow* main;
	bool executionDone=0;
};