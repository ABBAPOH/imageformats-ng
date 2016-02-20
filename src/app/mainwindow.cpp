#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imageinfodialog.h"
#include "saveoptionsdialog.h"
#include "supportedformatsdialog.h"
#include "supportedformatsmodel.h"

#include <ImageIO>
#include <ImageView>
#include <VariantMapModel>

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QMimeDatabase>
#include <QtGui/QClipboard>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

namespace {

QVector<QMimeType> formatsToMimeTypes(const QVector<ImageFormatInfo> &formats)
{
    QVector<QMimeType> result;
    for (auto format : formats)
        result.append(format.mimeType());
    return result;
}

QString mimeTypesToFilters(const QVector<QMimeType> &mimeTypes)
{
    QStringList result;
    for (auto mt : mimeTypes)
        result.append(mt.filterString());
    return result.join(";;");
}

} // namespace

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dockWidget->setTitleBarWidget(new QWidget());
    _model = new QStandardItemModel(this);
    ui->treeView->setModel(_model);

    _document = new ImageDocument(this);
    connect(_document, &AbstractDocument::openFinished, this, &MainWindow::onOpenFinished);
    connect(_document, &AbstractDocument::saveFinished, this, &MainWindow::onSaveFinished);

    connect(ui->actionNewFromClipboard, &QAction::triggered, this, &MainWindow::newFromClipboard);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionShowInfo, &QAction::triggered, this, &MainWindow::showInfo);
    connect(ui->actionConvertToProjection, &QAction::triggered,
            this, &MainWindow::convertToProjection);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);

    connect(ui->actionSupportedFormats, &QAction::triggered,
            this, &MainWindow::showSupportedFormatsDialog);

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

void MainWindow::newFromClipboard()
{
    const auto image = qApp->clipboard()->image();
    if (image.isNull())
        return;

    ImageContents contents(image);
    _document->setContents(contents);
    buildModel();
}

void MainWindow::open()
{
    const auto formats = ImageIO::supportedImageFormats(ImageFormatInfo::CanRead);
    const auto filters = tr("All Files (*);;") + mimeTypesToFilters(formatsToMimeTypes(formats));
    const auto path = QFileDialog::getOpenFileName(this, tr("open"), QString(), filters);
    if (path.isEmpty())
        return;

    _document->setUrl(QUrl::fromLocalFile(path));
    _document->open();
}

void MainWindow::save()
{
    _document->save();
}

void MainWindow::saveAs()
{
    const auto formats = ImageIO::supportedImageFormats(ImageFormatInfo::CanWrite);
    const auto filters = tr("All Files (*);;") + mimeTypesToFilters(formatsToMimeTypes(formats));
    const auto path = QFileDialog::getSaveFileName(this, tr("Save As"), QString(), filters);
    if (path.isEmpty())
        return;

    const auto mt = QMimeDatabase().mimeTypeForFile(path);
    const auto info = ImageIO::imageFormat(mt);
    if (!info) {
        QMessageBox::warning(this, tr("Save"), tr("No format for mimetype %1").arg(mt.name()));
        return;
    }

    SaveOptionsDialog dialog;
    dialog.setImageFormat(*info);
    if (dialog.exec() == QDialog::Rejected)
        return;

    _document->setUrl(QUrl::fromLocalFile(path));
    _document->save({ {"subType",  dialog.subType()},
                      {"imageOptions", QVariant::fromValue(dialog.options())} });
}

void MainWindow::convertToProjection()
{
    const auto projection = _document->contents().toProjection();
    if (!projection) {
        QMessageBox::warning(this,
                             tr("Convert to projection"),
                             tr("Can't convert file"));
    }
    _document->setContents(*projection);
    buildModel();
}

void MainWindow::showSupportedFormatsDialog()
{
    SupportedFormatsDialog dialog;
    dialog.model()->setFormats(ImageIO::supportedImageFormats());
    dialog.exec();
}

void MainWindow::buildModel()
{
    _model->clear();
    buildModel(_model->invisibleRootItem());
}

void MainWindow::buildModel(QStandardItem *parent)
{
    if (_document->contents().header().mipmapCount() <= 1) {
        buildModel(parent, 0);
    } else {
        for (int i = 0; i < _document->contents().header().mipmapCount(); ++i) {
            QStandardItem *mipmapItem = new QStandardItem(tr("Mipmap %1").arg(i));
            buildModel(mipmapItem, i);
            parent->appendRow(mipmapItem);
        }
    }
}

void MainWindow::buildModel(QStandardItem *parent, int level)
{
    if (_document->contents().header().imageCount() == 1) {
        buildModel(parent, 0, level);
    } else {
        for (int i = 0; i < _document->contents().header().imageCount(); i++) {
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

void MainWindow::showInfo()
{
    if (!_imageInfoDialog)
        _imageInfoDialog.reset(new ImageInfoDialog(this));

    _imageInfoDialog->setContents(_document->contents());
    _imageInfoDialog->show();
}

void MainWindow::onOpenFinished(bool ok)
{
    if (!ok) {
        QMessageBox::warning(this,
                             tr("Open"),
                             tr("Can't open file"),
                             QMessageBox::Close);
        return;
    }

    buildModel();
}

void MainWindow::onSaveFinished(bool ok)
{
    if (!ok) {
        QMessageBox::warning(this,
                             tr("Save"),
                             tr("Can't save file"),
                             QMessageBox::Close);
    }
}
