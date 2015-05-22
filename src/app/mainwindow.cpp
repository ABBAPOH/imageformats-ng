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
    for (int j = 0; j < _document->resourceCount(); j++) {
        QStandardItem *resItem = new QStandardItem(tr("Resource %1").arg(j));

        auto resource = _document->resource(j);
        for (int i = 0; i < resource.mipmapCount(); i++) {
            QStandardItem *mipmap = new QStandardItem(tr("Mipmap %1").arg(i));

            if (resource.type() == ImageResource::Cubemap) {
                for (int k = 0; k < 6; k++) {
                    ImageResource::Side side = ImageResource::Side(ImageResource::PositiveX << k);
                    if (resource.sides() & side) {
                        QStandardItem *item = new QStandardItem(tr("Side %1").arg(k));
                        item->setData(resource.side(side));
                        mipmap->appendRow(item);
                    }
                }
            } else {
                mipmap->setData(resource.image());
            }
            resItem->appendRow(mipmap);
        }

        parent->appendRow(resItem);
    }
}

void MainWindow::onClicked(const QModelIndex &index)
{
    auto item = _model->itemFromIndex(index);
    if (!item)
        return;

    ui->label->setPixmap(QPixmap::fromImage(item->data().value<QImage>()));
}
