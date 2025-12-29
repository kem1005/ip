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
        Q_UNUSED(event);
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
    //---------------------------------------------------------
    MainWindow(QWidget *parent = nullptr, bool isZoomedWindow = false); // 增加參數區分主視窗和放大視窗
    //---------------------------------------------------------------
    ~MainWindow();
    void createActions1();
    void createActions2();
    void createMenus1();
    void createMenus2();
    void createToolBars();
    //---------------------------------------------------------
    void createZoomedWindowActions(); // 建立放大視窗的動作（僅另存新檔）
    void createZoomedWindowMenus(); // 建立放大視窗的選單（僅檔案選單）
    //---------------------------------------------------------------
    void loadFile (QString filename);
private slots:
    void showOpenFile();
    void b();
    void s();
    void showGeometryTransform();
    //---------------------------------------------------------
    void saveAsImage(); // 另存圖片功能
    void setPenColor(); // 設定畫筆顏色
    //---------------------------------------------------------------
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
    //---------------------------------------------------------
    QAction *saveAsAction; // 另存新檔動作
    QAction *penColorAction; // 畫筆顏色選單動作
    QAction *redPenAction; // 紅色畫筆
    QAction *bluePenAction; // 藍色畫筆
    QAction *greenPenAction; // 綠色畫筆
    QAction *blackPenAction; // 黑色畫筆
    QAction *yellowPenAction; // 黃色畫筆
    QAction *whitePenAction; // 白色畫筆
    //---------------------------------------------------------------
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
    bool isZoomedWindow; // 標記是否為放大的子視窗
    //---------------------------------------------------------------
    //---------------------------------------------------------
    // 畫筆繪圖相關變數（僅用於放大視窗）
    bool isDrawing; // 是否正在繪圖
    QPoint lastDrawPoint; // 上一個繪圖點
    QColor penColor; // 畫筆顏色
    int penWidth; // 畫筆寬度
    //---------------------------------------------------------------
};
#endif // MAINWINDOW_H
