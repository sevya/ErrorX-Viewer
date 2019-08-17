#ifndef WELCOMETAB_HH
#define WELCOMETAB_HH

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class WelcomeTab : public QWidget {
	Q_OBJECT

public:
	explicit WelcomeTab( QWidget* parent=0 );
	~WelcomeTab();

signals:
public slots:

private:
	void init();
	
	QWidget* welcomeMsg;
	QVBoxLayout* welcomeLayout;
	QLabel* welcomeLabel;
};

#endif // WELCOMETAB_HH
