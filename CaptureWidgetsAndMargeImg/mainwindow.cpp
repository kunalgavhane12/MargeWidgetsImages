#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect tab change signal to a slot
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    grabAllTabs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTabChanged(int index)
{
    Q_UNUSED(index);
    grabAllTabs();

    // // Capture the current tab's image
    // QWidget *currentTab = ui->tabWidget->widget(index);
    // QPixmap tabImage = currentTab->grab();

    // // Save the image locally with a name based on the tab index
    // QString imageName = QString("tab_%1.png").arg(index);
    // tabImage.save(imageName);

    // // Store the image in a list for later merging
    // capturedImages.append(tabImage);

    // // If all three tabs have been captured, merge them
    // if (capturedImages.size() == 3) {
    //     mergeImage();
    // }
}

void MainWindow::mergeImage()
{
    // Define the target size for the merged image
    QSize targetSize(1920, 1080);

    // Create a new image to hold the merged result
    QPixmap mergedImage(targetSize);
    // mergedImage.fill(Qt::black); // Fill with a background color (optional, use black instead of white)

    QPainter painter(&mergedImage);
    int xOffset = 0;

    // Calculate the width for each tab image
    int tabWidth = targetSize.width() / 3; // Each tab gets 640 pixels width

    // Ensure there are exactly 3 images to merge
    if (capturedImages.size() != 3) {
        qDebug() << "Error: Expected 3 images, but found" << capturedImages.size();
        return;
    }

    // Draw each image side by side after resizing
    for (int i = 0; i < 3; ++i) {
        const QPixmap &image = capturedImages[i];
        qDebug() << "Drawing tab" << (i + 1) << "at xOffset:" << xOffset;

        // Resize the image to fit the tab width and target height
        QPixmap resizedImage = image.scaled(tabWidth, targetSize.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(xOffset, 0, resizedImage);
        xOffset += tabWidth;
    }

    // Save the merged image
    mergedImage.save("merged_image.png");
    qDebug() << "Merged image saved as merged_image.png with size 1920x1080";

    // Delete the individual tab images after merging
    for (int i = 0; i < 3; ++i) {
        QString imageName = QString("tab_%1.png").arg(i + 1); // Generate the file name
        if (QFile::remove(imageName)) {
            qDebug() << "Deleted:" << imageName;
        } else {
            qDebug() << "Failed to delete:" << imageName;
        }
    }

    // Clear the list for future captures
    capturedImages.clear();

}

void MainWindow::grabAllTabs()
{
    // Clear the list of captured images
    capturedImages.clear();

    // Iterate through all tabs and grab their contents in order
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        QWidget *tabWidget = ui->tabWidget->widget(i); // Get the widget of the current tab
        QPixmap tabImage = tabWidget->grab(); // Grab the contents of the tab
        capturedImages.append(tabImage); // Store the image in the list

        // Save the individual tab image (optional)
        QString imageName = QString("tab_%1.png").arg(i + 1); // Save as tab1.png, tab2.png, tab3.png
        tabImage.save(imageName);
        qDebug() << "Saved:" << imageName;
    }

    // Merge the images if all tabs are captured
    if (capturedImages.size() == ui->tabWidget->count()) {
        mergeImage();
    }
}




