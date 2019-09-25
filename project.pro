TEMPLATE = app
TARGET = ErrorX_Viewer

QT = core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

SOURCES = src/ProgressDialog.cc src/MainWindow.cc src/OptionsDialog.cc src/ConfirmFile.cc src/main.cc src/ErrorTab.cc src/SummaryTab.cc src/GeneTab.cc src/ClonotypeTab.cc src/DataTab.cc src/ParamsTab.cc src/qcustomplot.cc src/ErrorXWorker.cc src/gui_util.cc src/WelcomeTab.cc src/CDRTab.cc

HEADERS = include/ProgressDialog.hh include/MainWindow.hh include/OptionsDialog.hh include/ConfirmFile.hh include/ErrorTab.hh include/SummaryTab.hh include/GeneTab.hh include/ClonotypeTab.hh include/DataTab.hh include/ParamsTab.hh include/qcustomplot.hh include/ErrorXWorker.hh include/gui_util.hh include/WelcomeTab.hh include/CDRTab.hh

OBJECTS_DIR = obj/
MOC_DIR = obj/

INCLUDEPATH += include/ ../ErrorX_devel/include
LIBS += -L lib/ -lerrorx
CONFIG += c++11
RESOURCES = resources.qrc
QMAKE_POST_LINK = bash post_build_copy.sh $$TARGET
ICON = images/helix_icon.icns

CONFIG += qt debug

ex_extra.files = ../ErrorX_devel/bin ../ErrorX_devel/database ../ErrorX_devel/internal_data ../ErrorX_devel/lib ../ErrorX_devel/optional_file ../ErrorX_devel/model.nnet 
ex_extra.path = Contents/Resources
QMAKE_BUNDLE_DATA += ex_extra

