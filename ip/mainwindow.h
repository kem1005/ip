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
    void createActions();
    void createMenus();
    void createToolBars();
    void loadFile (QString filename);
private slots:
    void showOpenFile();
private:
    QWidget *central;
    QMenu *fileMenu;
    QToolBar *fileTool;
    QImage img;
    QString filename;
    QLabel *imgwin;
    QAction *openFileAction;
    QAction *exitAction;
};
#endif // MAINWINDOW_H
