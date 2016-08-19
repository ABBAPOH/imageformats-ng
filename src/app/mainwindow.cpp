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
#include <QtConcurrent/QtConcurrent>
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

    openDocument(QUrl::fromLocalFile(path));
}

void MainWindow::save()
{
    saveDocument(_url, QByteArray(), ImageOptions());
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

    saveDocument(QUrl::fromLocalFile(path), dialog.subType(), dialog.options());
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
    if (_document->contents().mipmapCount() <= 1) {
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

void MainWindow::openDocument(const QUrl &url)
{
    if (!url.isLocalFile()) {
        return;
    }

    _url = url;

    using Result = Optional<ImageContents>;

    auto worker = [](const QString &path)
    {
        ImageIO io(path);
        return io.read();
    };

    auto finisher = [this]()
    {
        const auto watcher = static_cast<QFutureWatcher<Result> *>(sender());
        const auto result = watcher->future().result();
        watcher->deleteLater();
        if (result) {
            _document->setContents(*result);
        } else {
            QMessageBox::warning(this,
                                 tr("Open"),
                                 tr("Can't open file"),
                                 QMessageBox::Close);
        }
    };

    auto watcher = new QFutureWatcher<Result>(this);
    connect(watcher, &QFutureWatcherBase::finished, this, finisher);
    watcher->setFuture(QtConcurrent::run(worker, url.toLocalFile()));
}

void MainWindow::saveDocument(const QUrl &url, const QByteArray &subType, const ImageOptions &options)
{
    if (!url.isLocalFile()) {
        return;
    }

    const auto workerSubType = subType;
    const auto workerOptions = options;
    using Result = bool;

    auto worker = [workerSubType, workerOptions](const QString &path, const ImageContents &contents)
    {
        ImageIO io(path);
        io.setSubType(workerSubType);
        return io.write(contents, workerOptions);
    };

    auto finisher = [this]()
    {
        const auto watcher = static_cast<QFutureWatcher<Result> *>(sender());
        const auto result = watcher->future().result();
        watcher->deleteLater();
        if (!result) {
            QMessageBox::warning(this,
                                 tr("Save"),
                                 tr("Can't save file"),
                                 QMessageBox::Close);
        }
    };

    auto watcher = new QFutureWatcher<Result>(this);
    connect(watcher, &QFutureWatcherBase::finished, this, finisher);
    watcher->setFuture(QtConcurrent::run(worker, url.toLocalFile(), _document->contents()));
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
