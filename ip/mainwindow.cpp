#include "mainwindow.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
//---------------------------------------------------------
#include <QResizeEvent>
//---------------------------------------------------------------

#include <mouse.h>
#include <gwidget.h>

//---------------------------------------------------------
// 建構函式：isZoomedWindow 參數用於區分主視窗和放大後的子視窗
MainWindow::MainWindow(QWidget *parent, bool isZoomedWindow)
    : QMainWindow(parent), isZoomedWindow(isZoomedWindow) // 初始化成員變數
//---------------------------------------------------------------
{
    //---------------------------------------------------------
    isSelecting = false;
    overlay = nullptr;
    saveAsAction = nullptr; // 初始化為 nullptr，只在放大視窗中使用
    //---------------------------------------------------------------
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
    //---------------------------------------------------------
    // 只在主視窗建立覆蓋層，放大視窗不需要選取功能
    if (!isZoomedWindow)
    {
        // Create overlay for selection rectangle on top of central widget
        overlay = new SelectionOverlay(central);
        overlay->setGeometry(0, 0, central->width(), central->height());
        overlay->raise(); // Make sure overlay is on top
        overlay->show();
    }
    
    // 根據視窗類型建立不同的功能
    if (isZoomedWindow)
    {
        // 放大視窗只需要另存新檔功能
        createZoomedWindowActions();
        createZoomedWindowMenus();
    }
    else
    {
        // 主視窗建立完整功能
        createActions1();
        createActions2();
        createMenus1();
        createMenus2();
        createToolBars();
        b();
        s();
    }
    //---------------------------------------------------------------
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

//---------------------------------------------------------
// 建立放大視窗專用的動作（僅另存新檔和結束）
void MainWindow::createZoomedWindowActions()
{
    // 建立另存新檔動作
    saveAsAction = new QAction (QStringLiteral("另存新檔(&A)"),this);
    saveAsAction->setShortcut (tr("Ctrl+S"));
    saveAsAction->setStatusTip (QStringLiteral("將圖片另存為新檔案"));
    connect (saveAsAction, SIGNAL (triggered()), this, SLOT (saveAsImage()));
    
    // 建立結束動作
    exitAction = new QAction (QStringLiteral("關閉(&Q)"),this);
    exitAction->setShortcut (tr("Ctrl+Q"));
    exitAction->setStatusTip (QStringLiteral("關閉視窗"));
    connect (exitAction, SIGNAL (triggered()), this, SLOT (close()));
}

// 建立放大視窗專用的選單（僅檔案選單）
void MainWindow::createZoomedWindowMenus()
{
    fileMenu = menuBar ()->addMenu (QStringLiteral("檔案&F"));
    fileMenu->addAction(saveAsAction); // 另存新檔
    fileMenu->addAction(exitAction);    // 關閉視窗
}
//---------------------------------------------------------------

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

//---------------------------------------------------------
// 另存圖片功能實作
void MainWindow::saveAsImage()
{
    // 檢查是否有載入的圖片
    if (img.isNull())
    {
        statusBar()->showMessage(QStringLiteral("沒有可儲存的圖片"), 3000);
        return;
    }
    
    // 開啟檔案對話框讓使用者選擇儲存位置和檔名
    QString filePath = QFileDialog::getSaveFileName(this, 
                                                     QStringLiteral("另存新檔"), 
                                                     "", 
                                                     "PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp)");
    
    // 如果使用者有選擇檔案路徑，則儲存圖片
    if (!filePath.isEmpty()) 
    {
        if (img.save(filePath))
        {
            statusBar()->showMessage(QStringLiteral("檔案已成功儲存至: ") + filePath, 3000);
        }
        else
        {
            statusBar()->showMessage(QStringLiteral("儲存失敗"), 3000);
        }
    }
}
//---------------------------------------------------------------

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
    //---------------------------------------------------------
    // 只在主視窗處理選取功能
    if (!isZoomedWindow && isSelecting)
    {
        selectionEnd = event->pos();
        selectionRect = QRect(selectionStart, selectionEnd).normalized();
        
        // Convert MainWindow coordinates to central widget coordinates
        QPoint centralTopLeft = central->mapFrom(this, selectionRect.topLeft());
        QPoint centralBottomRight = central->mapFrom(this, selectionRect.bottomRight());
        QRect centralRect(centralTopLeft, centralBottomRight);
        
        overlay->setSelectionRect(centralRect, true);
    }
    //---------------------------------------------------------------
}

void MainWindow::mousePressEvent(QMouseEvent * event)
{
    QString str = "(" + QString::number (event->x()) + ", " +QString::number (event->y()) +")";
    if (event->button() == Qt::LeftButton)
    {
        statusBar()->showMessage(QStringLiteral("左鍵:")+str);
        //---------------------------------------------------------
        // 只在主視窗啟用選取功能
        if (!isZoomedWindow && !img.isNull())
        {
            isSelecting = true;
            selectionStart = event->pos();
            selectionEnd = event->pos();
            selectionRect = QRect();
        }
        //---------------------------------------------------------------
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
    //---------------------------------------------------------
    // 只在主視窗處理放大功能
    if (!isZoomedWindow && isSelecting && event->button() == Qt::LeftButton)
    {
        isSelecting = false;
        
        if (selectionRect.width() > 10 && selectionRect.height() > 10 && !img.isNull())
        {
            QRect imgGeometry = imgwin->geometry();
            
            int menuHeight = menuBar()->height();
            int toolbarHeight = (fileTool ? fileTool->height() : 0);
            int statusHeight = statusBar()->height();
            
            int selX = selectionRect.x() - imgGeometry.x();
            int selY = selectionRect.y() - imgGeometry.y() - menuHeight - toolbarHeight - statusHeight;
            int selW = selectionRect.width();
            int selH = selectionRect.height();
            
            if (imgwin->width() > 0 && imgwin->height() > 0 && 
                selX >= 0 && selY >= 0 && selX + selW <= imgGeometry.width() && selY + selH <= imgGeometry.height())
            {
                double scaleX = (double)img.width() / imgwin->width();
                double scaleY = (double)img.height() / imgwin->height();
                
                int imgX = selX * scaleX;
                int imgY = selY * scaleY;
                int imgW = selW * scaleX;
                int imgH = selH * scaleY;
                
                QImage croppedImg = img.copy(imgX, imgY, imgW, imgH);
                
                QImage zoomedImg = croppedImg.scaled(imgW * 2, imgH * 2, Qt::KeepAspectRatio, Qt::FastTransformation);
                
                //---------------------------------------------------------
                // 建立放大視窗，傳入 true 表示這是放大後的子視窗
                MainWindow *newIPWin = new MainWindow(nullptr, true);
                //---------------------------------------------------------------
                newIPWin->setAttribute(Qt::WA_DeleteOnClose);
                newIPWin->img = zoomedImg;
                newIPWin->imgwin->setPixmap(QPixmap::fromImage(zoomedImg));
                //---------------------------------------------------------
                newIPWin->setWindowTitle(QStringLiteral("放大視窗 - 2x")); // 設定視窗標題
                //---------------------------------------------------------------
                newIPWin->show();
            }
        }
        
        selectionRect = QRect();
        //---------------------------------------------------------
        overlay->setSelectionRect(QRect(), false);
        //---------------------------------------------------------------
        update();
    }
    //---------------------------------------------------------------
}

void MainWindow::b(){
     imgwin->resize(imgwin->width() * 1.25, imgwin->height() * 1.25);
};
void MainWindow::s(){
     imgwin->resize(imgwin->width() * 0.8, imgwin->height() * 0.8);
};

//---------------------------------------------------------
void MainWindow::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);
    // 只在主視窗更新覆蓋層
    if (!isZoomedWindow && overlay)
    {
        // Update overlay geometry to match central widget
        overlay->setGeometry(0, 0, central->width(), central->height());
    }
}
//---------------------------------------------------------------

