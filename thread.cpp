#include <QtGui>
#include "thread.h"

Thread::Thread(QObject *parent)
	: QThread(parent)
{
}

Thread::~Thread()
{
	mutex.lock();
	condition.wakeOne();
	mutex.unlock();
	wait();
}

void Thread::render(QImage& img, int w1, int h1, int w2, int h2)
{
	QMutexLocker locker(&mutex);

	x1 = w1;
	y1 = h1;
	x2 = w2;
	y2 = h2;
	image = img;

	if (!isRunning())
		start(NormalPriority);
	else
		condition.wakeOne();
}

 void Thread::run()
 {
     int l_x, l_y, m_x, m_y, r_x, r_y;
     l_x = l_y = m_x = m_y = r_x = r_y = 0;

     QImage l_image(3, 240, QImage::Format_RGB32);
     QImage m_image(3, 240, QImage::Format_RGB32);
     QImage r_image(3, 240, QImage::Format_RGB32);
             /*
                     Traverse each pixel in the original frame.
                     the dimension is obtained through third party tool (Bink Video)
             */
	for (int y = 0; y < 240; y++)
	 {
		 for (int x = 0; x < 320; x++)
		 {
			 if ((x >= 52) && (x <= 54))             // 3 columns within the left view
			 {
				 l_color = image.pixel (x, y);
				 l_image.setPixel (l_x, l_y, l_color);  //Copy pixel from original frame to l_image at l_x, l_y.
				 l_x++;
				 if (l_x % 3 == 0)
				 {// Creating 3 columns image.
					 l_x = 0;
					 l_y++;
				 }
			 }
			 else if ((x >= 158) && (x <= 160))              // 3 columns within the middle view.
			 {
				 m_color = image.pixel (x, y);
				 m_image.setPixel (m_x, m_y, m_color);
				 m_x++;
				 if (m_x % 3 == 0)
				 {
					 m_x = 0;
					 m_y++;
				 }
			 }
			 else if ((x >= 264) && (x <= 266))              //3 columns within the right view.
			 {
				 r_color = image.pixel (x, y);
				 r_image.setPixel (r_x, r_y, r_color);
				 r_x++;
				 if (r_x % 3 == 0)
				 {
					 r_x = 0;
					 r_y++;
				 }
			 }
		 }
	}
	emit renderedImage(l_image, m_image, r_image, x1, y1, x2, y2);
 }
