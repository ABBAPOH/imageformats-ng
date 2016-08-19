#pragma once

#include <QtWidgets/QDialog>

class SupportedFormatsModel;

namespace Ui {
class SupportedFormatsDialog;
}

class SupportedFormatsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SupportedFormatsDialog(QWidget *parent = 0);
    ~SupportedFormatsDialog();

    SupportedFormatsModel *model() const;

private:
    Ui::SupportedFormatsDialog *ui {nullptr};
    QScopedPointer<SupportedFormatsModel> _model;
};
