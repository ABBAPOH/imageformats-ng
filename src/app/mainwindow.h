#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtGui/QStandardItemModel>

#include <ImageIO>
#include <ImageDocument>

class ImageView;
class ImageInfoDialog;

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
    void save();
    void saveAs();
    void showSupportedFormatsDialog();

private:
    void buildModel();
    void buildModel(QStandardItem *parent);
    void buildModel(QStandardItem *parent, int level);
    void buildModel(QStandardItem *parent, int index, int level);

private slots:
    void onClicked(const QModelIndex &index);
    void showInfo();
    void onOpenFinished(bool ok);
    void onSaveFinished(bool ok);

private:
    Ui::MainWindow *ui;

    QStandardItemModel *_model;
    ImageDocument *_document;
    ImageView *_view {Q_NULLPTR};
    QScopedPointer<ImageInfoDialog> _imageInfoDialog;
};

#endif // MAINWINDOW_H
