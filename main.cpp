/*
        Written by Wai Khoo <wlkhoo@gmail.com>
        Creating simple mosaics from images obtained from a rotating platform camera.
*/
#include <QApplication>
#include "mainwindow.h"
#include <iostream>
using namespace std;

int main (int argc, char *argv[])
{
        QApplication app (argc, argv);

        MainWindow rotplat;
        rotplat.show();
        return app.exec();
}
//Wai Khoo
