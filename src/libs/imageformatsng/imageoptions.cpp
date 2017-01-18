#include "imageoptions.h"

#include <QtCore/QMetaEnum>

/*!
    \class ImageOptions

    This class describes the different options supported by ImageIO.

    Some options are used to modify how the image is read, and others are used
    to toggle the way in which an image should be written.
*/

/*!
    \enum ImageOptions::Option

    This enum describes possible options.

    \var ImageOptions::ClipRect
    \sa ImageOptions::clipRect

    \var ImageOptions::ScaledSize
    \sa ImageOptions::scaledSize

    \var ImageOptions::ScaledClipRect
    \sa ImageOptions::scaledCliptRect

    \var ImageOptions::InputQuality
    \sa ImageOptions::inputQuality

    \var ImageOptions::BackgroundColor
    \sa ImageOptions::backgroundColor

    \var ImageOptions::CompressionRatio
    \sa ImageOptions::compression

    \var ImageOptions::Gamma
    \sa ImageOptions::gamma

    \var ImageOptions::Quality
    \sa ImageOptions::quality
*/

/*!
    Creates an empty ImageOptions() object.
*/
ImageOptions::ImageOptions() = default;

/*!
    Copy-constructs an ImageOptions object.
*/
ImageOptions::ImageOptions(const ImageOptions &other) = default;

/*!
    Move-constructs an ImageOptions object.
*/
ImageOptions::ImageOptions(ImageOptions &&other) = default;

/*!
    Destroys the ImageOptions object.
*/
ImageOptions::~ImageOptions() = default;

/*!
    Assigns \a other ImageOptions to this result.
*/
ImageOptions &ImageOptions::operator=(const ImageOptions &other) = default;

/*!
    Move-assigns \a other ImageOptions to this result.
*/
ImageOptions &ImageOptions::operator=(ImageOptions &&other) = default;

/*!
    Returns true if no options are set.
*/
bool ImageOptions::isEmpty() const
{
    return _options.isEmpty();
}

/*!
    Returns true if the given \a option is set.
*/
bool ImageOptions::contains(ImageOptions::Option option) const
{
    return _options.contains(option);
}

/*!
    Unsets the value for the given \a option.
*/
void ImageOptions::remove(ImageOptions::Option option)
{
    _options.remove(option);
}

/*!
    \property ImageOptions::clipRect

    The clip rect, or ROI (Region Of Interest). A handler that supports this
    option is expected to only read the provided QRect area from the
    original image in read(), before any other transformation is applied. If
    the handler does not support this option, ImageIO will perform the clipping
    after the image has been read.
*/

QRect ImageOptions::clipRect() const
{
    return _options.value(ClipRect, QRect()).toRect();
}

void ImageOptions::setClipRect(const QRect &rect)
{
    _options.insert(ClipRect, rect);
}

/*!
    \property ImageOptions::scaledSize

    The scaled size of the image. A handler that supports this option is
    expected to scale the image to the provided size (a QSize), after applying
    any clip rect transformation (ClipRect). If the handler does not support
    this option, ImageIO will perform the scaling after the image has
    been read.
*/

QSize ImageOptions::scaledSize() const
{
    return _options.value(ScaledSize, QSize()).toSize();
}

void ImageOptions::setScaledSize(const QSize &size)
{
    _options.insert(ScaledSize, size);
}

/*!
    \property ImageOptions::scaledCliptRect

    The scaled clip rect (or ROI, Region Of Interest) of the image. A handler
    that supports this option is expected to apply the provided clip rect
    (a QRect), after applying any scaling (ScaleSize) or regular clipping
    (ClipRect). If the handler does not support this option, ImageIO will apply
    the scaled clip rect after the image has been read.
*/

QRect ImageOptions::scaledCliptRect() const
{
    return _options.value(ScaledClipRect, QRect()).toRect();
}

void ImageOptions::setScaledCliptRect(const QRect &rect)
{
    _options.insert(ScaledClipRect, rect);
}

/*!
    \property ImageOptions::inputQuality

    The input quality level of the image.
    This option can be used to speed up reading of the image in a cost of
    quality. If handler supports this option, it can use use faster
    transforming/scaling/decompression algorithms when reading image.

    This value should be in range [0..100].
*/

/*!
    Returns the value of the InputQuality option if it is set or the
    \a defaultValue otherwise.
*/
int ImageOptions::inputQuality(int defaultValue) const
{
    return _options.value(InputQuality, defaultValue).toInt();
}

void ImageOptions::setInputQuality(int quality)
{
    _options.insert(InputQuality, quality);
}

/*!
    \property ImageOptions::backgroundColor

    Certain image formats allow the background color to be specified. A handler
    that supports BackgroundColor initializes the background color to this
    option when reading an image.
*/

QColor ImageOptions::backgroundColor() const
{
    return _options.value(BackgroundColor).value<QColor>();
}

void ImageOptions::setBackgroundColor(const QColor &color)
{
    _options.insert(BackgroundColor, color);
}

/*!
    \property ImageOptions::compression

    The compression ratio of the image data. A handler that supports this
    option is expected to set its compression rate depending on the value of
    this option when writing.
*/

/*!
    Returns the value of the CompressionRatio option if it is set or
    the \a defaultValue otherwise.
*/
int ImageOptions::compression(int defaultValue) const
{
    return _options.value(CompressionRatio, defaultValue).toInt();
}

void ImageOptions::setCompression(int compression)
{
    _options.insert(CompressionRatio, compression);
}

/*!
    \property ImageOptions::gamma
    The gamma level of the image. A handler that supports this option is
    expected to set the image gamma level depending on the value of this option
    when writing.
*/

/*!
    Returns the value of the Gamma option if it is set or
    the \a defaultValue otherwise.
*/
float ImageOptions::gamma(float defaultValue) const
{
    return _options.value(Gamma, defaultValue).toFloat();
}

void ImageOptions::setGamma(float gamma)
{
    _options.insert(Gamma, gamma);
}

/*!
    \property ImageOptions::quality

    The quality level of the image. A handler that supports this option is
    expected to set the image quality level depending on the value of this
    option when writing.
*/

int ImageOptions::quality(int defaultValue) const
{
    return _options.value(Quality, defaultValue).toInt();
}

void ImageOptions::setQuality(int quality)
{
    _options.insert(Quality, quality);
}

/*!
    \fn void ImageOptions::swap(ImageOptions &other) Q_DECL_NOEXCEPT

    Swaps \a other options with this options.

    This operation is very fast and never fails.
*/

/*!
    \internal
*/
QString ImageOptions::optionToString(ImageOptions::Option option)
{
    const auto enumerator = QMetaEnum::fromType<ImageOptions::Option>();
    return enumerator.valueToKey(option);
}

/*!
    \internal
*/
QString ImageOptions::optionsToString(const QVector<ImageOptions::Option> &options)
{
    QStringList result;
    for (auto option : options)
        result.append(optionToString(option));
    return result.join("|");
}

/*!
    Returns true if the \a lhs options and the \a rhs options have the same
    contents; otherwise returns false.
*/
bool operator==(const ImageOptions &lhs, const ImageOptions &rhs)
{
    return lhs._options == rhs._options;
}

/*!
    Returns true if the \a lhs options and the \a rhs options have different
    contents; otherwise returns false.
*/
bool operator!=(const ImageOptions &lhs, const ImageOptions &rhs)
{
    return !(lhs == rhs);
}
