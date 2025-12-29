#ifndef MOUSE_H
#define MOUSE_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QStatusBar>
#include <QLabel>
class mouse : public QMainWindow
{
    Q_OBJECT

public:
    mouse(QWidget *parent = nullptr);
    ~mouse();
protected:
    void mouseDoubleClickEvent(QMouseEvent * event);
    void mouseMoveEvent (QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent (QMouseEvent * event);
private:
    QLabel *statusLabel;
    QLabel *MousePosLabel;
};
#endif // MOUSE_H
