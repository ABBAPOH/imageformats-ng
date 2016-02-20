#include "saveoptionsdialog.h"
#include "ui_saveoptionsdialog.h"

SaveOptionsDialog::SaveOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveOptionsDialog)
{
    ui->setupUi(this);
    connect(ui->subTypeInput, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SaveOptionsDialog::updateUi2);
}

SaveOptionsDialog::~SaveOptionsDialog()
{
    delete ui;
}

ImageFormatInfo SaveOptionsDialog::imageFormat() const
{
    return _imageFormat;
}

void SaveOptionsDialog::setImageFormat(const ImageFormatInfo &imageFormat)
{
    if (_imageFormat == imageFormat)
        return;

    _imageFormat = imageFormat;
    updateUi();
    emit imageFormatChanged(imageFormat);
}

ImageOptions SaveOptionsDialog::options() const
{
    return _options;
}

void SaveOptionsDialog::setOptions(const ImageOptions &options)
{
    if (_options == options)
        return;

    _options = options;
    emit optionsChanged(_options);
}

void SaveOptionsDialog::updateUi()
{
    const auto subTypes = _imageFormat.supportedSubTypes();
    const bool hasSubTypes = subTypes.size() > 0;
    ui->subTypeLabel->setVisible(hasSubTypes);
    ui->subTypeInput->setVisible(hasSubTypes);

    for (const auto type : subTypes) {
        ui->subTypeInput->addItem(QString::fromLatin1(type));
    }
    ui->subTypeInput->setCurrentIndex(0);
}

void SaveOptionsDialog::updateUi2()
{
    const auto subType = ui->subTypeInput->count() > 0
            ? ui->subTypeInput->currentText().toLatin1()
            : QByteArray();

    const bool hasCompression = _imageFormat.supportsOption(ImageOptions::CompressionRatio, subType);
    ui->compressionLabel->setVisible(hasCompression);
    ui->compressionInput->setVisible(hasCompression);

    const bool hasQuality = _imageFormat.supportsOption(ImageOptions::Quality, subType);
    ui->qualityLabel->setVisible(hasQuality);
    ui->qualityInput->setVisible(hasQuality);

    const bool hasGamma = _imageFormat.supportsOption(ImageOptions::Gamma, subType);
    ui->gammaLabel->setVisible(hasGamma);
    ui->gammaInput->setVisible(hasGamma);
}
