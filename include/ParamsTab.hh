#ifndef PARAMSTAB_HH
#define PARAMSTAB_HH

#include <QWidget>
#include <QLabel>
#include <QGridLayout>

#include "SequenceRecords.hh"

class ParamsTab : public QWidget {
	Q_OBJECT

#ifdef UNITTEST
#include "../unit_test/friendship.hh"
#endif

public:
	explicit ParamsTab( QWidget *parent=0 );

	~ParamsTab();
	void update();

	void records( errorx::SequenceRecordsSP const records );
	errorx::SequenceRecordsSP records();

signals:
public slots:


private:
	void init();

	errorx::SequenceRecordsSP records_;
	
	QGridLayout* mainLayout;

	QLabel* label1;
	QLabel* label2;
	QLabel* label3;
	QLabel* label4;
	QLabel* label5;
	QLabel* label6;
	QLabel* label7;
	QLabel* label8;
	QLabel* formatLabel;
	QLabel* speciesLabel;
	QLabel* receptorLabel;
	QLabel* threadLabel;
	QLabel* errorLabel;
	QLabel* correctionLabel;
	QLabel* nonprodLabel;

	QWidget* filler;
	QWidget* filler2;
	QWidget* empty;
	QLabel* finePrint;
	QLabel* logo;

};

#endif // PARAMSTAB_HH
