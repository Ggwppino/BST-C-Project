#include "artistiwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ArtistiWindow w;
    w.starts();
    w.show();
    return a.exec();
}
