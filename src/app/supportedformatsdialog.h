#ifndef SUPPORTEDFORMATSDIALOG_H
#define SUPPORTEDFORMATSDIALOG_H

#include <QtWidgets/QDialog>

namespace Ui {
class SupportedFormatsDialog;
}

class SupportedFormatsModel;

class SupportedFormatsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SupportedFormatsDialog(QWidget *parent = 0);
    ~SupportedFormatsDialog();

    SupportedFormatsModel *model() const;

private:
    Ui::SupportedFormatsDialog *ui {nullptr};
    QScopedPointer<SupportedFormatsModel> _model {nullptr};
};

#endif // SUPPORTEDFORMATSDIALOG_H
