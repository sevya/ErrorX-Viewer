#include <QApplication>

#include "TestFastq.hh"
#include "TestFasta.hh"
#include "TestTSV.hh"
#include "TestBadFastq.hh"
#include "TestProjectExport.hh"
#include "TestProjectImport.hh"
#include "TestSetPreferences.hh"
#include "gui_util.hh"

// Note: This is equivalent to QTEST_APPLESS_MAIN for multiple test classes.
int main( int argc, char** argv ) {
	QApplication app( argc, argv );
	#if defined(__linux__)
		gui_util::registerFont( app );
	#endif
	int status = 0;
	{
		TestFastq tc;
		status |= QTest::qExec(&tc, argc, argv);
	}
	{
		TestFasta tc;
		status |= QTest::qExec(&tc, argc, argv);
	}
	{
		TestTSV tc;
		status |= QTest::qExec(&tc, argc, argv);
	}
	{
		TestBadFastq tc;
		status |= QTest::qExec(&tc, argc, argv);
	}
	{
		TestProjectExport tc;
		status |= QTest::qExec(&tc, argc, argv);	
	}
	{
		TestProjectImport tc;
		status |= QTest::qExec(&tc, argc, argv);	
	}
	{
		TestSetPreferences tc;
		status |= QTest::qExec(&tc, argc, argv);	
	}
	return status;
}