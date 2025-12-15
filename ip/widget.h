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
};
#endif // WIDGET_H
