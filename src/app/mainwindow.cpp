#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtWidgets/QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _model = new QStandardItemModel(this);
    ui->treeView->setModel(_model);

    _document = new ImageDocument(this);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);

    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    QString path = QFileDialog::getOpenFileName(this, tr("open"));

    _document->setFileName(path);

    if (!_document->read())
        qWarning() << "Can't read" << _document->error().errorString();
    buildModel();
}

void MainWindow::buildModel()
{
    _model->clear();
    buildModel(_model->invisibleRootItem());
}

void MainWindow::buildModel(QStandardItem *parent)
{
    for (int i = 0; i < _document->mipmapCount(); ++i) {
        QStandardItem *mipmapItem = new QStandardItem(tr("Mipmap %1").arg(i));
        buildModel(mipmapItem, i);
        parent->appendRow(mipmapItem);
    }
}

void MainWindow::buildModel(QStandardItem *parent, int level)
{
    for (int i = 0; i < _document->imageCount(); i++) {
        QStandardItem *item = new QStandardItem(tr("Image %1").arg(i));
        buildModel(item, i, level);
        parent->appendRow(item);
    }
}

void MainWindow::buildModel(QStandardItem *parent, int index, int level)
{
    parent->setData(_document->image(index, level));
}

void MainWindow::onClicked(const QModelIndex &index)
{
    auto item = _model->itemFromIndex(index);
    if (!item)
        return;

    ui->label->setPixmap(QPixmap::fromImage(item->data().value<QImage>()));
}
