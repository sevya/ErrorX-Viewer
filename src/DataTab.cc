#include "DataTab.hh"

#include <Qt>
#include <QApplication>
#include <QClipboard>
#include <QPushButton>
#include <QStackedLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QCheckBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QMessageBox>

#include <iostream>
#include <functional>

#include "SequenceRecords.hh"
#include "SequenceRecord.hh"

DataTab::DataTab( QWidget* parent ) :
	QWidget( parent ),
	records_( nullptr )
{
	init();
}

DataTab::~DataTab() {}

void DataTab::init() {
	mainLayout = new QVBoxLayout( this );
	setLayout( mainLayout );

	mainTable = new QTableWidget( 0, 0, this );
	mainTable->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );

	mainLayout->addWidget( mainTable );

	lowerPanel = new QWidget( this );
	lowerLayout = new QHBoxLayout(); // this );
	lowerPanel->setLayout( lowerLayout );

	copyButton = new QPushButton( "Copy selected cells", this );
	QSizePolicy pol = copyButton->sizePolicy();
	pol.setHorizontalPolicy( QSizePolicy::Fixed );
	copyButton->setSizePolicy( pol );
	// Connect copy button action
	connect( copyButton, &QPushButton::released, 
			 this, &DataTab::copySelected );

	lowerLayout->addWidget( copyButton );

	copyAllButton = new QPushButton( "Copy table", this );
	copyAllButton->setSizePolicy( pol );
	// Connect copy button action
	connect( copyAllButton, &QPushButton::released, 
			 this, &DataTab::copyAll );

	lowerLayout->addWidget( copyAllButton );

	exportButton = new QPushButton( "Export table", this );
	copyAllButton->setSizePolicy( pol );
	// Connect copy button action
	connect( exportButton, &QPushButton::released, 
			 // this, &DataTab::exportTable );
			[this](){ exportTable(""); } );

	lowerLayout->addWidget( exportButton );

	// Add this so the check box is right-aligned
	lowerLayout->addStretch();
	
	checkBox = new QCheckBox( "Show full data?", this );
	checkBox->setChecked( false );

	// Connect checking this box to redoing update
	connect( checkBox, &QCheckBox::stateChanged, 
			 this, &DataTab::updateFromBox );
	lowerLayout->addWidget( checkBox );

	mainLayout->addWidget( lowerPanel );
}

void DataTab::updateFromBox() {
	update( checkBox->isChecked() );
}

void DataTab::selectCheckBox() {
	checkBox->setChecked( true );
}

void DataTab::update( bool fullData ) {
	// If the records object was never set -> return now
	if ( records_ == nullptr ) return;
	// Clear in case it's been used before
	mainTable->setRowCount( 0 );

	vector<std::string> labels = errorx::util::get_labels( fullData );

 	mainTable->setColumnCount( labels.size() );

	QTableWidgetItem* colName;
	for ( size_t ii = 0; ii < labels.size(); ++ii ) {
		colName = new QTableWidgetItem( QString::fromStdString(labels[ ii ]) );
		colName->setTextAlignment( Qt::AlignLeft ); 
		mainTable->setHorizontalHeaderItem( ii, colName );
	}

	mainTable->setRowCount( records_->good_records() );
 
	vector<vector<string>> tableData = records_->get_summary( fullData );

	for ( size_t row_it = 0; row_it < tableData.size(); ++row_it ) {
		for ( size_t col_it = 0; col_it < tableData[row_it].size(); ++col_it ) {
			mainTable->setItem( 
				row_it, col_it, 
				new QTableWidgetItem( QString::fromStdString( 
					tableData[ row_it ][ col_it ] 
					))
				);

		}
	}
}

QString DataTab::tableToQString() {
	QTableWidgetItem* currentCell;
	QString text = "";

	// TODO fix this hacky way
	vector<string> labels = errorx::util::get_labels( 
		/*fulldata=*/mainTable->columnCount()!=7 
		);

	// vector<QString> labels;

	// for ( string str : strLabels ) labels.push_back( QString::fromStdString( str ));

	vector<string>::const_iterator it = labels.begin();
	text += QString::fromStdString( *it );
	++it;
	for ( ; it != labels.end(); ++it ) {
		text += "\t";
		text += QString::fromStdString( *it );
	}
	text += "\n";


	for ( int rowNo = 0; rowNo < mainTable->rowCount(); ++rowNo ) {	
		int colNo = 0;
		currentCell = mainTable->item( rowNo, colNo );
		text += currentCell->text();
		++colNo;
		for ( ; colNo < mainTable->columnCount(); ++colNo ) {
			text += "\t";
			currentCell = mainTable->item( rowNo, colNo );
			text += currentCell->text();
		}
		// add newline if it's not the last row
		// this prevents an extra newline at the end
		if ( rowNo != mainTable->rowCount()-1 ) text += "\n";
	}

	return text;
}

void DataTab::copyAll() {
	// update( /*fullData = */true );
	// selectCheckBox();
	QString text = tableToQString();
	QClipboard* clipboard = QGuiApplication::clipboard();
	clipboard->clear();
	clipboard->setText( text );
}

void DataTab::exportTable( QString inputFile, bool showConfirmation/*=1*/ ) {

	if ( inputFile == "" ) {
		inputFile = QFileDialog::getSaveFileName(
			this,
			tr("Export table")
		);
	}

	if ( inputFile.isNull() ) return;

	QString text = tableToQString();

	QFile file( inputFile );

	if ( !file.open( QIODevice::WriteOnly )){
		file.close();
	} else {
		QTextStream out( &file ); 
		out << text;
		file.close();
	}

	if ( showConfirmation ) {
		QMessageBox box( QMessageBox::NoIcon,
		 "Success!",
		 "File written succesfully!",
		 QMessageBox::Ok );

		box.exec();
	}
}

void DataTab::copySelected() {
	QList< QTableWidgetItem* > selectedCells = mainTable->selectedItems();
	int lastRow = 0;
	int row;
	QString selectedText = "";
	for ( int ii = 0; ii < selectedCells.size(); ++ii ) {
		QTableWidgetItem* currentCell = selectedCells.at( ii );
		row = currentCell->row();
		// Separate columns by a tab, rows by a newline character
		if ( row != lastRow ) {
			selectedText += "\n";
			lastRow = row;
		}
 
		selectedText += currentCell->text();
		selectedText += "\t";
	}
 
	QClipboard* clipboard = QGuiApplication::clipboard();
	clipboard->clear();
	clipboard->setText( selectedText );
}

void DataTab::records( errorx::SequenceRecordsSP const records ) { records_ = records; }
errorx::SequenceRecordsSP DataTab::records() { return records_; }