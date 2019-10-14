#include "ClonotypeTab.hh"

#include <Qt>

#include <QPushButton>
#include <QStackedLayout>
#include <QGridLayout>
#include <QClipboard>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QDebug>

#include <iostream>

#include "SequenceRecords.hh"
#include "SequenceRecord.hh"
#include "ClonotypeGroup.hh"

ClonotypeTab::ClonotypeTab( QWidget* parent ) :
	QWidget( parent ),
	records_( nullptr )
{
	init();
}

ClonotypeTab::~ClonotypeTab() {}

void ClonotypeTab::init() {
	createTabWidget();
}

void ClonotypeTab::createTabWidget() {
	mainLayout = new QVBoxLayout( this );
	setLayout( mainLayout );

	cloneTable = new QTableWidget( 0, 3, this );

	col1 = new QTableWidgetItem( "Clonotype" );
	col1->setTextAlignment( Qt::AlignLeft ); 

	col2 = new QTableWidgetItem( "# occurrences" );
	col2->setTextAlignment( Qt::AlignLeft ); 

	col3 = new QTableWidgetItem( "# somatic variants" );
	col3->setTextAlignment( Qt::AlignLeft ); 

	cloneTable->setHorizontalHeaderItem( 0, col1 );
	cloneTable->setHorizontalHeaderItem( 1, col2 );
	cloneTable->setHorizontalHeaderItem( 2, col3 );

	cloneTable->horizontalHeader()->setSectionResizeMode( 0, QHeaderView::ResizeToContents );
	cloneTable->horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Interactive );
	cloneTable->horizontalHeader()->setSectionResizeMode( 2, QHeaderView::Interactive );
	cloneTable->horizontalHeader()->setStretchLastSection( true );

	mainLayout->addWidget( cloneTable );

	lowerPanel = new QWidget(); // this );
	lowerLayout = new QHBoxLayout(); // this );
	lowerPanel->setLayout( lowerLayout );

	copyButton = new QPushButton( "Copy selected cells" );//, this );
	QSizePolicy pol = copyButton->sizePolicy();
	pol.setHorizontalPolicy( QSizePolicy::Fixed );
	copyButton->setSizePolicy( pol );
	// Connect copy button action
	connect( copyButton, &QPushButton::released, 
			 this, &ClonotypeTab::copySelected );
	lowerLayout->addWidget( copyButton );


	copyAllButton = new QPushButton( "Copy table" );//, this );
	copyAllButton->setSizePolicy( pol );
	// Connect copy button action
	connect( copyAllButton, &QPushButton::released, 
			 this, &ClonotypeTab::copyAll );
	lowerLayout->addWidget( copyAllButton );


	exportButton = new QPushButton( "Export table" );//, this );
	copyAllButton->setSizePolicy( pol );
	// Connect copy button action
	connect( exportButton, &QPushButton::released, 
			 // this, &ClonotypeTab::exportTable );
			[this](){ exportTable(""); } );
	
	lowerLayout->addWidget( exportButton );

	// Add this so everything is left-aligned
	lowerLayout->addStretch();

	mainLayout->addWidget( lowerPanel );

}

void ClonotypeTab::createAltWidget() {
	altWidget = new QWidget();
	altLayout = new QVBoxLayout();
	QLabel* text = new QLabel( "<b>Clonotype view is not available when using TSV input</b>", this );
	text->setAlignment(Qt::AlignCenter);
	altLayout->addWidget( text );
	altWidget->setLayout( altLayout );
	setLayout( altLayout );
}

void ClonotypeTab::update() {
	if ( records_==nullptr ) return;

	// Remove whatever is currently in the layout
	qDeleteAll( this->children() );

	// TSV input does not have this tab
	if ( records_->get_options()->format() == "tsv" ) {
		setEnabled( false );

		// Add in the alternative layout
		createAltWidget();

		return;
	} 
	
	setEnabled( true );

	createTabWidget();

	// clear table if it's been used before
	cloneTable->setRowCount( 0 );

	using namespace errorx;
 
	// Get clonotype list
	std::vector<ClonotypeGroup> clonotypes = records_->clonotypes();
 
	// Only show top N clonotypes on the plot
	size_t N = 100;
 
	// Sort vector of ClonotypeGroups by the number of members
	std::sort( clonotypes.begin(), clonotypes.end(),
		[]( ClonotypeGroup const & a, ClonotypeGroup const & b ) -> bool
	{
		return a.size() > b.size();
	});
 
	// If the length of the V gene vector is less than N, adjust accordingly
	N = ( N < clonotypes.size() ) ? N : clonotypes.size();
 
	cloneTable->setRowCount( N );
 
	std::vector<ClonotypeGroup>::const_iterator it;
	std::string clonotypeName;
	int row_it = 0;
 
 	QTableWidgetItem* name;
 	QTableWidgetItem* occurrences;
 	QTableWidgetItem* variants;

	for ( it = clonotypes.begin(); it != clonotypes.end(); ++it ) {
 
 		name = new QTableWidgetItem();
		name->setData( 0, QString::fromStdString( it->toString() ));
		name->setFlags( name->flags() ^ Qt::ItemIsEditable );

		occurrences = new QTableWidgetItem();
		occurrences->setData( 0, QString::number( it->size() ));
		occurrences->setFlags( occurrences->flags() ^ Qt::ItemIsEditable );

		variants = new QTableWidgetItem();
		variants->setData( 0, QString::number( it->somatic_variants( 1 )));
		variants->setFlags( variants->flags() ^ Qt::ItemIsEditable );

		cloneTable->setItem( row_it, 0, name );
		cloneTable->setItem( row_it, 1, occurrences );
		cloneTable->setItem( row_it, 2, variants );

		++row_it;
	}
}

void ClonotypeTab::copySelected() {
	QList< QTableWidgetItem* > selectedCells = cloneTable->selectedItems();
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

QString ClonotypeTab::tableToQString() {
	QTableWidgetItem* currentCell;
	QString text = "";

	vector<QString> labels = { "Clonotype", "# occurrences", "# somatic variants" };

	for ( vector<QString>::const_iterator it = labels.begin();
		it != labels.end(); ++it ) {
		text += *it;
		text += "\t";
	}
	text += "\n";

	for ( int rowNo = 0; rowNo < cloneTable->rowCount(); ++rowNo ) {	
		for ( int colNo = 0; colNo < cloneTable->columnCount(); ++colNo ) {
			currentCell = cloneTable->item( rowNo, colNo );

			text += currentCell->text();
			text += "\t";
		}
		// add newline if it's not the last row
		// this prevents an extra newline at the end
		if ( rowNo != cloneTable->rowCount()-1 ) text += "\n";
	}

	return text;
}

void ClonotypeTab::copyAll() {
 	QString text = tableToQString();
	QClipboard* clipboard = QGuiApplication::clipboard();
	clipboard->clear();
	clipboard->setText( text );
}

void ClonotypeTab::exportTable( QString inputFile, bool showConfirmation/*=1*/ ) {
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

void ClonotypeTab::records( errorx::SequenceRecordsSP const records ) { records_ = records; }
errorx::SequenceRecordsSP ClonotypeTab::records() { return records_; }