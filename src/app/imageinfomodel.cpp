#include "imageinfomodel.h"

namespace {

QString typeToString(const ImageHeader::Type type)
{
    // As we can't use QMetaEnum for QImage, use manual switch here.
    // At least, we can translate types.
    switch (type) {
    case ImageHeader::Invalid : return ImageInfoModel::tr("Invalid");
    case ImageHeader::Image : return ImageInfoModel::tr("Image");
    case ImageHeader::Animation : return ImageInfoModel::tr("Animation");
    case ImageHeader::ImageArray : return ImageInfoModel::tr("Image array");
    case ImageHeader::Cubemap : return ImageInfoModel::tr("Cubemap");
    case ImageHeader::VolumeTexture : return ImageInfoModel::tr("Volume texture");
    default: return ImageInfoModel::tr("Unknown");
    }
    Q_UNREACHABLE();
    return QString();
}

QString imageFormatToString(const QImage::Format format)
{
    switch (format) {
    case QImage::Format_Invalid : return ImageInfoModel::tr("Invalid");
    case QImage::Format_Mono : return ImageInfoModel::tr("Mono");
    case QImage::Format_MonoLSB : return ImageInfoModel::tr("MonoLSB");
    case QImage::Format_Indexed8 : return ImageInfoModel::tr("Indexed8");
    case QImage::Format_RGB32 : return ImageInfoModel::tr("RGB32");
    case QImage::Format_ARGB32 : return ImageInfoModel::tr("ARGB32");
    case QImage::Format_ARGB32_Premultiplied : return ImageInfoModel::tr("ARGB32_Premultiplied");
    case QImage::Format_RGB16 : return ImageInfoModel::tr("RGB16");
    case QImage::Format_ARGB8565_Premultiplied : return ImageInfoModel::tr("ARGB8565_Premultiplied");
    case QImage::Format_RGB666 : return ImageInfoModel::tr("RGB666");
    case QImage::Format_ARGB6666_Premultiplied : return ImageInfoModel::tr("RGB666_Premultiplied");
    case QImage::Format_RGB555 : return ImageInfoModel::tr("RGB555");
    case QImage::Format_ARGB8555_Premultiplied : return ImageInfoModel::tr("RGB555_Premultiplied");
    case QImage::Format_RGB888 : return ImageInfoModel::tr("RGB888");
    case QImage::Format_RGB444 : return ImageInfoModel::tr("RGB444");
    case QImage::Format_ARGB4444_Premultiplied : return ImageInfoModel::tr("ARGB444_Premultiplied");
    case QImage::Format_RGBX8888 : return ImageInfoModel::tr("RGBX8888");
    case QImage::Format_RGBA8888 : return ImageInfoModel::tr("RGBA8888");
    case QImage::Format_RGBA8888_Premultiplied : return ImageInfoModel::tr("RGBA8888_Premultiplied");
    case QImage::Format_BGR30 : return ImageInfoModel::tr("BGR30");
    case QImage::Format_A2BGR30_Premultiplied : return ImageInfoModel::tr("A2BGR30_Premultiplied");
    case QImage::Format_RGB30 : return ImageInfoModel::tr("RGB30");
    case QImage::Format_A2RGB30_Premultiplied : return ImageInfoModel::tr("A2RGB30_Premultiplied");
    case QImage::Format_Alpha8 : return ImageInfoModel::tr("Alpha8");
    case QImage::Format_Grayscale8 : return ImageInfoModel::tr("Grayscale8");
    default: return ImageInfoModel::tr("Unknown");
    }
    Q_UNREACHABLE();
    return QString();
}

QString sizeToString(const QSize &size)
{
    return ImageInfoModel::tr("%1 x %2").arg(size.width()).arg(size.height());
}

} // namespace

ImageInfoModel::ImageInfoModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

ImageHeader ImageInfoModel::header() const
{
    return _header;
}

void ImageInfoModel::setImageHeader(const ImageHeader &header)
{
    if (_header == header)
        return;

    beginResetModel();
    _header = header;
    endResetModel();
}

int ImageInfoModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return _header.isNull() ? 0 : RowCount;
}

int ImageInfoModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

QVariant ImageInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
            if (section == ColumnName)
                return tr("name");
            else if (section == ColumnValue)
                return tr("value");
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant ImageInfoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole) {
        if (index.column() == ColumnName) {
            switch (index.row()) {
            case RowType: return tr("Type");
            case RowSize: return tr("Size");
            case RowImageFormat: return tr("Image format");
            case RowName: return tr("Name");
            case RowImageCount: return tr("Image count");
            case RowMipmapCount: return tr("Mipmap count");
            case RowImageDelay: return tr("Image delay");
            case RowLoopCount: return tr("Loop count");
            default:
                break;
            }
        } else if (index.column() == ColumnValue) {
            if (_header.isNull())
                return QString();
            switch (index.row()) {
            case RowType: return typeToString(_header.type());
            case RowSize: return sizeToString(_header.size());
            case RowImageFormat: return imageFormatToString(_header.imageFormat());
            case RowName: return _header.name();
            case RowImageCount: return _header.imageCount();
            case RowMipmapCount: return _header.mipmapCount();
            case RowImageDelay: return _header.imageDelay();
            case RowLoopCount: return _header.loopCount();
            default:
                break;
            }
        }
    }

    return QVariant();
}
