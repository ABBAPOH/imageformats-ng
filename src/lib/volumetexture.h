#ifndef VOLUMETEXTURE_H
#define VOLUMETEXTURE_H

#include "mipmappedimage.h"
#include <QtGui/QImage>

class VolumeTexture
{
public:
    VolumeTexture();

    int depth() const;

    QImage slice(int slice);
    void addSlice(const QImage &image);
    void removeSlice(int index);
    void setSlice(int index, const QImage &image);

    MipmappedImage mipmappedSlice(int index) const;
    void addMipmappedSlice(const MipmappedImage &image);
    void removeMipmappedSlice(int index);
    void setMipmappedSlice(int index, const MipmappedImage &image);

private:
    QVector<MipmappedImage> _images;
};

#endif // VOLUMETEXTURE_H
