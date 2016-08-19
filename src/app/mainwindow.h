#pragma once

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
    void newFromClipboard();
    void open();
    void save();
    void saveAs();
    void convertToProjection();
    void showSupportedFormatsDialog();

private:
    void buildModel();
    void buildModel(QStandardItem *parent);
    void buildModel(QStandardItem *parent, int level);
    void buildModel(QStandardItem *parent, int index, int level);
    void openDocument(const QUrl &url);
    void saveDocument(const QUrl &url, const QByteArray &subType, const ImageOptions &options);

private slots:
    void onClicked(const QModelIndex &index);
    void showInfo();

private:
    Ui::MainWindow *ui;

    QStandardItemModel *_model;
    ImageDocument *_document;
    QUrl _url;
    ImageView *_view {Q_NULLPTR};
    QScopedPointer<ImageInfoDialog> _imageInfoDialog;
};
