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
    if (_document->mipmapCount() == 1) {
        buildModel(parent, _document->mipmap());
    } else {
        for (int i = 0; i < _document->mipmapCount(); ++i) {
            ImageMipmap mipmap = _document->mipmap(i);
            QStandardItem *mipmapItem = new QStandardItem(tr("Mipmap %1").arg(i));
            buildModel(mipmapItem, mipmap);
            parent->appendRow(mipmapItem);
        }
    }
}

void MainWindow::buildModel(QStandardItem *parent, const ImageMipmap &mipmap)
{
    if (mipmap.resourceCount() == 1) {
        auto resource = mipmap.resource();
        buildModel(parent, resource);
    } else {
        for (int i = 0; i < mipmap.resourceCount(); i++) {
            QStandardItem *item = new QStandardItem(tr("Resource %1").arg(i));
            auto resource = mipmap.resource(i);
            buildModel(item, resource);
            parent->appendRow(item);
        }
    }
}

void MainWindow::buildModel(QStandardItem *parent, const ImageResource &resource)
{
    if (resource.type() == ImageResource::Image) {
        parent->setData(resource.image());
    } else if (resource.type() == ImageResource::Cubemap) {
        for (int k = 0; k < 6; k++) {
            QStandardItem *item = new QStandardItem(tr("Side %1").arg(k));
            item->setData(resource.side(ImageResource::Side(k)));
            parent->appendRow(item);
        }
    }
}

void MainWindow::onClicked(const QModelIndex &index)
{
    auto item = _model->itemFromIndex(index);
    if (!item)
        return;

    ui->label->setPixmap(QPixmap::fromImage(item->data().value<QImage>()));
}
