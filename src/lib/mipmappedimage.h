#ifndef MIPMAPPEDIMAGE_H
#define MIPMAPPEDIMAGE_H

#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

class MipmappedImageData;

class MipmappedImage
{
public:
    MipmappedImage();
    MipmappedImage(const QImage &image);
    MipmappedImage(const QVector<QImage> &images);
    MipmappedImage(const MipmappedImage &other);
    MipmappedImage &operator=(const MipmappedImage &);
    ~MipmappedImage();

    bool isEmpty() const;

    int mipmapCount() const;

    QVector<QImage> images() const;
    void setImages(const QVector<QImage> &images);

    QImage image() const;
    QImage image(int index) const;
    //QImage image(const QSize &size) const; // returns closest image to the size scaled to the size

private:
    QSharedDataPointer<MipmappedImageData> d;
};

#endif // MIPMAPPEDIMAGE_H
