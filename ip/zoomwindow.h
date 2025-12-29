#ifndef ZOOMWINDOW_H
#define ZOOMWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QToolBar>
#include <QAction>
#include <QColor>
#include <QPoint>

class ZoomWindow : public QMainWindow
{
    Q_OBJECT

public:
    ZoomWindow(const QImage &image, QWidget *parent = nullptr);
    ~ZoomWindow();

private slots:
    void saveImageAs();
    void enableBrush();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QImage originalImage;
    QImage drawingImage;
    QLabel *imageLabel;
    QPushButton *saveButton;
    QPushButton *brushButton;
    QToolBar *toolbar;
    QAction *saveAction;
    QAction *brushAction;
    
    bool brushEnabled;
    bool isDrawing;
    QPoint lastPoint;
    QColor brushColor;
    int brushSize;
};

#endif // ZOOMWINDOW_H
