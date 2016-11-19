#pragma once

#include "mainwindow.h"
#include <ThumbnailsModel>

class MainWindowPrivate
{
    Q_DECLARE_PUBLIC(MainWindow)
    Q_DISABLE_COPY(MainWindowPrivate)
public:
    enum Action {
        NewFromClipboard,
        Open,
        Save,
        SaveAs,
        ShowInfo,
        ConvertToProjection,
        Quit,
        SupportedFormats,

        ActionsCount
    };
    explicit MainWindowPrivate(MainWindow *qq) : q_ptr(qq) {}

    void init();
    void createDockWidget();
    void createActions();
    void retranslateUi();
    void createMenuBar();

public:
    QScopedPointer<ThumbnailsModel> _model;
    ImageDocumentPointer _document;
    QScopedPointer<ImageView> _view;
    QScopedPointer<ImageInfoDialog> _imageInfoDialog;
    QScopedPointer<QAction> actions[MainWindowPrivate::ActionsCount];
    QScopedPointer<QMenu> menuFile;
    QScopedPointer<QMenu> menuView;
    QScopedPointer<QMenu> menuTools;
    QScopedPointer<QMenu> menuHelp;
    QUrl _url;

protected:
    MainWindow *q_ptr {nullptr};
};
