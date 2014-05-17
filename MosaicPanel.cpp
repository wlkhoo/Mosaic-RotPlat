/*
        MosaicPanel.cpp written by Wai Khoo <wlkhoo@gmail.com>
        Implementation of the MosaicPanel class.
*/
#include <QtGui>
#include "MosaicPanel.h"

// Constructor.
MosaicPanel::MosaicPanel (QWidget* parent, Qt::WFlags f)
        : QWidget (parent, f)
{
        setCursor (Qt::PointingHandCursor);             // change pointer as it hovers through the display area.

        // setting how the area is going to looks like.
        QPalette pal;
        pal.setColor (QPalette::Window, QColor (Qt::black));
        pal.setColor (QPalette::WindowText, QColor (Qt::white));
        setPalette(pal);
        setBackgroundRole (QPalette::Window);
        setAutoFillBackground (true);

        // initialize initial variables.
        _px = 0;
        _py = 0;
        row = 0;
        column = 0;
        mosaic = new QImage();
}

MosaicPanel::~MosaicPanel()
{
        // Destructor with nothing in particular to perform further.
}

/*
        This function get called from open() of MainWindow
        Once open() has figure out how long is the resulting mosaic's width going to be, it will
                passed it to here to initialize the variable mosaic instead of leaving it at null.
*/
void MosaicPanel::initialize (int size)
{
        // Since the original purpose of this program is for RotPlat.wmv.  The height (240) is hard-coded.
        mosaic = new QImage (size, 240, QImage::Format_RGB32);
}

/*
        This function get called from the open() of MainWindow
        This function receive selected columns from each frame and paint it on the display area
                with the given coordinate (x, y).
*/
void MosaicPanel::receive(QImage& p, int x, int y)
{
        QPainter painter (mosaic);
        painter.setBackgroundMode (Qt::OpaqueMode);
        painter.setBackground (QBrush(Qt::black));
        painter.drawImage (x, y, p);
        repaint();      //thread testing.
}

/*
        This function is also called from open() of MainWindow
        Once all the necessary frames have been processed, show() will display the image on
                the display area.
*/ /*
void MosaicPanel::show()
{
        repaint();
}
 */
/*
        This function get called from save() of MainWindow
        It will save the resulting mosaic given the path from MainWindow.
        A confirmation message will be displayed if it is successfully saved or not.
*/
void MosaicPanel::saveImage (const QString &fileName)
{
        if(mosaic -> save(fileName, "jpg"))
                QMessageBox::information(this, tr("Save"), tr("Your file %1 have been successfully saved").arg(fileName.section('/', -1)));
        else
                QMessageBox::information(this, tr("Save"), tr("Could not save file %1").arg(fileName.section('/', -1)));
}

//      Paint the mosaic.
void MosaicPanel::paintEvent(QPaintEvent *e)
{
  QPainter painter(this);
  painter.setBackgroundMode(Qt::OpaqueMode);
  painter.setBackground(QBrush(Qt::black));
  painter.drawImage (_px, _py, *mosaic);
}

//      Stores the current coordinates when mouse pressed.
void MosaicPanel::mousePressEvent(QMouseEvent* e)
{
  _pressed = true;
  _x = e->x(); _y = e->y();
}

//      Set the _pressed state to false when user releases the mouse key.
void MosaicPanel::mouseReleaseEvent(QMouseEvent*)
{
  _pressed = false;
}

// When mouse key pressed, move image 2-pixels at a time and then repaint it.
void MosaicPanel::mouseMoveEvent(QMouseEvent* e)
{
          int x = e->x();
          int y = e->y();

  if (_pressed)
  {
          if (x > this -> width() || x < 0 || y > this -> height() || y < 0)
                return;

          int dx = x - _x;
          int dy = y - _y;

          if(dx < 0) {
                dx += (x%2);
                x += (x%2);
          } else {
                dx -= (x%2);
                x -= (x%2);
          }

          if(dy < 0) {
                dy += (y%2);
                y += (y%2);
          } else {
                dy -= (y%2);
                y -= (y%2);
          }

          _x = x;
          _y = y;
          _px += dx;
          _py += dy;
          update();
  }
}
// Wai Khoo
