#ifndef IMAGEPAGE_H
#define IMAGEPAGE_H

#include "imageresource.h"
#include <QSharedDataPointer>

class ImagePageData;

class ImagePage
{
public:
    ImagePage();
    ImagePage(const ImagePage &other);
    ImagePage &operator=(const ImagePage &other);
    ~ImagePage();

    ImageResource resource() const;
    void setResource(const ImageResource &resource);

    ImageResource mipmap(int index = 0) const;
    void setMipmap(int index, const ImageResource &resurce);

    int mipmapCount() const;
    void setMipmapCount(int count);

private:
    QSharedDataPointer<ImagePageData> d;
};

#endif // IMAGEPAGE_H
