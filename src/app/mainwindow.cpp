#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <ImageView>
#include <VariantMapModel>

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
    connect(ui->actionShowInfo, &QAction::triggered, this, &MainWindow::showInfo);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);

    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onClicked);

    _view = new ImageView;
    _view->setDocument(_document);
    setCentralWidget(_view);

    connect(ui->actionZoom_In, &QAction::triggered, _view, &ImageView::zoomIn);
    connect(ui->actionZoom_Out, &QAction::triggered, _view, &ImageView::zoomOut);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    QString path = QFileDialog::getOpenFileName(this, tr("open"));

    _document->setFileName(path);

    auto ok = _document->open();
    if (!ok)
        qWarning() << "Can't open" << ok.errorString();
    buildModel();
}

void MainWindow::buildModel()
{
    _model->clear();
    buildModel(_model->invisibleRootItem());
}

void MainWindow::buildModel(QStandardItem *parent)
{
    if (_document->contents().mipmapCount() == 1) {
        buildModel(parent, 0);
    } else {
        for (int i = 0; i < _document->contents().mipmapCount(); ++i) {
            QStandardItem *mipmapItem = new QStandardItem(tr("Mipmap %1").arg(i));
            buildModel(mipmapItem, i);
            parent->appendRow(mipmapItem);
        }
    }
}

void MainWindow::buildModel(QStandardItem *parent, int level)
{
    if (_document->contents().imageCount() == 1) {
        buildModel(parent, 0, level);
    } else {
        for (int i = 0; i < _document->contents().imageCount(); i++) {
            QStandardItem *item = new QStandardItem(tr("Image %1").arg(i));
            buildModel(item, i, level);
            parent->appendRow(item);
        }
    }
}

void MainWindow::buildModel(QStandardItem *parent, int index, int level)
{
    QStandardItem *item = parent;
    if (item == _model->invisibleRootItem()) {
        item = new QStandardItem(tr("Image"));
        parent->appendRow(item);
    }
    item->setData(QVariant::fromValue(qMakePair(index, level)));
}

void MainWindow::onClicked(const QModelIndex &index)
{
    auto item = _model->itemFromIndex(index);
    if (!item)
        return;

    auto data = item->data().value<QPair<int, int>>();
    _view->jumpTo(data.first, data.second);
}

QString tagName(ImageExifMeta::Tag tag)
{
    switch (tag) {
    case ImageExifMeta::TagOrientation: return MainWindow::tr("Orientation");
    default:
        break;
    }
    return QString();
}

void MainWindow::showInfo()
{
    QTreeView *view = new QTreeView();
    auto values = _document->contents().exifMeta().toVariantMap();
    auto model = new VariantMapModel(view);
    model->setVariantMap(values);
    view->resize(300, 400);
    view->setModel(model);
    view->show();
}
