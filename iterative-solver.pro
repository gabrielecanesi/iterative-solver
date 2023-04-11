QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    benchmarkresults.cpp \
    main.cpp \
    mainwindow.cpp \
    uiutils.cpp

HEADERS += \
    benchmarkresults.h \
    mainwindow.h \
    uiutils.h

FORMS += \
    benchmarkresults.ui \
    mainwindow.ui

TRANSLATIONS += \
    iterative-solver_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

INCLUDEPATH += lib/


macx {
    system(cd $$PWD && mkdir -p build && cd build && /usr/local/bin/cmake -DCMAKE_BUILD_TYPE=Release ../lib && make && cp libiterative_solver.a ../)
    LIBS += $$PWD/libiterative_solver.a
    INCLUDEPATH += /usr/local/include/eigen3
}



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
