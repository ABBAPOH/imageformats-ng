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

class MainWindowPrivate;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MainWindow)
    Q_DISABLE_COPY(MainWindow)
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void newFromClipboard();
    void open();
    void save();
    void saveAs();
    void openDocument(const QUrl &url);
    void saveDocument(const QUrl &url, const QByteArray &subType, const ImageOptions &options);
    void convertToProjection();
    void showSupportedFormats();

private slots:
    void onClicked(const QModelIndex &index);
    void showInfo();

private:
    QScopedPointer<MainWindowPrivate> d_ptr;
};
