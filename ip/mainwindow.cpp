#include "mainwindow.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>

#include <mouse.h>
#include <gwidget.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), rubberBand(nullptr), isSelecting(false)
{
    statusLabel = new QLabel;
    statusLabel->setText (QStringLiteral("指標位置"));
    statusLabel->setFixedWidth (100);
    MousePosLabel = new QLabel;
    MousePosLabel->setText(tr(" "));
    MousePosLabel->setFixedWidth (100);
    statusBar()->addPermanentWidget (statusLabel);
    statusBar()->addPermanentWidget (MousePosLabel);
    setMouseTracking (true);

    resize (400,300);
    central =new QWidget();
    central->setMouseTracking(true);
    QHBoxLayout *mainLayout = new QHBoxLayout (central);
    imgwin = new QLabel();
    imgwin->setMouseTracking(true);
    QPixmap
        *initPixmap = new QPixmap(300,200);
    gWin =new Widget();
    initPixmap->fill (QColor(255,255,255));
    imgwin->resize (300,200);
    imgwin->setScaledContents(true);
    imgwin->setPixmap (*initPixmap);
    mainLayout->addWidget (imgwin);
    setCentralWidget (central);
    createActions1();
    createActions2();
    createMenus1();
    createMenus2();
    createToolBars();
    b();
    s();
}

MainWindow::~MainWindow() {

}
void MainWindow::createActions1()
{

    openFileAction = new QAction (QStringLiteral("開啟檔案(&0)"),this);
    openFileAction->setShortcut (tr("Ctrl+0"));
    openFileAction->setStatusTip (QStringLiteral("開啟影像檔案"));
    connect (openFileAction, SIGNAL (triggered()), this, SLOT (showOpenFile()));

    exitAction = new QAction (QStringLiteral("結束(&Q)"),this);
    exitAction->setShortcut (tr("Ctrl+Q"));
    exitAction->setStatusTip (QStringLiteral("退出程式"));
    connect (exitAction, SIGNAL (triggered()), this, SLOT (close()));
    connect (exitAction,SIGNAL(triggered()),imgwin,SLOT (close()));

    geometryAction = new QAction (QStringLiteral("幾何轉換"),this);
    geometryAction->setShortcut (tr("Ctrl+G"));
    geometryAction->setStatusTip (QStringLiteral("影像幾何轉換"));
    connect (geometryAction, SIGNAL (triggered()), this, SLOT (showGeometryTransform()));
    connect (exitAction, SIGNAL (triggered()),gWin, SLOT (close()));
}

void MainWindow::createActions2()
{

    big = new QAction (QStringLiteral("放大(&B)"),this);
    big->setShortcut (tr("Ctrl++"));
    openFileAction->setStatusTip (QStringLiteral("大"));
    connect (big, SIGNAL (triggered()), this, SLOT (b()));

    small = new QAction (QStringLiteral("縮小(&S)"),this);
    small->setShortcut (tr("Ctrl+-"));
    openFileAction->setStatusTip (QStringLiteral("小"));
    connect (small, SIGNAL (triggered()), this, SLOT (s()));


}
void MainWindow::createMenus1()
{
    fileMenu = menuBar ()->addMenu (QStringLiteral("檔案&F"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(geometryAction);
    fileMenu->addAction(exitAction);
}
void MainWindow::createMenus2()
{
    fileMenu = menuBar ()->addMenu (QStringLiteral("工具&P"));
    fileMenu->addAction(big);
    fileMenu->addAction(small);
}
void MainWindow::createToolBars()
{
    fileTool = addToolBar("file");
    fileTool->addAction (openFileAction);
    fileTool->addAction (geometryAction);
    fileTool->addAction (big);
    fileTool->addAction (small);
}
void MainWindow::loadFile (QString filename)
{
    qDebug() <<QString("file name: %1").arg(filename);
    QByteArray ba=filename.toLatin1();
    printf("FN:%s\n", (char *) ba.data());
    img.load(filename);
    imgwin->setPixmap (QPixmap:: fromImage (img));
}
void MainWindow::showOpenFile()
{
filename = QFileDialog::getOpenFileName(this,
                    QStringLiteral("開啟影像"),
                    tr("."),
                    "bmp(*.bmp);;png(*.png)"
                    ";;Jpeg(*.jpg)");
    if (!filename.isEmpty())
    {
        if(img.isNull())
        {
            loadFile(filename);
        }
        else
        {
            MainWindow *newIPWin = new MainWindow();
            newIPWin->show();
            newIPWin->loadFile(filename);
        }
    }
}
void MainWindow:: showGeometryTransform()
{
    if (!img.isNull())
        gWin->srcImg = img;
    gWin->inWin->setPixmap (QPixmap:: fromImage (gWin->srcImg));
    gWin->show();
}

void MainWindow::mouseMoveEvent (QMouseEvent * event)
{
    int x = event->x();
    int y = event->y();
    QString str = "(" + QString::number (event->x()) + ", " +QString::number (event->y()) +")";
    if(!img.isNull() && x>=0 && x< imgwin->width()&&y>=0 &&y< imgwin->height())
    {
        int gray = qGray(img.pixel(x,y));
        str += ("=" +QString::number(gray));
    }
    MousePosLabel->setText(str);
    
    // Update rubber band during selection
    if (isSelecting && rubberBand) {
        rubberBand->setGeometry(QRect(selectionOrigin, event->pos()).normalized());
    }
}

void MainWindow::mousePressEvent(QMouseEvent * event)
{
    QString str = "(" + QString::number (event->x()) + ", " +QString::number (event->y()) +")";
    if (event->button() == Qt::LeftButton)
    {
        statusBar()->showMessage(QStringLiteral("左鍵:")+str);
        
        // Start selection if clicking on image area
        QPoint imgPos = imgwin->mapFrom(this, event->pos());
        if (!img.isNull() && imgwin->geometry().contains(event->pos())) {
            isSelecting = true;
            selectionOrigin = event->pos();
            if (!rubberBand) {
                rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
            }
            rubberBand->setGeometry(QRect(selectionOrigin, QSize()));
            rubberBand->show();
        }
    }
    else if (event->button()== Qt::RightButton)
    {
        statusBar ()->showMessage (QStringLiteral ("右鍵:")+str);
    }
    else if (event->button()== Qt::MiddleButton)
    {
        statusBar ()->showMessage(QStringLiteral("中鍵:")+str);
    }
}
void MainWindow::mouseReleaseEvent (QMouseEvent* event)
{
    QString str = "(" + QString::number (event->x()) + ", " +QString::number (event->y()) +")";
    statusBar ()->showMessage (QStringLiteral("釋放:")+str);
    
    // Handle selection completion
    if (isSelecting && rubberBand && event->button() == Qt::LeftButton) {
        isSelecting = false;
        QRect selection = rubberBand->geometry();
        rubberBand->hide();
        
        // Only open zoom window if selection is large enough
        if (selection.width() > 10 && selection.height() > 10) {
            openZoomWindow();
        }
    }
}

void MainWindow::b(){
     imgwin->resize(imgwin->width() * 1.25, imgwin->height() * 1.25);
};
void MainWindow::s(){
     imgwin->resize(imgwin->width() * 0.8, imgwin->height() * 0.8);
};

void MainWindow::openZoomWindow()
{
    if (img.isNull() || !rubberBand) {
        return;
    }
    
    // Get the selection rectangle
    QRect selection = rubberBand->geometry();
    
    // Convert selection coordinates to image coordinates
    QPoint imgTopLeft = imgwin->mapFrom(this, selection.topLeft());
    QPoint imgBottomRight = imgwin->mapFrom(this, selection.bottomRight());
    
    // Calculate scaling factor from displayed image to actual image
    double scaleX = (double)img.width() / imgwin->width();
    double scaleY = (double)img.height() / imgwin->height();
    
    // Map to actual image coordinates
    int x1 = qMax(0, qMin((int)(imgTopLeft.x() * scaleX), img.width() - 1));
    int y1 = qMax(0, qMin((int)(imgTopLeft.y() * scaleY), img.height() - 1));
    int x2 = qMax(0, qMin((int)(imgBottomRight.x() * scaleX), img.width()));
    int y2 = qMax(0, qMin((int)(imgBottomRight.y() * scaleY), img.height()));
    
    int width = x2 - x1;
    int height = y2 - y1;
    
    if (width <= 0 || height <= 0) {
        return;
    }
    
    // Extract the selected region
    QImage selectedRegion = img.copy(x1, y1, width, height);
    
    // Scale by 2x
    QImage zoomedImage = selectedRegion.scaled(
        width * 2, 
        height * 2, 
        Qt::IgnoreAspectRatio, 
        Qt::SmoothTransformation
    );
    
    // Create and show zoom window
    ZoomWindow *zoomWin = new ZoomWindow(zoomedImage, this);
    zoomWin->show();
}

