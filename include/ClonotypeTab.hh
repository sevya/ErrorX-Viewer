#ifndef CLONOTYPETAB_HH
#define CLONOTYPETAB_HH

#include <QWidget>
#include <QLabel>

#include "qcustomplot.hh"

#include "SequenceRecords.hh"
#include "constants.hh"

class ClonotypeTab : public QWidget {
	Q_OBJECT

#ifdef UNITTEST
#include "../unit_test/friendship.hh"
#endif
	
public:
	explicit ClonotypeTab( QWidget *parent=0 );
	
	~ClonotypeTab();
	void update();

	void records( errorx::SequenceRecordsSP const records );
	errorx::SequenceRecordsSP records();

signals:
public slots:
	void copySelected();
	void copyAll();
	void exportTable( QString inputFile="", bool showConfirmation=1 );


private:
	void init();
	QString tableToQString();

	errorx::SequenceRecordsSP records_;

	QVBoxLayout* mainLayout;
	QTableWidget* cloneTable;
	QTableWidgetItem* col1;
	QTableWidgetItem* col2;
	QTableWidgetItem* col3;
	QWidget* lowerPanel;
	QHBoxLayout* lowerLayout;
	QPushButton* copyButton;
	QPushButton* copyAllButton;
	QPushButton* exportButton;

};

#endif // CLONOTYPETAB_HH
