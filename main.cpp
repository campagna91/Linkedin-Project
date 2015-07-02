#include <QApplication>
#include "clientstart.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientStart w;
    w.show();
    return a.exec();
}
