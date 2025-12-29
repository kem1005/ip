#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QDial>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QImage>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QLabel *inWin;
    QGroupBox *mirrorGroup;
    QCheckBox *hCheckBox;
    QCheckBox *vCheckBox;
    QPushButton *mirrorButton;
    QPushButton *saveButton;
    QDial *rotateDial;
    QSpacerItem *vSpacer;
    QHBoxLayout *mainLayout;
    QVBoxLayout *groupLayout;
    QVBoxLayout *leftLayout;
    QImage srcImg;
    QImage dstImg;


private slots:
    void mirroredImage();
    void rotatedImage();
    void saveImage();
};
#endif // WIDGET_H
