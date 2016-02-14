#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtGui/QStandardItemModel>

#include <ImageIO>
#include <ImageDocument>

class ImageView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void open();
    void saveAs();

private:
    void buildModel();
    void buildModel(QStandardItem *parent);
    void buildModel(QStandardItem *parent, int level);
    void buildModel(QStandardItem *parent, int index, int level);

private slots:
    void onClicked(const QModelIndex &index);
    void showInfo();

private:
    QPair<Optional<ImageContents>, ImageIO::Error> loadContents(const QString &path);
    ImageIO::Error saveContents(const QString &path, const ImageContents &contents);

private:
    Ui::MainWindow *ui;

    QStandardItemModel *_model;
    ImageDocument *_document;
    ImageView *_view {Q_NULLPTR};
};

#endif // MAINWINDOW_H
