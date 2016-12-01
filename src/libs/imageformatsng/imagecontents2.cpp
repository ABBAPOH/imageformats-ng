#include "imagecontents2.h"

class ImageContents2Data : public QSharedData
{
public:

};

ImageContents2::ImageContents2() : data(new ImageContents2Data)
{

}

ImageContents2::ImageContents2(const ImageContents2 &rhs) : data(rhs.data)
{

}

ImageContents2 &ImageContents2::operator=(const ImageContents2 &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

ImageContents2::~ImageContents2()
{

}
