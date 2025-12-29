#include "gwidget.h"
#include <QPixmap>
#include <QPainter>
#include <QFileDialog>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QHBoxLayout (this);
    leftLayout = new  QVBoxLayout (this);
    mirrorGroup = new QGroupBox (QStringLiteral("鏡射"),this);
    groupLayout = new QVBoxLayout (mirrorGroup);

    hCheckBox = new QCheckBox (QStringLiteral("水平"), mirrorGroup);
    vCheckBox = new QCheckBox (QStringLiteral("垂直"), mirrorGroup);
    mirrorButton = new QPushButton (QStringLiteral("執行"),mirrorGroup);

    hCheckBox->setGeometry (QRect (13, 28, 87, 19));
    vCheckBox->setGeometry (QRect (13, 54, 87, 19));
    mirrorButton->setGeometry (QRect (13, 80, 93, 28));

    groupLayout->addWidget (hCheckBox);
    groupLayout->addWidget (vCheckBox);
    groupLayout->addWidget (mirrorButton);

    leftLayout->addWidget (mirrorGroup);

    rotateDial = new QDial (this);
    rotateDial->setNotchesVisible(true);
    rotateDial->setRange(0,360);
    rotateDial->setWrapping(true);

    vSpacer = new QSpacerItem (20, 58, QSizePolicy:: Minimum,
                              QSizePolicy:: Expanding);
    leftLayout->addWidget (rotateDial);
    leftLayout->addItem(vSpacer);

    saveButton = new QPushButton(QStringLiteral("儲存"), this);
    leftLayout->addWidget(saveButton);

    mainLayout->addLayout (leftLayout);

    inWin = new QLabel (this);
    inWin->setScaledContents (true);
    QPixmap *initPixmap= new QPixmap(300,200);
    initPixmap->fill (QColor (255,255,255));
    //
    QPainter *paint=new QPainter(initPixmap);
    paint->setPen(*(new QColor(0,0,0)));
    paint->begin(initPixmap);
    paint->drawRect(15,15,60,40);
    paint->end();
    if (srcImg.isNull())
    {
        srcImg=initPixmap->toImage();
        mainLayout->addWidget(inWin);
        //
    inWin->setPixmap(*initPixmap);
    inWin->setSizePolicy(QSizePolicy:: Expanding, QSizePolicy:: Expanding);
    if (srcImg.isNull())
    {
        QPixmap *initPixmap= new QPixmap(300,200);
        initPixmap->fill(QColor(255,255,255));
        inWin->setPixmap(*initPixmap);
    }
    mainLayout->addWidget(inWin);
    connect (mirrorButton, SIGNAL (clicked()),this, SLOT (mirroredImage()));
    connect (rotateDial, SIGNAL (valueChanged (int)), this, SLOT (rotatedImage()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveImage()));
    }
}
Widget::~Widget(){}
void Widget::mirroredImage()
{
    bool H,V;
    if (srcImg.isNull())
        return;
    H=hCheckBox->isChecked();
    V=vCheckBox->isChecked();
    dstImg = srcImg.mirrored (H,V);
    inWin->setPixmap (QPixmap:: fromImage (dstImg));
    srcImg = dstImg;
}
void Widget::rotatedImage ()
{
    QTransform tran;
    int angle;
    if (srcImg.isNull())
        return;
    angle = rotateDial->value();
    tran.rotate (angle);
    dstImg = srcImg.transformed (tran);
    inWin->setPixmap (QPixmap:: fromImage (dstImg));
}
void Widget::saveImage()
{
    if (dstImg.isNull())
        return;
    QString filePath = QFileDialog::getSaveFileName(this, QStringLiteral("儲存"), "", "PNG Files (*.png);;JPEG Files (*.jpg)");
    if (!filePath.isEmpty()) {
        dstImg.save(filePath);
    }
}
