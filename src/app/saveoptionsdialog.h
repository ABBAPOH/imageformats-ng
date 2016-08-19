#pragma once

#include <QtWidgets/QDialog>

#include <ImageFormatInfo>
class ImageOptions;

namespace Ui {
class SaveOptionsDialog;
}

class SaveOptionsDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(ImageFormatInfo imageFormat READ imageFormat WRITE setImageFormat NOTIFY imageFormatChanged)
    Q_PROPERTY(ImageOptions options READ options WRITE setOptions NOTIFY optionsChanged)
public:
    explicit SaveOptionsDialog(QWidget *parent = 0);
    ~SaveOptionsDialog();

    ImageFormatInfo imageFormat() const;
    void setImageFormat(const ImageFormatInfo &imageFormat);

    QByteArray subType() const;

    ImageOptions options() const;
    void setOptions(const ImageOptions &options);

signals:
    void optionsChanged(const ImageOptions &options);
    void imageFormatChanged(const ImageFormatInfo &imageFormat);

private:
    void updateUi();
    void updateUi2();

private:
    Ui::SaveOptionsDialog *ui;
    ImageOptions _options;
    ImageFormatInfo _imageFormat;
};
