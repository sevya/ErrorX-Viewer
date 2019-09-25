#ifndef CONFIRMFILE_HH
#define CONFIRMFILE_HH
 
#include <QDialog>
#include <QRadioButton>
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>



#include "ErrorXOptions.hh"

 
class ConfirmFile : public QDialog {
	Q_OBJECT

public:
	explicit ConfirmFile( errorx::ErrorXOptionsSP options, QWidget* parent=0 );
	~ConfirmFile();
 
 	void setFile( QString const & inputFile );

public slots:
	void acceptDialog();
	void rejectDialog();
	void chooseFile();

private:
	void buildInputFileSection();
	void buildFileTypeSection();
	void buildReceptorTypeSection();
	void buildSpeciesSection();
	void autofillExtension( QString const & infile );

	errorx::ErrorXOptionsSP options_;

	QVBoxLayout* mainLayout;
	QDialogButtonBox* buttonBox;
	QWidget* inputWidget;
	QLabel* inputLabel;
	QGridLayout* inputLayout;
	QPushButton* selectFile;
	QString inputFile;
	QLineEdit* fileEdit;

	QGroupBox* extGroup;
	QHBoxLayout* extLayout;
	QLabel* label1;
	QRadioButton* radioButtonFASTQ;
	QRadioButton* radioButtonTSV;

	QGroupBox* igtypeGroup;
	QHBoxLayout* igtypeLayout;
	QLabel* label3;
	QRadioButton* radioButtonBCR;
	QRadioButton* radioButtonTCR;

	QGroupBox* speciesGroup;
	QHBoxLayout* speciesLayout;
	QLabel* label2;
	QRadioButton* radioButtonHuman;
	QRadioButton* radioButtonMouse;

// Needed for unit testing
#ifdef UNITTEST
#include "../unit_test/friendship.hh"
#endif

};
 
#endif // CONFIRMFILE_HH