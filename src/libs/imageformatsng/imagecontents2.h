#pragma once

#include <QtCore/QSharedDataPointer>

#include "imagemipmap.h"
#include "cubetexture.h"
#include "volumetexture.h"

class ImageContents2Data;

class ImageContents2
{
    Q_GADGET
public:
    enum class Type {
        Invalid = 0x00,
        Image = 0x01,
        ImageArray = 0x02,
        Cubemap = 0x04,
        CubemapArray = 0x08,
        VolumeTexture = 0x10,
        // VolumeTextureArray = 0x20, // not supported
        ImageMipmaps = 0x40,
        ImageMipmapsArray = 0x80,
        CubemapMipmaps = 0x100,
        CubemapMipmapsArray = 0x200,
        VolumeTextureMipmaps = 0x400,
        // VolumeTextureMipmapsArray = 0x800 // not supported
    };
//    Q_ENUM(Type)

    ImageContents2();
    ImageContents2(const ImageContents2 &);

    explicit ImageContents2(const QImage &image);
    explicit ImageContents2(const QVector<QImage> &images);
    explicit ImageContents2(const CubeTexture &texture);
    explicit ImageContents2(const QVector<CubeTexture> &textures);
    explicit ImageContents2(const VolumeTexture &texture);
    explicit ImageContents2(const ImageMipmap &mipmaps);
    explicit ImageContents2(const QVector<ImageMipmap> &mipmaps);
    explicit ImageContents2(const CubeTextureMipmaps &mipmaps);
    explicit ImageContents2(const QVector<CubeTextureMipmaps> &mipmaps);
    explicit ImageContents2(const VolumeTextureMipmaps &mipmaps);

    ~ImageContents2();

    ImageContents2 &operator=(const ImageContents2 &);

    int arrayCount() const;
    int mipmapCount() const;

    QImage image(int index = 0, int level = 0) const;
    QVector<QImage> imageArray() const;

    CubeTexture cubeTexture(int index = 0, int level = 0) const;
    QVector<CubeTexture> cubeTextureArray() const;

    VolumeTexture volumeTexture(int index = 0, int level = 0) const;
    QVector<VolumeTexture> volumeTextureArray() const;

    ImageMipmap imageMipmaps(int index = 0) const;
    QVector<ImageMipmap> imageMipmapsArray() const;

    CubeTextureMipmaps cubeTextureMipmaps(int index = 0) const;
    QVector<CubeTextureMipmaps> cubeTextureMipmapsArray() const;

    VolumeTextureMipmaps volumeTextureMipmaps() const;

private:
    QSharedDataPointer<ImageContents2Data> data;
};
