#include "imageindex.h"

class ImageIndexData : public QSharedData
{
public:
    int mipmap;
    int frame;
    int slice;
    int layer;
};

ImageIndex::ImageIndex() :
    d(new ImageIndexData)
{
    d->mipmap = 0;
    d->frame = 0;
    d->slice = 0;
    d->layer = -1;
}

ImageIndex::ImageIndex(const ImageIndex &other) :
    d(other.d)
{
}

ImageIndex &ImageIndex::operator=(const ImageIndex &other)
{
    if (this != &other)
        d.operator=(other.d);
    return *this;
}

ImageIndex::~ImageIndex()
{
}

int ImageIndex::mipmap() const
{
    return d->mipmap;
}

void ImageIndex::setMipmap(int mipmap)
{
    d->mipmap = mipmap;
}

int ImageIndex::frame() const
{
    return d->frame;
}

void ImageIndex::setFrame(int frame)
{
    d->frame = frame;
}

int ImageIndex::layer() const
{
    return d->layer;
}

void ImageIndex::setLayer(int layer)
{
    d->layer = layer;
}

bool operator ==(const ImageIndex &lhs, const ImageIndex &rhs)
{
    return lhs.mipmap() == rhs.mipmap()
            && lhs.frame() == rhs.frame()
            && lhs.layer() == rhs.layer();
}

bool operator <(const ImageIndex &lhs, const ImageIndex &rhs)
{
    if (lhs.mipmap() != rhs.mipmap())
        return lhs.mipmap() < rhs.mipmap();
    else if (lhs.frame() != rhs.frame())
        return lhs.frame() < rhs.frame();
    else
        return lhs.layer() < rhs.layer();
}

bool operator >(const ImageIndex &lhs, const ImageIndex &rhs)
{
    if (lhs.mipmap() != rhs.mipmap())
        return lhs.mipmap() > rhs.mipmap();
    else if (lhs.frame() != rhs.frame())
        return lhs.frame() > rhs.frame();
    else
        return lhs.layer() > rhs.layer();
}
