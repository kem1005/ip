#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QACtion>
#include <QMenu>
#include <QToolBar>
#include <QImage>
#include <QLabel>
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
private:
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
};
#endif // MAINWINDOW_H
