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

	// vector<std::string> labels = { "SequenceID", "V_gene", "V_identity", "V_Evalue", "D_gene", "D_identity", "D_Evalue", "J_gene", "J_identity", "J_Evalue", "Strand", "Chain", "Productive", "CDR3_NT_sequence", "CDR3_AA_sequence", "Full_NT_sequence", "Full_GL_NT_sequence", "Full_AA_sequence", "Full_NT_sequence_corrected", "Full_AA_sequence_corrected", "N_errors" };

	// QTableWidgetItem* colName;
	// for ( size_t ii = 0; ii < labels.size(); ++ii ) {
	// 	colName = new QTableWidgetItem( QString::fromStdString(labels[ ii ]) );
	// 	colName->setTextAlignment( Qt::AlignLeft ); 
	// 	mainTable->setHorizontalHeaderItem( ii, colName );
	// }
	// mainTable->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );

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

	vector<std::string> labels;
	if ( fullData ) {
		labels = { "SequenceID", "V_gene", "V_identity", "V_Evalue", "D_gene", "D_identity", "D_Evalue", "J_gene", "J_identity", "J_Evalue", "Strand", "Chain", "Productive", "CDR3_NT_sequence", "CDR3_AA_sequence", "Full_NT_sequence", "Full_GL_NT_sequence", "Full_AA_sequence", "Full_NT_sequence_corrected", "Full_AA_sequence_corrected", "N_errors" };
	} else {
		labels = { "SequenceID", "V_gene", "D_gene", "J_gene", "Full_NT_sequence","Full_NT_sequence_corrected", "N_errors" };
	}

 	mainTable->setColumnCount( labels.size() );

	QTableWidgetItem* colName;
	for ( size_t ii = 0; ii < labels.size(); ++ii ) {
		colName = new QTableWidgetItem( QString::fromStdString(labels[ ii ]) );
		colName->setTextAlignment( Qt::AlignLeft ); 
		mainTable->setHorizontalHeaderItem( ii, colName );
	}

	mainTable->setRowCount( records_->good_records() );
 
	errorx::SequenceRecordPtr record;
	vector<string> tableData;
	int row_it = 0;

	for ( int ii = 0; ii < records_->size(); ++ii ) {
		
		record = records_->get( ii );

		if ( !record->isGood()) continue;
 
 		if ( fullData ) {
			tableData = {
				record->sequenceID(),
				record->v_gene(),
				record->sequence().v_identity_fmt(),
				record->sequence().v_evalue_fmt(),
				record->d_gene(),
				record->sequence().d_identity_fmt(),
				record->sequence().d_evalue_fmt(),
				record->j_gene(),
				record->sequence().j_identity_fmt(),
				record->sequence().j_evalue_fmt(),
				record->sequence().strand(),
				record->sequence().chain(),
				record->sequence().productive_fmt(),
				record->sequence().cdr3_nt_sequence(),
				record->sequence().cdr3_aa_sequence(),
				record->sequence().full_nt_sequence(),
				record->sequence().full_gl_nt_sequence(),
				record->sequence().full_aa_sequence(),
				record->sequence().full_nt_sequence_corrected(),
				record->sequence().full_aa_sequence_corrected(),
				to_string( record->n_errors() )
			};
		} else { 
			tableData = {
				record->sequenceID(),
				record->v_gene(),
				record->d_gene(),
				record->j_gene(),
				record->sequence().full_nt_sequence(),
				record->sequence().full_nt_sequence_corrected(),
				to_string( record->n_errors() )
			};
		}
 
		for ( size_t jj = 0; jj < tableData.size(); ++jj ) {
			mainTable->setItem( 
				row_it, jj, 
				new QTableWidgetItem( QString::fromStdString( tableData[jj] ))
				);
		}
 
		++row_it;
	}
}

QString DataTab::tableToQString() {
	QTableWidgetItem* currentCell;
	QString text = "";

	vector<QString> labels;
	if ( mainTable->columnCount() == 7 ) {
		labels = { "SequenceID", "V_gene", "D_gene", "J_gene", "Full_NT_sequence","Full_NT_sequence_corrected", "N_errors" };
	} else {
		labels = { "SequenceID", "V_gene", "V_identity", "V_Evalue", "D_gene", "D_identity", "D_Evalue", "J_gene", "J_identity", "J_Evalue", "Strand", "Chain", "Productive", "CDR3_NT_sequence", "CDR3_AA_sequence", "Full_NT_sequence", "Full_GL_NT_sequence", "Full_AA_sequence", "Full_NT_sequence_corrected", "Full_AA_sequence_corrected", "N_errors" };
	}

	vector<QString>::const_iterator it = labels.begin();
	text += *it;
	++it;
	for ( ; it != labels.end(); ++it ) {
		text += "\t";
		text += *it;
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

	update( /*fullData = */true );
	selectCheckBox();
	
	QString text = tableToQString();
	QClipboard* clipboard = QGuiApplication::clipboard();
	clipboard->clear();
	clipboard->setText( text );
}

void DataTab::exportTable( QString inputFile ) {

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

	QMessageBox box( QMessageBox::NoIcon,
		 "Success!",
		 "File written succesfully!",
		 QMessageBox::Ok );

	box.exec();
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