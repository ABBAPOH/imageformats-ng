//! [0]
#include <QImageReader>
#include <QDebug>

void handleImage(const QImage &image);

int main(int argc, char *argv[])
{
    QImageReader reader("image.png");
    QImage image = reader.read();
    if (image.isNull()) {
        qWarning() << "Error reading image" << reader.errorString();
        return 1;
    }

    handleImage(image);
    return 0;
}
//! [0]


//! [1]
#include <ImageIO>
#include <QDebug>

void handleImage(const QImage &image);

int main(int argc, char *argv[])
{
    ImageIO reader("image.png");
    auto result = reader.read();
    ImageIOResult ok = result.first;
    if (!ok) {
        qWarning() << "Error reading image:" << ok.toString();
        return 1;
    }
    ImageContents contents = result.second;
    handleImage(contents.image());
    return 0;
}
//! [1]


//! [2]
#include <ImageIO>
#include <QDebug>

QImage createImage();

int main(int argc, char *argv[])
{
    QImage image = createImage();
    ImageIO writer("image.png");
    ImageContents contents(image);
    ImageIOResult ok = writer.write(contents);
    if (!ok) {
        qWarning() << "Error writing image:" << ok.toString();
        return 1;
    }
    return 0;
}
//! [2]


//! [3]
#include <ImageIO>
#include <QDebug>

int main(int argc, char *argv[])
{
    ImageIO reader("image.png");
    const auto result = reader.readHeader();
    if (!result.first) {
        qWarning() << "Error reading header:" << result.first.toString();
        return 1;
    }
    ImageHeader header = result.second;
    qDebug() << "size = " << header.size();
    qDebug() << "format = " << header.imageFormat();
    qDebug() << "image count = " << header.imageCount();
    qDebug() << "mipmap count = " << header.mipmapCount();

    const auto contentsResult = reader.readContents(header);
    // if (!contentsResult.first) ...

    return 0;
}
//! [3]


//! [4]
#include <ImageIO>
#include <QDebug>

int main(int argc, char *argv[])
{
    ImageIO reader("texture.dds");
    const auto result = reader.read();

    // if (!result.first) ...

    const auto &contents = result.second;
    for (int index = 0; index < contents.header().imageCount(); ++index) { // read array
        for (int level = 0; level < contents.header().mipmapCount(); ++level) { // read mipmaps
            ImageResource resource = contents.resource(index, level);
            if (resource.type() == ImageResource::Type::Image) {
                const QImage image = resource.image();
                // ...
            } else if (resource.type() == ImageResource::Type::CubeTexture) {
                CubeTexture texture = resource.cubeTexture();
                // ...
            } else if (resource.type() == ImageResource::Type::VolumeTexture) {
                VolumeTexture texture = resource.volumeTexture();
                // ...
            }
        }
    }

    ImageExifMeta exif = contents.exifMeta();
    if (!exif.isEmpty()) {
        // ...
    }

    return 0;
}
//! [4]
