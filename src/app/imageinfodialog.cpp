#include "imageinfodialog.h"
#include "ui_imageinfodialog.h"

#include <ImageInfoModel>
#include <VariantMapModel>

ImageInfoDialog::ImageInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageInfoDialog),
    _imageInfoModel(new ImageInfoModel),
    _exifInfoModel(new VariantMapModel)
{
    ui->setupUi(this);
    ui->imageInfoView->setModel(_imageInfoModel.data());
    ui->exifInfoView->setModel(_exifInfoModel.data());
}

ImageInfoDialog::~ImageInfoDialog()
{
    delete ui;
}

ImageContents ImageInfoDialog::contents()
{
    return _contents;
}

void ImageInfoDialog::setContents(const ImageContents &contents)
{
    _contents = contents;
    _imageInfoModel->setImageContents(_contents);
    _exifInfoModel->setVariantMap(_contents.exifMeta().toVariantMap());
}
