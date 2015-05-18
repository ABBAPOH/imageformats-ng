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
    _document->open(ImageDocument::ReadWrite);

    if (!_document->read())
        qWarning() << "Can't read" << _document->errorString();
    buildModel();
}

void MainWindow::buildModel()
{
    _model->clear();
    buildModel(_model->invisibleRootItem());
}

void MainWindow::buildModel(QStandardItem *parent)
{
    for (int i = 0; i < _document->mipmapCount(); i++) {
        QStandardItem *mipmap = new QStandardItem(tr("Mipmap %1").arg(i));
        for (int j = 0; j < _document->frameCount(); j++) {
            QStandardItem *frame = new QStandardItem(tr("Frame %1").arg(j));

            auto resource = _document->resource(i, j);
            if (resource.type() == ImageResource::Cubemap) {
                for (int k = 0; k < 6; k++) {
                    ImageResource::Side side = ImageResource::Side(ImageResource::PositiveX << k);
                    if (_document->sides() & side) {
                        QStandardItem *item = new QStandardItem(tr("Side %1").arg(k));
                        item->setData(resource.image(side));
                        frame->appendRow(item);
                    }
                }
            } else {
                frame->setData(resource.image());
            }
            mipmap->appendRow(frame);
        }

        parent->appendRow(mipmap);
    }
}

void MainWindow::onClicked(const QModelIndex &index)
{
    auto item = _model->itemFromIndex(index);
    if (!item)
        return;

    ui->label->setPixmap(QPixmap::fromImage(item->data().value<QImage>()));
}
