/*
        MainWindow.h written by Wai Khoo <wlkhoo@gmail.com>
        A class that created the basic skeleton of the program to perform general task.
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "MosaicPanel.h"
#include "thread.h"

class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
// Constructor.
        MainWindow();
// Destructor.
        ~MainWindow();

public slots:
        void receiver(QImage, QImage, QImage, int, int, int, int);

private slots:
// Fuction to get a list of files and process each files.
        void open();
// Function to get a path from user for which the resulting mosaic he/she wants it to be saved in.
        void save();
// Turn the window into full screen.
        void fullScreen();

private:
// Fuction that process each frames (files)
//      void mosaicing();
// Functions to create the basic of the window.
        void createActions();
        void createMenus();

        int lr_x;
        int lr_y;
        int m_x;
        int m_y;

        MosaicPanel *panel_l;
        MosaicPanel *panel_m;
        MosaicPanel *panel_r;

        Thread myThread;

        QImage *image;
        QImage *l_image;
        QImage *m_image;
        QImage *r_image;

        QRgb l_color;
        QRgb m_color;
        QRgb r_color;

        QLabel *l_column;
        QLabel *m_column;
        QLabel *r_column;

        QAction *mosaicAct;
        QAction *saveAct;
        QAction *exitAct;
        QAction *fullScreenAct;

        QMenu *fileMenu;
};
#endif
// Wai Khoo
