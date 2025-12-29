#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "gwidget.h"
#include "mouse.h"
#include <QMainWindow>
#include <QACtion>
#include <QMenu>
#include <QToolBar>
#include <QImage>
#include <QLabel>
//---------------------------------------------------------
#include <QPoint>
#include <QRect>
#include <QPainter>
//---------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createActions1();
    void createActions2();
    void createMenus1();
    void createMenus2();
    void createToolBars();
    void loadFile (QString filename);
private slots:
    void showOpenFile();
    void b();
    void s();
    void showGeometryTransform();
protected:
    void mouseMoveEvent (QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent (QMouseEvent * event);
    //---------------------------------------------------------
    void paintEvent(QPaintEvent * event);
    //---------------------------------------------------------------
private:
    Widget *gWin;
    QWidget *central;
    QMenu *fileMenu;
    QToolBar *fileTool;
    QImage img;
    QString filename;
    QLabel *imgwin;
    QAction *openFileAction;
    QAction *exitAction;
    QAction *big;
    QAction *small;
    QAction *geometryAction;
    QLabel *statusLabel;
    QLabel *MousePosLabel;
    int x=300;
    int y=200;
    //---------------------------------------------------------
    bool isSelecting;
    QPoint selectionStart;
    QPoint selectionEnd;
    QRect selectionRect;
    //---------------------------------------------------------------
};
#endif // MAINWINDOW_H
