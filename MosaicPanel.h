/*
        MosaicPanel.h written by Wai Khoo <wlkhoo@gmail.com>
        A class to create an ared to display the mosaics and to save images.
        MosaicPanel is called from MainWindow
*/
#ifndef MOSAICPANEL_H
#define MOSAICPANEL_H

#include <QtGui>

class MosaicPanel : public QWidget
{
        Q_OBJECT
public:
// Constructor and destructor.
        MosaicPanel (QWidget* parent=0, Qt::WFlags f=0);
        ~MosaicPanel();

// to initialize an image with the given size.
        void initialize (int);
// receive several columns from the original image (after processing in MainWindow).
        void receive (QImage&, int, int);
// display the resulting mosaic
//      void show();
// save the mosaic in user specified path.
        void saveImage (const QString&);

protected:
        void paintEvent (QPaintEvent*);
        void mousePressEvent (QMouseEvent* e);
        void mouseReleaseEvent (QMouseEvent* e);
        void mouseMoveEvent (QMouseEvent* e);

private:
        QImage *mosaic;

        int _x;
        int _y;
        int _px;
        int _py;
        int row;
        int column;

        bool _pressed;

};

#endif
// Wai Khoo
