#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void grabAllImage();
    void grabAllTabs(); // Slot to grab all tabs' contents

    void grabAllTabs(QWidget *widget);

private slots:
    void onTabChanged(int index); // Slot to handle tab changes

private:
    void mergeImage(const QStringList &imagePaths, const QSize &targetSize, bool deleteSourceImages);
    void mergeImage(); // Function to merge captured images

    Ui::MainWindow *ui; // UI object
    QList<QPixmap> capturedImages; // List to store captured tab images
};

#endif // MAINWINDOW_H
