#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtGui/QStandardItemModel>

#include <imagedocument.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void open();

private:
    void buildModel();
    void buildModel(QStandardItem *parent, ImageIndex index = ImageIndex(), int level = 0);

private slots:
    void onClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    QStandardItemModel *_model;
    ImageDocument *_document;
};

#endif // MAINWINDOW_H
