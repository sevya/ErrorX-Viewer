#include "WelcomeTab.hh"

WelcomeTab::WelcomeTab( QWidget* parent ) :
	QWidget( parent )
{
	init();
}


WelcomeTab::~WelcomeTab() {}

void WelcomeTab::init() {
	welcomeMsg = new QWidget( this );
	welcomeLayout = new QVBoxLayout( this );
	welcomeMsg->setLayout( welcomeLayout );

	welcomeLabel = new QLabel( this );
	welcomeLabel->setText( 
		"<h2>Welcome to ErrorX!</h2>"
		"ErrorX is the state-of-the-art in error correction for immune repertoire data</br>"
		"<h2>Getting started:</h2>"
		"Drop a file here, click \"New project\" in the menu, or go to \"File->New Project\""
		"<h3>Valid file types:</h3>"
		"FASTQ files with extension .fastq or .fq<br>"
		"TSV files with four columns, tab-separated, no header:<br>1. Sequence ID, 2. NT sequence, 3. Germline sequence, 4. PHRED score"
		);
	welcomeLabel->setAlignment( Qt::AlignCenter );
	welcomeLayout->addWidget( welcomeLabel );

	setLayout( welcomeLayout );
}