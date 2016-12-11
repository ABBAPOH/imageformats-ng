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
    Optional<ImageContents> maybeContents = reader.read();
    if (!maybeContents) {
        qWarning() << "Error reading image:" << reader.error().errorString();
        return 1;
    }
    ImageContents contents = *maybeContents;
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
    if (!writer.write(contents)) {
        qWarning() << "Error writing image:" << writer.error().errorString();
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
    const auto maybeHeader = reader.readHeader();
    if (!maybeHeader) {
        qWarning() << "Error reading header:" << reader.error().errorString();
        return 1;
    }
    ImageHeader header = *maybeHeader;
    qDebug() << "size = " << header.size();
    qDebug() << "format = " << header.imageFormat();
    qDebug() << "image count = " << header.imageCount();
    qDebug() << "mipmap count = " << header.mipmapCount();
    return 0;
}
//! [3]


//! [4]
#include <ImageIO>
#include <QDebug>

int main(int argc, char *argv[])
{
    ImageIO reader("texture.dds");
    auto contents = reader.read();

    // if (!contents) ...

    for (int index = 0; index < contents->header().imageCount(); ++index) { // read array
        for (int level = 0; level < contents->header().mipmapCount(); ++level) { // read mipmaps
            ImageResource resource = contents->resource(index, level);
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

    ImageExifMeta exif = contents->exifMeta();
    if (!exif.isEmpty()) {
        // ...
    }

    return 0;
}
//! [4]
