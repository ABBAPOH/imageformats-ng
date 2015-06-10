#ifndef IMAGEMIPMAP_H
#define IMAGEMIPMAP_H

#include "imageresource.h"

class ImageMipmap
{
public:
    int resourceCount() const;
    ImageResource resource(int index = 0) const;
    void addResource(const ImageResource &resoure);
    void removeResource(int index = 0);
    void setResource(const ImageResource &resoure, int index = 0);

private:
    QVector<ImageResource> _resources;
};

#endif // IMAGEMIPMAP_H
