#include "supportedformatsdialog.h"
#include "ui_supportedformatsdialog.h"

#include "supportedformatsmodel.h"

SupportedFormatsDialog::SupportedFormatsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SupportedFormatsDialog),
    _model(new SupportedFormatsModel)
{
    ui->setupUi(this);
    ui->view->setModel(_model.data());
}

SupportedFormatsDialog::~SupportedFormatsDialog()
{
    delete ui;
}

SupportedFormatsModel *SupportedFormatsDialog::model() const
{
    return _model.data();
}
