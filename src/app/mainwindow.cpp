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

void MainWindow::buildModel(QStandardItem *parent, ImageIndex index, int level)
{
    switch (level++) {
    case 0:
        for (int i = 0; i < _document->mipmapCount(); i++) {
            index.setMipmap(i);
            QStandardItem *item = new QStandardItem(tr("Mipmap %1").arg(i));
            item->setData(_document->image(index));
            buildModel(item, index, level);
            parent->appendRow(item);
        }
        break;
    case 1:
        for (int i = 0; i < _document->frameCount(); i++) {
            index.setFrame(i);
            QStandardItem *item = new QStandardItem(tr("Frame %1").arg(i));
            item->setData(_document->image(index));
            buildModel(item, index, level);
            parent->appendRow(item);
        }
        break;
    case 2:
        if (_document->sides() != ImageIndex::NoSides) {
            for (int i = 0; i < 6; i++) {
                ImageIndex::Side side = ImageIndex::Side(ImageIndex::PositiveX << i);
                if (_document->sides() & side) {
                    index.setSide(side);
                    QStandardItem *item = new QStandardItem(tr("Side %1").arg(side));
                    item->setData(_document->image(index));
                    buildModel(item, index, level);
                    parent->appendRow(item);
                }
            }
        } else {
            index.setSide(ImageIndex::AllSides);
            QStandardItem *item = new QStandardItem(tr("No sides"));
            item->setData(_document->image(index));
            buildModel(item, index, level);
            parent->appendRow(item);
        }
        break;
    default:
        break;
    }
}

void MainWindow::onClicked(const QModelIndex &index)
{
    auto item = _model->itemFromIndex(index);
    if (!item)
        return;

    ui->label->setPixmap(QPixmap::fromImage(item->data().value<QImage>()));
}
