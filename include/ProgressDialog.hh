#ifndef PROGRESSDIALOG_HH
#define PROGRESSDIALOG_HH

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QCloseEvent>
#include <QVBoxLayout>

#include <mutex>

class ProgressDialog  : public QWidget {
	Q_OBJECT

public:
	explicit ProgressDialog( QWidget* parent = 0 );
	~ProgressDialog();
	
	void reset();
	
	QPushButton* getCancelButton();
	QProgressBar* getBar();
	QLabel* getTopLabel();
	QLabel* getBottomLabel();

	void updateLowerText();
signals:
public slots:
	// void increment( int value, int total, std::mutex* mutex );
	// void reset();
	// void finish();
	// void setMessage( std::string message );

	int getValue();
	void setValue( int a );
	int getMaximum();
	void setMaximum( int a );

private:
	QVBoxLayout* mainLayout;
	QProgressBar* bar;
	QLabel* topLabel;
	QLabel* bottomLabel;
	QPushButton* cancelButton;

	int value_;
	int maximum_;
};

#endif // PROGRESSDIALOG_HH