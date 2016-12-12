#include "sharedvideo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SharedVideo w;
    w.show();

    return a.exec();
}
