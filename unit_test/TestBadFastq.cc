#include "TestBadFastq.hh"

using namespace std;

TestBadFastq::TestBadFastq() {};

void TestBadFastq::testBadRunFromFileMenu() {
	main = new MainWindow();

	// Connect action handlers to check tab correctness after processing
	connect( main, &MainWindow::signalUnitTest,
			 this, &TestBadFastq::verifyTabCorrectness );
	
	// Set up options
	main->options_->nthreads( -1 );
	main->options_->error_threshold( errorx::constants::OPTIMIZED_THRESHOLD );
	main->options_->correction( 'N' );
	main->options_->allow_nonproductive( 1 );
	main->options_->species( "human" );
	main->options_->igtype( "Ig" );


	QTimer::singleShot( 1, this, &TestBadFastq::testBadFASTQDialog );
	main->newProjectAction->trigger();

	// Wait for protocol to finish in increments of 250ms
	while ( !executionDone ) QTest::qWait( 250 );
}

void TestBadFastq::verifyTabCorrectness() {
	executionDone = 1;

	QCOMPARE( main->records_, nullptr );
}

void TestBadFastq::testBadFASTQDialog() {
	foreach ( QWidget* w, QApplication::topLevelWidgets() ) {
		ConfirmFile* confirm = qobject_cast<ConfirmFile*>( w );
		if ( confirm != nullptr ) {
			confirm->setFile( "testing/bad.fastq" );

			QCOMPARE( confirm->fileEdit->text(), "testing/bad.fastq" );
			QVERIFY( confirm->radioButtonFASTQ->isChecked() );
			QVERIFY( !confirm->radioButtonTSV->isChecked() );

			QTest::keyClick( confirm, Qt::Key_Enter );
		}
	}
}

// QTEST_MAIN( TestBadFastq )
// #include "TestBadFastq.moc"

