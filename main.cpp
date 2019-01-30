#include "sunrisewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("sitr0n");
    QCoreApplication::setApplicationName("Sunrise");

    QApplication a(argc, argv);
    SunriseWindow w;
    w.show();

    return a.exec();
}
