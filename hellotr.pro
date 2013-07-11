SOURCES      = main.cpp hosteditor.cpp log.cpp parsehost.cpp

LIBS += -L/usr/lib \
        -lPocoUtil -lPocoFoundation -lPocoJSON -lPocoXML

CONFIG += release

win32: {
LIBS += -liphlpapi
INCLUDEPATH += c:/msys/1.0/include
}

HEADERS += hosteditor.h log.h parsehost.h

FORMS += hosteditor.ui


# install
target.path = $$[QT_INSTALL_EXAMPLES]/linguist/hellotr
sources.files = $$SOURCES *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/linguist/hellotr
INSTALLS += target sources
