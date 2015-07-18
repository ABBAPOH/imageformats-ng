#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtGui/QStandardItemModel>

#include <ImageDocument>

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
    void buildModel(QStandardItem *parent);
    void buildModel(QStandardItem *parent, int level);
    void buildModel(QStandardItem *parent, int index, int level);

private slots:
    void onClicked(const QModelIndex &index);
    void showInfo();

private:
    Ui::MainWindow *ui;

    QStandardItemModel *_model;
    ImageDocument *_document;
};

#endif // MAINWINDOW_H
