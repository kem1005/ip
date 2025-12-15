#include "mainwindow.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("影像處理"));
    central =new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout (central);
    imgwin = new QLabel();
    QPixmap
        *initPixmap = new QPixmap(300,200);
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
void MainWindow::b(){
     imgwin->resize(imgwin->width() * 1.25, imgwin->height() * 1.25);
};
void MainWindow::s(){
     imgwin->resize(imgwin->width() * 0.8, imgwin->height() * 0.8);
};


