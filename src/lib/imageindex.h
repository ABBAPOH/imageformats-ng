#ifndef IMAGEINDEX_H
#define IMAGEINDEX_H

#include <QSharedDataPointer>

class ImageIndexData;

class ImageIndex
{
public:
    enum Side {
        NoSides = 0x0,
        PositiveX = 0x1,
        NegaviveX = 0x2,
        PositiveY = 0x4,
        NegaviveY = 0x8,
        PositiveZ = 0x10,
        NegaviveZ = 0x20,
        AllSides = 0x3F
    };
    Q_DECLARE_FLAGS(Sides, Side)

    ImageIndex();
    ImageIndex(const ImageIndex &other);
    ImageIndex &operator=(const ImageIndex &other);
    ~ImageIndex();

    int mipmap() const;
    void setMipmap(int mipmap);

    int frame() const;
    void setFrame(int frame);

    Side side() const;
    void setSide(Side side);

    int slice() const;
    void setSlice(int slice);

    int layer() const;
    void setLayer(int layer);

private:
    QSharedDataPointer<ImageIndexData> d;
};

bool operator ==(const ImageIndex &lhs, const ImageIndex &rhs);

inline bool operator !=(const ImageIndex &lhs, const ImageIndex &rhs)
{
    return !(lhs == rhs);
}

#endif // IMAGEINDEX_H
