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
    if (_document->resourceCount() == 1) {
        auto resource = _document->resource();
        buildModel(parent, resource);
    } else {
        for (int j = 0; j < _document->resourceCount(); j++) {
            QStandardItem *item = new QStandardItem(tr("Resource %1").arg(j));
            auto resource = _document->resource(j);
            buildModel(item, resource);
            parent->appendRow(item);
        }
    }
}

void MainWindow::buildModel(QStandardItem *parent, const ImageResource &resource, int mipmap)
{
    if (resource.type() == ImageResource::Image) {
        parent->setData(resource.image(mipmap));
    } else if (resource.type() == ImageResource::Cubemap) {
        for (int k = 0; k < 6; k++) {
            CubeTexture::Side side = CubeTexture::Side(CubeTexture::PositiveX << k);
            auto texture = resource.cubeTexture();
            if (texture.sides() & side) {
                QStandardItem *item = new QStandardItem(tr("Side %1").arg(k));
                item->setData(texture.side(side));
                parent->appendRow(item);
            }
        }
    }
}

void MainWindow::buildModel(QStandardItem *parent, const ImageResource &resource)
{
    if (resource.mipmapCount() == 1) {
        buildModel(parent, resource, 0);
    } else {
        for (int i = 0; i < resource.mipmapCount(); i++) {
            QStandardItem *mipmap = new QStandardItem(tr("Mipmap %1").arg(i));
            buildModel(parent, resource, i);
            parent->appendRow(mipmap);
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
