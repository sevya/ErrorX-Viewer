#ifndef DATATAB_HH
#define DATATAB_HH

#include <QWidget>
#include <QTableWidget>
#include <QCheckBox>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "SequenceRecords.hh"

class DataTab : public QWidget {
	Q_OBJECT

#ifdef UNITTEST
#include "../unit_test/friendship.hh"
#endif

public:
	explicit DataTab( QWidget* parent=0 );

	~DataTab();
	void update( bool fullData=0 );
	void updateFromBox();
	void selectCheckBox();
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
	QWidget* lowerPanel;
	QHBoxLayout* lowerLayout;
	QPushButton* copyButton;
	QPushButton* copyAllButton;
	QPushButton* exportButton;
	QCheckBox* checkBox;
	QTableWidget* mainTable;
};

#endif // DATATAB_HH
