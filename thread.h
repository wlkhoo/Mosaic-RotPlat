#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QtGui>

class Thread : public QThread
{
	Q_OBJECT

public:
	Thread(QObject *parent = 0);
	~Thread();
	void render(QImage& img, int w1, int h1, int w2, int h2);

signals:
	void renderedImage(QImage image1, QImage image2, QImage image3, int w1, int h1, int w2, int h2);

protected:
	void run();

private:
	QMutex mutex;
	QWaitCondition condition;

	int x1;
	int y1;
	int x2;
	int y2;

	QImage image;

	QRgb l_color;
	QRgb m_color;
	QRgb r_color;

};
#endif
