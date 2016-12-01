#ifndef IMAGEMIPMAP_H
#define IMAGEMIPMAP_H

#include <QImage>

class ImageMipmap
{
public:
    ImageMipmap();
    explicit ImageMipmap(const QVector<QImage> &images);

    bool isValid() const;
    QSize size(int level = 0) const;
    int mipmapCount() const;
    QImage mipmap(int level = 0) const;

private:
    bool _valid {false};
    QVector<QImage> _images;
};

#endif // IMAGEMIPMAP_H
