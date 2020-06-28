#include "mainwindow.h"
#include "plants.h"
#include "plantmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Plants plants;
    PlantManager plantManager(&plants);
    MainWindow w (&plantManager);
    w.show();
    return a.exec();
}
