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
#include <QWidget>
//---------------------------------------------------------------

//---------------------------------------------------------
// Overlay widget for drawing selection rectangle
class SelectionOverlay : public QWidget
{
    Q_OBJECT
public:
    SelectionOverlay(QWidget *parent = nullptr) : QWidget(parent) 
    {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_TranslucentBackground);
    }
    
    void setSelectionRect(const QRect &rect, bool active)
    {
        selectionRect = rect;
        isActive = active;
        update();
    }
    
protected:
    void paintEvent(QPaintEvent *event) override
    {
        if (isActive && !selectionRect.isNull())
        {
            QPainter painter(this);
            painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
            painter.drawRect(selectionRect);
        }
    }
    
private:
    QRect selectionRect;
    bool isActive = false;
};
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
    void resizeEvent(QResizeEvent * event);
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
    SelectionOverlay *overlay;
    //---------------------------------------------------------------
};
#endif // MAINWINDOW_H
