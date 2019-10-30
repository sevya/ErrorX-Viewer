TEMPLATE = app
TARGET = "ErrorX_Viewer"

QT = core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

SOURCES = src/ProgressDialog.cc src/MainWindow.cc src/OptionsDialog.cc src/ConfirmFile.cc src/main.cc src/ErrorTab.cc src/SummaryTab.cc src/GeneTab.cc src/ClonotypeTab.cc src/DataTab.cc src/ParamsTab.cc src/qcustomplot.cc src/ErrorXWorker.cc src/gui_util.cc src/WelcomeTab.cc src/CDRTab.cc

HEADERS = include/ProgressDialog.hh include/MainWindow.hh include/OptionsDialog.hh include/ConfirmFile.hh include/ErrorTab.hh include/SummaryTab.hh include/GeneTab.hh include/ClonotypeTab.hh include/DataTab.hh include/ParamsTab.hh include/qcustomplot.hh include/ErrorXWorker.hh include/gui_util.hh include/WelcomeTab.hh include/CDRTab.hh

OBJECTS_DIR = obj/
MOC_DIR = obj/

INCLUDEPATH += include/ include/errorx/

CONFIG += c++11
RESOURCES = resources.qrc

ICON = images/helix_icon.icns

unix:!macx { 
	LIBS += -L lib/ -lerrorx
	QMAKE_POST_LINK = bash post_build_copy_linux.sh $$TARGET
}

macx {
	LIBS += -L lib/ -lerrorx
	QMAKE_POST_LINK = bash post_build_copy.sh $$TARGET
	ex_extra.files = bin database internal_data lib optional_file model.nnet 
	ex_extra.path = Contents/Resources
	QMAKE_BUNDLE_DATA += ex_extra
}

win32 {
	DEFINES += "ERRORX_EXPORTS"
	# LIBS += C:\Users\sevya\boost_1_68_0\boost_1_68_0\stage\lib\libboost_filesystem-vc141-mt-x64-1_68.lib
	LIBPATH += C:\Users\sevya\boost_1_68_0\boost_1_68_0\stage\lib\

	QMAKE_POST_LINK = C:\Qt\Qt5.10.1\5.10.1\msvc2017_64\bin\windeployqt.exe release

	SOURCES += src/errorx/AbSequence.cc \
		src/errorx/ErrorPredictor.cc \
		src/errorx/ErrorXOptions.cc \
		src/errorx/keras/DataChunk2D.cc \
		src/errorx/keras/DataChunkFlat.cc \
		src/errorx/keras/KerasModel.cc \
		src/errorx/keras/LayerActivation.cc \
		src/errorx/keras/LayerDense.cc \
		src/errorx/SequenceFeatures.cc \
		src/errorx/SequenceRecord.cc \
		src/errorx/ClonotypeGroup.cc \
		src/errorx/errorx.cc \
		src/errorx/IGBlastParser.cc \
		src/errorx/ProgressBar.cc \
		src/errorx/SequenceQuery.cc \
		src/errorx/SequenceRecords.cc \
		src/errorx/util.cc

	HEADERS += include/errorx/AbSequence.hh \
		include/errorx/ErrorPredictor.hh \
		include/errorx/ErrorXOptions.hh \
		include/errorx/keras/DataChunk2D.hh \
		include/errorx/keras/DataChunkFlat.hh \
		include/errorx/keras/KerasModel.hh \
		include/errorx/keras/LayerActivation.hh \
		include/errorx/keras/LayerDense.hh \
		include/errorx/SequenceFeatures.hh \
		include/errorx/SequenceRecord.hh \
		include/errorx/ClonotypeGroup.hh \
		include/errorx/errorx.hh \
		include/errorx/IGBlastParser.hh \
		include/errorx/ProgressBar.hh \
		include/errorx/SequenceQuery.hh \
		include/errorx/SequenceRecords.hh \
		include/errorx/util.hh
}


