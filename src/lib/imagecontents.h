#ifndef IMAGECONTENTS_H
#define IMAGECONTENTS_H

#include <QtCore/QSharedDataPointer>
#include <QtGui/QImage>

#include "imageexifmeta.h"

class ImageContentsData;

class ImageContents
{
public:
    enum Type {
        Image,
        Animation,
        ImageArray,
        Cubemap,
        VolumeTexture
    };

    ImageContents();
    ImageContents(const ImageContents &other);
    ImageContents &operator=(const ImageContents &other);
    ~ImageContents();

    Type type() const;
    void setType(Type t);

    int imageCount() const;
    void setImageCount(int count);

    int mipmapCount() const;
    void setMipmapCount(int count);

    QImage image(int index = 0, int level = 0);
    void setImage(const QImage &image, int index = 0, int level = 0);

    int imageDelay(); // TODO: should we support separate delays for each frame?
    void setImageDelay(int delay); // TODO: this could be only called by handler. Move to DocData?

    int loopCount() const;
    void setLoopCount(int count);

    ImageExifMeta exifMeta() const;
    void setExifMeta(const ImageExifMeta &exif);

    void clear();

private:
    QSharedDataPointer<ImageContentsData> d;
};

#endif // IMAGECONTENTS_H
