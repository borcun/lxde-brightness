#include "lxdebrightness.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LXDEBrightness w;

    w.show();

    return a.exec();
}
