//! [0]
ImageIO reader("image.png");
const auto headerResult = reader.readHeader();
if (!headerResult.first) {
    qWarning() << "Error reading header:" << headerResult.first.toString();
    return 1;
}

ImageHeader header = headerResult.second;
handleHeader(header);

const auto contentsResult = reader.readContents(header);
if (!contentsResult.first) {
    qWarning() << "Error reading header:" << headerResult.first.toString();
    return 1;
}

ImageContents contents = contentsResult.second;
handleImage(contents.image());
//! [0]

//! [1]
ImageIO reader("image.png");
auto result = reader.read();
ImageIOResult ok = result.first;
if (!ok) {
    qWarning() << "Error reading image:" << ok.toString();
    return 1;
}
ImageContents contents = result.second;
handleHeader(contents.header());
handleImage(contents.image());
//! [1]


//! [2]
ImageIO reader("image.dds");
ImageOptions options;
options.setScaledSize(QSize(64, 64));
options.setSkipMipmaps(true);
auto result = reader.read(options);
// ...
//! [2]
