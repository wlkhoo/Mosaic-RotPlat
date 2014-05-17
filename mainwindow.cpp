/*
        MainWindow.cpp written by Wai Khoo <wlkhoo@gmail.com>
        Implementation of the class MainWindow.
        To create a better resulting mosaic, the input images have to be within the time frame when the camera is rotating
                otherwise, you will see stretched portion of the image (indicating camera is idle)
        This particular class doesn't deal with vertical motion resulting from the camera in the input images.
*/
#include <QtGui>
#include "mainwindow.h"
#include "MosaicPanel.h"
#include "thread.h"

// Constructor.
MainWindow::MainWindow()
{
// Initialize image and mosaics.
        image = new QImage();
        l_image = new QImage (3, 240, QImage::Format_RGB32);
        m_image = new QImage (3, 240, QImage::Format_RGB32);
        r_image = new QImage (3, 240, QImage::Format_RGB32);


// Creating three display area.
        panel_l = new MosaicPanel;
        panel_m = new MosaicPanel;
        panel_r = new MosaicPanel;

// Creating label to indicate what each display area is displaying.
        l_column = new QLabel (tr("Left Column"), this);
        m_column = new QLabel (tr("Middle Column"), this);
        r_column = new QLabel (tr("Right Column"), this);

        l_column -> setWordWrap (true);
        m_column -> setWordWrap (true);
        r_column -> setWordWrap (true);

        l_column -> setMinimumWidth (40);
        m_column -> setMinimumWidth (40);
        r_column -> setMinimumWidth (40);

        l_column -> setMaximumHeight (100);
        l_column -> setMaximumWidth (30);
        m_column -> setMaximumHeight (100);
        m_column -> setMaximumWidth (30);
        r_column -> setMaximumHeight (100);
        r_column -> setMaximumWidth (30);

        QWidget *w = new QWidget;
        QGridLayout *layout = new QGridLayout;
        layout -> addWidget (l_column, 0, 0);
        layout -> addWidget (panel_l, 0, 1);
        layout -> addWidget (m_column, 1, 0);
        layout -> addWidget (panel_m, 1, 1);
        layout -> addWidget (r_column, 2, 0);
        layout -> addWidget (panel_r, 2, 1);
        layout -> setColumnMinimumWidth (1, 700);
        layout -> setRowMinimumHeight (0, 230);
        layout -> setRowMinimumHeight (1, 230);
        layout -> setRowMinimumHeight (2, 230);

        createActions();
        createMenus();

        setWindowTitle (tr("Mosaic Viewer"));
        resize (900, 690);

        w -> setLayout (layout);
        setCentralWidget (w);

        qRegisterMetaType<QImage>("QImage");
        connect (&myThread, SIGNAL(renderedImage(QImage, QImage, QImage, int, int, int, int)), this, SLOT(receiver(QImage, QImage, QImage, int, int, int, int)));
/*
        Initialize variables that are used to tell MosaicPanel where to start painting the mosaic in
                the display area.
*/
        lr_x = lr_y = m_x = m_y = 0;
}

MainWindow::~MainWindow()
{
        // Destructor with nothing in particular to perform further.
}

void MainWindow::receiver(QImage img1, QImage img2, QImage img3, int w1, int h1, int w2, int h2)
{
        panel_l -> receive (img1, w1, h1);
        panel_m -> receive (img2, w2, h2);
        panel_r -> receive (img3, w1, h1);
}

/*
        A function that asks the user to open multiple files to be processed.
*/
void MainWindow::open()
{
        QStringList fileName = QFileDialog::getOpenFileNames(this, tr("Select Files"), QDir::currentPath());

        /*
                The resulting mosaic's width can be determined by knowing how many files the user opened.
                Since two of the columns of the video (left and right) are inverted, the corresponding mosaics
                        need to be painted in opposite direction with respect of the middle column.
        */
        lr_x = (fileName.size() - 1) * 2 + 3;
        lr_y = m_x = m_y = 0;
        int prog = 0;   // Keep track of how many file has been read; use along with progress bar.

/*
        open() has figured out what the resulting mosaic's width should be (lr_x) and tell MosaicPanel to
                create a blank with the given width.  The height is hard-coded in MosaicPanel.
*/
        panel_l -> initialize (lr_x);
        panel_m -> initialize (lr_x);
        panel_r -> initialize (lr_x);

/*
        Since constructing numerous mosaics take time, a progress bar is displayed as to ensure the user
                that the program is running and at which frame (file) is it processing.
*/
        QProgressDialog constructing (this);
        constructing.setCancelButtonText (tr("&Abort"));
        constructing.setRange (0, fileName.size());
        constructing.setWindowTitle (tr("Constructing Mosaics"));


        QStringListIterator i(fileName);
//      Traverse the list of file names specified by the user.
        while (i.hasNext())
        {
                if (!i.hasNext())
                {// Display a message to user to indicate that the current file cannot be opened.
                        QMessageBox::information(this, tr("Open"), tr("Cannot open %1.").arg(i.next().toLocal8Bit().constData()));
                        return;
                }
                else
                {       // Update the progress bar.
                        constructing.setValue (prog);
                        constructing.setLabelText(tr("Constructing %1 of %2 frame").arg(prog).arg(fileName.size()));
                        qApp -> processEvents();
                        if (constructing.wasCanceled())
                        {       // If user abort the mosaic construction process, the whole program is aborted.
                                QMessageBox::warning(this, tr("Abort"), tr("Process aborted! Application will be closed"));
                                QMessageBox::warning(this, tr("Abort"), tr("Process aborted!"));
//                                this -> close();
                                break;
                        }
                        // Read the file and store the image.
                        image = new QImage (i.next().toLocal8Bit().constData());
                        // Process the given image.
 //                       mosaicing();
                        myThread.render(*image, lr_x, lr_y, m_x, m_y);
                        /*
                                mosaicing() should generated 3 (much) smaller images that should be passed to
                                        MosaicPanel::receive().
                        */
 /*                       panel_l -> receive (*l_image, lr_x, lr_y);
                        panel_m -> receive (*m_image, m_x, m_y);
                        panel_r -> receive (*r_image, lr_x, lr_y);
 */                       /*
                                Update the next position for which the mosaic is going to be painted on.
                                        Y-coordinate stays the same.
                        */
                        m_x += 2;
                        lr_x -= 2;
                }
                prog++;
        }
        constructing.setValue (fileName.size());
        // Mosaics are constructed and should be displayed corresponding.
/*        panel_l -> show();
        panel_m -> show();
        panel_r -> show();
*/        saveAct -> setEnabled (true);
}

/*
        A function to get the file name (the title name) that the user wants to save the image in.
        Since there's three images to be save, this function appends (left, middle, right) to the title
                name given by the user.  Therefore, the user only need to provide one name and all three images
                will be saved.
        Also, since the program can only read in JPG files, the user can only save the resulting images in
                JPG format (which is hard-coded).
*/
void MainWindow::save()
{
        QString saveFile = QFileDialog::getSaveFileName (this, tr("Save images as (title name) in JPG"), QDir::currentPath() + "/untitled", tr("Images (*.jpg)"));
        QString name = saveFile;
        // String to append to the title name corresponding to the images.
        QString one = "_left";
        QString two = "_middle";
        QString three = "_right";

        // The true paths that the images are going to be saved in.
        QString save1, save2, save3;

// User sometimes might or might not provided the extension of the file
        if (name.endsWith (".jpg"))
        {// If the user did provided the extension, append the string just before ".jpg".
                int pos = name.indexOf (".");
                save1 = name.insert (pos-1, one);
                name = saveFile;
                save2 = name.insert (pos-1, two);
                name = saveFile;
                save3 = name.insert (pos-1, three);
                name = saveFile;
        }
        else
        {// If the user didn't provided the extension, just append the string at the end and add the extension.
                save1 = name.append(one + ".jpg");
                name = saveFile;
                save2 = name.append(two + ".jpg");
                name = saveFile;
                save3 = name.append(three + ".jpg");
                name = saveFile;
        }
        // call MosaicPanel::saveImage() to save the corresponding images with the corresponding true paths.
        panel_l -> saveImage (save1);
        panel_m -> saveImage (save2);
        panel_r -> saveImage (save3);
}

// Set main window to full screen.
void MainWindow::fullScreen()
{
        this -> setWindowState (this -> windowState() ^ Qt::WindowFullScreen);
}

/*
        Function that process each frame (file).
        Since each frame has three views (two are inverted), this function will take 3 columns X height from each views,
                overlapping each frames by 1 column (stitching).
*/
/*void MainWindow::mosaicing()
{
        int l_x, l_y, m_x, m_y, r_x, r_y;
        l_x = l_y = m_x = m_y = r_x = r_y = 0;
*/
        /*
                Traverse each pixel in the original frame.
                the dimension is obtained through third party tool (Bink Video)
        */
/*      for (int y = 0; y < 240; y++)
        {
                for (int x = 0; x < 320; x++)
                {
                        if ((x >= 52) && (x <= 54))             // 3 columns within the left view
                        {
                                l_color = image -> pixel (x, y);
                                l_image -> setPixel (l_x, l_y, l_color);  //Copy pixel from original frame to l_image at l_x, l_y.
                                l_x++;
                                if (l_x % 3 == 0)
                                {// Creating 3 columns image.
                                        l_x = 0;
                                        l_y++;
                                }
                        }
                        else if ((x >= 158) && (x <= 160))              // 3 columns within the middle view.
                        {
                                m_color = image -> pixel (x, y);
                                m_image -> setPixel (m_x, m_y, m_color);
                                m_x++;
                                if (m_x % 3 == 0)
                                {
                                        m_x = 0;
                                        m_y++;
                                }
                        }
                        else if ((x >= 264) && (x <= 266))              //3 columns within the right view.
                        {
                                r_color = image -> pixel (x, y);
                                r_image -> setPixel (r_x, r_y, r_color);
                                r_x++;
                                if (r_x % 3 == 0)
                                {
                                        r_x = 0;
                                        r_y++;
                                }
                        }
                }
        }
}
 */
// Below functions create basic functionality of the program's window.

void MainWindow::createActions()
{
        mosaicAct = new QAction (tr("Create &Mosaic"), this);
        mosaicAct -> setShortcut (tr("Ctrl+M"));
        connect (mosaicAct, SIGNAL (triggered()), this, SLOT (open()));

        saveAct = new QAction (tr("&Save"), this);
        saveAct -> setShortcut (tr("Ctrl+S"));
        saveAct -> setEnabled (false);
        connect (saveAct, SIGNAL (triggered()), this, SLOT (save()));

        exitAct = new QAction (tr("&Exit"), this);
        exitAct -> setShortcut (tr("Ctrl+Q"));
        connect (exitAct, SIGNAL (triggered()), this, SLOT (close()));

        fullScreenAct = new QAction (tr("&Full Screen"), this);
        fullScreenAct -> setShortcut (tr("Ctrl+F"));
        connect (fullScreenAct, SIGNAL (triggered()), this, SLOT (fullScreen()));
}

void MainWindow::createMenus()
{
        fileMenu = new QMenu (tr("&File"), this);
        fileMenu -> addAction (mosaicAct);
        fileMenu -> addAction (saveAct);
        fileMenu -> addAction (fullScreenAct);
        fileMenu -> addSeparator();
        fileMenu -> addAction (exitAct);

        menuBar() -> addMenu (fileMenu);
}
// Wai Khoo
