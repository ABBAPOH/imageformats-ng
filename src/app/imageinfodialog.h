#pragma once

#include <ImageContents>
#include <QtWidgets/QDialog>

class ImageInfoModel;
class VariantMapModel;

namespace Ui {
class ImageInfoDialog;
}

class ImageInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImageInfoDialog(QWidget *parent = 0);
    ~ImageInfoDialog();

    ImageContents contents();
    void setContents(const ImageContents &contents);

private:
    Ui::ImageInfoDialog *ui;
    ImageContents _contents;
    QScopedPointer<ImageInfoModel> _imageInfoModel;
    QScopedPointer<VariantMapModel> _exifInfoModel;
};
