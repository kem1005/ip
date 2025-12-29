#include "zoomwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QMessageBox>
#include <QScrollArea>

ZoomWindow::ZoomWindow(const QImage &image, QWidget *parent)
    : QMainWindow(parent),
      originalImage(image),
      drawingImage(image),
      brushEnabled(false),
      isDrawing(false),
      brushColor(Qt::red),
      brushSize(3)
{
    setWindowTitle(QStringLiteral("放大影像編輯"));
    resize(drawingImage.width() + 50, drawingImage.height() + 100);

    // Create central widget with scroll area
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Create scroll area for image
    QScrollArea *scrollArea = new QScrollArea(this);
    imageLabel = new QLabel(this);
    imageLabel->setPixmap(QPixmap::fromImage(drawingImage));
    imageLabel->setScaledContents(false);
    scrollArea->setWidget(imageLabel);
    scrollArea->setWidgetResizable(false);
    
    mainLayout->addWidget(scrollArea);
    setCentralWidget(centralWidget);

    // Create toolbar
    toolbar = addToolBar(QStringLiteral("工具"));
    
    // Save action
    saveAction = new QAction(QStringLiteral("另存新檔"), this);
    saveAction->setStatusTip(QStringLiteral("儲存影像"));
    connect(saveAction, &QAction::triggered, this, &ZoomWindow::saveImageAs);
    toolbar->addAction(saveAction);
    
    // Brush action
    brushAction = new QAction(QStringLiteral("畫筆"), this);
    brushAction->setCheckable(true);
    brushAction->setStatusTip(QStringLiteral("啟用畫筆工具"));
    connect(brushAction, &QAction::triggered, this, &ZoomWindow::enableBrush);
    toolbar->addAction(brushAction);
    
    statusBar()->showMessage(QStringLiteral("就緒"));
}

ZoomWindow::~ZoomWindow()
{
}

void ZoomWindow::saveImageAs()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        QStringLiteral("另存新檔"),
        "",
        "PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp)"
    );
    
    if (!filePath.isEmpty()) {
        if (drawingImage.save(filePath)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("影像已儲存"));
            statusBar()->showMessage(QStringLiteral("影像已儲存: ") + filePath);
        } else {
            QMessageBox::warning(this, QStringLiteral("錯誤"), QStringLiteral("無法儲存影像"));
        }
    }
}

void ZoomWindow::enableBrush()
{
    brushEnabled = brushAction->isChecked();
    if (brushEnabled) {
        statusBar()->showMessage(QStringLiteral("畫筆已啟用 - 在影像上拖曳以繪圖"));
        setCursor(Qt::CrossCursor);
    } else {
        statusBar()->showMessage(QStringLiteral("畫筆已停用"));
        setCursor(Qt::ArrowCursor);
    }
}

void ZoomWindow::mousePressEvent(QMouseEvent *event)
{
    if (brushEnabled && event->button() == Qt::LeftButton) {
        // Convert global position to image label coordinates
        QPoint labelPos = imageLabel->mapFrom(this, event->pos());
        
        // Check if click is within image bounds
        if (labelPos.x() >= 0 && labelPos.x() < drawingImage.width() &&
            labelPos.y() >= 0 && labelPos.y() < drawingImage.height()) {
            isDrawing = true;
            lastPoint = labelPos;
        }
    }
    QMainWindow::mousePressEvent(event);
}

void ZoomWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (brushEnabled && isDrawing) {
        QPoint labelPos = imageLabel->mapFrom(this, event->pos());
        
        // Check if within bounds
        if (labelPos.x() >= 0 && labelPos.x() < drawingImage.width() &&
            labelPos.y() >= 0 && labelPos.y() < drawingImage.height()) {
            
            // Draw on the image
            QPainter painter(&drawingImage);
            painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(lastPoint, labelPos);
            
            lastPoint = labelPos;
            
            // Update display
            imageLabel->setPixmap(QPixmap::fromImage(drawingImage));
        }
    }
    QMainWindow::mouseMoveEvent(event);
}

void ZoomWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (brushEnabled && event->button() == Qt::LeftButton && isDrawing) {
        isDrawing = false;
    }
    QMainWindow::mouseReleaseEvent(event);
}

void ZoomWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
}
