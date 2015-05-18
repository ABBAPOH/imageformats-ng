#ifndef IMAGEINDEX_H
#define IMAGEINDEX_H

#include <QSharedDataPointer>

class ImageIndexData;

class ImageIndex
{
public:
    ImageIndex();
    ImageIndex(const ImageIndex &other);
    ImageIndex &operator=(const ImageIndex &other);
    ~ImageIndex();

    int mipmap() const;
    void setMipmap(int mipmap);

    int frame() const;
    void setFrame(int frame);

private:
    QSharedDataPointer<ImageIndexData> d;
};

bool operator ==(const ImageIndex &lhs, const ImageIndex &rhs);

inline bool operator !=(const ImageIndex &lhs, const ImageIndex &rhs)
{
    return !(lhs == rhs);
}

bool operator <(const ImageIndex &lhs, const ImageIndex &rhs);
bool operator >(const ImageIndex &lhs, const ImageIndex &rhs);

#endif // IMAGEINDEX_H
