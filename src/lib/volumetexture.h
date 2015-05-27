#ifndef VOLUMETEXTURE_H
#define VOLUMETEXTURE_H

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

private:
    QVector<QImage> _images;
};

#endif // VOLUMETEXTURE_H
