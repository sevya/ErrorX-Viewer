#ifndef OPTIONSDIALOG_HH
#define OPTIONSDIALOG_HH
 
#include <QDialog>
#include <QSettings>
#include <QString>
#include <QGridLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QDialogButtonBox>


class OptionsDialog : public QDialog
{
	Q_OBJECT
 
public:
	explicit OptionsDialog( QSettings* settings, QDialog* parent=0 );
	~OptionsDialog();
 
private slots:
	void acceptDialog();
	void rejectDialog();
	void restoreDefaults();
 
private:
	void initErrorThreshold(); // Build UI for error threshold
 	void initThreads();        // Build UI for # threads
	void initChars();          // Build UI for correction char
	void initNonProd();        // Build UI for allow nonproductive
	void initButtonBox();      // Build UI for OK/cancel button box
	
	QSettings* settings_; 
 	QVBoxLayout* mainLayout;
	QWidget* lowerParent;
	QGridLayout* lowerLayout;

	QLabel* label0;
	QLabel* label1;
	QLabel* label2;
	QLabel* label3;
	QLabel* label4;
	QLabel* label5;
	QLabel* label6;
	QSpinBox* spinBox;
	QGridLayout* sliderLayout;
	QWidget* sliderParent;
	QLineEdit* charLine;
	QComboBox* comboBox;
	QSlider* slider;

	QDialogButtonBox* buttonBox;
	QPushButton* restoreDefaultsButton;
};
 
#endif // OPTIONSDIALOG_HH