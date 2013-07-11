#include <QApplication>
#include <QPushButton>
#include <QTranslator>

#include "parsehost.h"
#include "hosteditor.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    inicfg();
    initlogger();

    HostEditor he;
    he.show();


    return app.exec();

    //    return 0;
}

