#include "TestProjectExport.hh"

#include "gui_util.hh"

#include "SequenceRecords.hh"

using namespace std;

TestProjectExport::TestProjectExport() {};

void TestProjectExport::testRunFromFileMenu() {
	main = new MainWindow();

	// Connect action handlers to check tab correctness after processing
	connect( main, &MainWindow::signalUnitTest,
			 this, &TestProjectExport::verifyTabCorrectness );
	
	// Set up options
	main->options_->nthreads( -1 );
	main->options_->error_threshold( errorx::constants::OPTIMIZED_THRESHOLD );
	main->options_->correction( 'N' );
	main->options_->allow_nonproductive( 1 );
	main->options_->species( "human" );
	main->options_->igtype( "Ig" );


	QTimer::singleShot( 1, this, &TestProjectExport::enterDialog );
	main->newProjectAction->trigger();

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );
}

void TestProjectExport::verifyTabCorrectness() {
	executionDone = 1;

	verifyExport();
}

void TestProjectExport::enterDialog() {
	foreach ( QWidget* w, QApplication::topLevelWidgets() ) {
		ConfirmFile* confirm = qobject_cast<ConfirmFile*>( w );
		if ( confirm != nullptr ) {
			confirm->setFile( "../100.fastq" );

			QCOMPARE( confirm->fileEdit->text(), "../100.fastq" );
			QVERIFY( confirm->radioButtonFASTQ->isChecked() );
			QVERIFY( !confirm->radioButtonTSV->isChecked() );

			QTest::keyClick( confirm, Qt::Key_Enter );
		}
	}
}

void TestProjectExport::verifyExport() {
	
	// Instead of calling main->exportProject() I'm gonna go 
	// through dataTab so that it doesn't show the file menu
	main->dataTab_->update( /*fullData = */true );
	main->dataTab_->selectCheckBox();
	main->dataTab_->exportTable( "export_project.tsv");


	QCOMPARE( 
		gui_util::checksum( "export_project.tsv" ), 
		gui_util::checksum( "correct_table_long.tsv" ) 
		);

	QFile file( "export_project.tsv" );
	file.remove();
}


// QTEST_MAIN( TestProjectExport )
// #include "TestProjectExport.moc"

