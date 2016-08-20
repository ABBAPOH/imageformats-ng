#include "mainwindow.h"
#include "mainwindow_p.h"

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
#include <QtWidgets/QApplication>
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTreeView>

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

void MainWindowPrivate::init()
{
    Q_Q(MainWindow);

    q->resize(800, 600);

    _model.reset(new QStandardItemModel);
    _document.reset(new ImageDocument);
    _view.reset(new ImageView);

    _view->setDocument(_document.data());
    q->setCentralWidget(_view.data());

    createDockWidget();
    createActions();
    createMenuBar();
    retranslateUi();
}

void MainWindowPrivate::createDockWidget()
{
    Q_Q(MainWindow);
    const auto dockWidget = new QDockWidget(q);
    dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    const auto treeView = new QTreeView(dockWidget);
    treeView->setModel(_model.data());
    q->connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
               q, &MainWindow::onClicked);
    dockWidget->setWidget(treeView);
    dockWidget->setTitleBarWidget(new QWidget);
    q->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
}

void MainWindowPrivate::createActions()
{
    Q_Q(MainWindow);

    actions[MainWindowPrivate::NewFromClipboard].reset(new QAction);
    actions[MainWindowPrivate::NewFromClipboard]->setShortcut(QKeySequence::New);
    q->addAction(actions[MainWindowPrivate::NewFromClipboard].data());
    q->connect(actions[MainWindowPrivate::NewFromClipboard].data(), &QAction::triggered,
            q, &MainWindow::newFromClipboard);

    actions[MainWindowPrivate::Open].reset(new QAction);
    actions[MainWindowPrivate::Open]->setShortcut(QKeySequence::Open);
    q->addAction(actions[MainWindowPrivate::Open].data());
    q->connect(actions[MainWindowPrivate::Open].data(), &QAction::triggered, q, &MainWindow::open);

    actions[MainWindowPrivate::Save].reset(new QAction);
    actions[MainWindowPrivate::Save]->setShortcut(QKeySequence::Save);
    q->addAction(actions[MainWindowPrivate::Save].data());
    q->connect(actions[MainWindowPrivate::Save].data(), &QAction::triggered, q, &MainWindow::save);

    actions[MainWindowPrivate::SaveAs].reset(new QAction);
    actions[MainWindowPrivate::SaveAs]->setShortcut(QKeySequence::SaveAs);
    q->addAction(actions[MainWindowPrivate::SaveAs].data());
    q->connect(actions[MainWindowPrivate::SaveAs].data(), &QAction::triggered,
            q, &MainWindow::saveAs);

    actions[MainWindowPrivate::ShowInfo].reset(new QAction);
    actions[MainWindowPrivate::ShowInfo]->setShortcut(QKeySequence("Ctrl+I"));
    q->addAction(actions[MainWindowPrivate::ShowInfo].data());
    q->connect(actions[MainWindowPrivate::ShowInfo].data(), &QAction::triggered,
            q, &MainWindow::showInfo);

    actions[MainWindowPrivate::ConvertToProjection].reset(new QAction);
    q->addAction(actions[MainWindowPrivate::ConvertToProjection].data());
    q->connect(actions[MainWindowPrivate::ConvertToProjection].data(), &QAction::triggered,
            q, &MainWindow::convertToProjection);

    actions[MainWindowPrivate::Quit].reset(new QAction);
    actions[MainWindowPrivate::Quit]->setShortcut(QKeySequence::Quit);
    actions[MainWindowPrivate::Quit]->setMenuRole(QAction::QuitRole);
    q->addAction(actions[MainWindowPrivate::Quit].data());
    q->connect(actions[MainWindowPrivate::Quit].data(), &QAction::triggered,
            q, &MainWindow::close);

    actions[MainWindowPrivate::SupportedFormats].reset(new QAction);
    q->addAction(actions[MainWindowPrivate::SupportedFormats].data());
    q->connect(actions[MainWindowPrivate::SupportedFormats].data(), &QAction::triggered,
            q, &MainWindow::showSupportedFormats);
}

void MainWindowPrivate::retranslateUi()
{
    actions[MainWindowPrivate::NewFromClipboard]->setText(MainWindow::tr("New from clipboard"));
    actions[MainWindowPrivate::Open]->setText(MainWindow::tr("Open"));
    actions[MainWindowPrivate::Save]->setText(MainWindow::tr("Save"));
    actions[MainWindowPrivate::SaveAs]->setText(MainWindow::tr("Save As..."));
    actions[MainWindowPrivate::ShowInfo]->setText(MainWindow::tr("Show Info..."));
    actions[MainWindowPrivate::ConvertToProjection]->setText(MainWindow::tr("Convert to projection"));
    actions[MainWindowPrivate::Quit]->setText(MainWindow::tr("Quit"));
    actions[MainWindowPrivate::SupportedFormats]->setText(MainWindow::tr("Supported Formats..."));

    menuFile->setTitle(MainWindow::tr("File"));
    menuView->setTitle(MainWindow::tr("View"));
    menuHelp->setTitle(MainWindow::tr("Help"));
}

void MainWindowPrivate::createMenuBar()
{
    Q_Q(MainWindow);

    const auto menuBar = new QMenuBar(q);
    menuFile.reset(new QMenu);
    menuFile->addAction(actions[MainWindowPrivate::NewFromClipboard].data());
    menuFile->addAction(actions[MainWindowPrivate::Open].data());
    menuFile->addAction(actions[MainWindowPrivate::Save].data());
    menuFile->addAction(actions[MainWindowPrivate::SaveAs].data());
    menuFile->addSeparator();
    menuFile->addAction(actions[MainWindowPrivate::ShowInfo].data());
    menuFile->addSeparator();
    menuFile->addAction(actions[MainWindowPrivate::ConvertToProjection].data());
    menuFile->addSeparator();
    menuFile->addAction(actions[MainWindowPrivate::Quit].data());
    menuBar->addAction(menuFile->menuAction());

    menuView.reset(new QMenu);
    menuView->addAction(_view->action(ImageView::ZoomIn));
    menuView->addAction(_view->action(ImageView::ZoomOut));
    menuView->addAction(_view->action(ImageView::NormalSize));
    menuBar->addAction(menuView->menuAction());

    menuHelp.reset(new QMenu);
    menuHelp->addAction(actions[MainWindowPrivate::SupportedFormats].data());
    menuBar->addAction(menuHelp->menuAction());
    q->setMenuBar(menuBar);
}

/*!
    \class MainWindow
    Implementation of the main application window.
*/

/*!
    Constructs MainWindow with the given \a parent.
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    d_ptr(new MainWindowPrivate(this))
{
    Q_D(MainWindow);
    d->init();
}

MainWindow::~MainWindow()
{
}

void MainWindow::newFromClipboard()
{
    Q_D(MainWindow);
    const auto image = qApp->clipboard()->image();
    if (image.isNull())
        return;

    ImageContents contents(image);
    d->_document->setContents(contents);
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
    Q_D(MainWindow);
    saveDocument(d->_url, QByteArray(), ImageOptions());
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
    Q_D(MainWindow);
    const auto projection = d->_document->contents().toProjection();
    if (!projection) {
        QMessageBox::warning(this,
                             tr("Convert to projection"),
                             tr("Can't convert file"));
    }
    d->_document->setContents(*projection);
    buildModel();
}

void MainWindow::showSupportedFormats()
{
    SupportedFormatsDialog dialog;
    dialog.model()->setFormats(ImageIO::supportedImageFormats());
    dialog.exec();
}

void MainWindow::buildModel()
{
    Q_D(MainWindow);
    d->_model->clear();
    buildModel(d->_model->invisibleRootItem());
}

void MainWindow::buildModel(QStandardItem *parent)
{
    Q_D(MainWindow);
    if (d->_document->contents().mipmapCount() <= 1) {
        buildModel(parent, 0);
    } else {
        for (int i = 0; i < d->_document->contents().mipmapCount(); ++i) {
            QStandardItem *mipmapItem = new QStandardItem(tr("Mipmap %1").arg(i));
            buildModel(mipmapItem, i);
            parent->appendRow(mipmapItem);
        }
    }
}

void MainWindow::buildModel(QStandardItem *parent, int level)
{
    Q_D(MainWindow);
    if (d->_document->contents().imageCount() == 1) {
        buildModel(parent, 0, level);
    } else {
        for (int i = 0; i < d->_document->contents().imageCount(); i++) {
            QStandardItem *item = new QStandardItem(tr("Image %1").arg(i));
            buildModel(item, i, level);
            parent->appendRow(item);
        }
    }
}

void MainWindow::buildModel(QStandardItem *parent, int index, int level)
{
    Q_D(MainWindow);
    QStandardItem *item = parent;
    if (item == d->_model->invisibleRootItem()) {
        item = new QStandardItem(tr("Image"));
        parent->appendRow(item);
    }
    item->setData(QVariant::fromValue(qMakePair(index, level)));
}

void MainWindow::openDocument(const QUrl &url)
{
    Q_D(MainWindow);
    if (!url.isLocalFile()) {
        return;
    }

    d->_url = url;

    using Result = Optional<ImageContents>;

    auto worker = [](const QString &path)
    {
        ImageIO io(path);
        return io.read();
    };

    auto finisher = [this, d]()
    {
        const auto watcher = static_cast<QFutureWatcher<Result> *>(sender());
        const auto result = watcher->future().result();
        watcher->deleteLater();
        if (result) {
            d->_document->setContents(*result);
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
    buildModel();
}

void MainWindow::saveDocument(const QUrl &url, const QByteArray &subType, const ImageOptions &options)
{
    Q_D(MainWindow);

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
    watcher->setFuture(QtConcurrent::run(worker, url.toLocalFile(), d->_document->contents()));
}

void MainWindow::onClicked(const QModelIndex &index)
{
    Q_D(MainWindow);
    auto item = d->_model->itemFromIndex(index);
    if (!item)
        return;

    auto data = item->data().value<QPair<int, int>>();
    d->_view->jumpTo(data.first, data.second);
}

void MainWindow::showInfo()
{
    Q_D(MainWindow);

    if (!d->_imageInfoDialog)
        d->_imageInfoDialog.reset(new ImageInfoDialog(this));

    d->_imageInfoDialog->setContents(d->_document->contents());
    d->_imageInfoDialog->show();
}
