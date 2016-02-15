/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef JPEGHANDLER_P_H
#define JPEGHANDLER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <ImageIOHandler>
#include <QtCore/QSize>
#include <QtCore/QRect>

class JpegHandlerPrivate;
class JpegHandler : public ImageIOHandler
{
public:
    JpegHandler();
    ~JpegHandler();

    bool canRead() Q_DECL_OVERRIDE;
    bool readHeader(ImageHeader &contents) Q_DECL_OVERRIDE;
    bool read(ImageContents &contents, const ImageOptions &options) Q_DECL_OVERRIDE;
    bool write(const ImageContents &contents, const ImageOptions &options) Q_DECL_OVERRIDE;

    static bool canRead(QIODevice *device);

    bool supportsOption(ImageOptions::Option option) const Q_DECL_OVERRIDE;

private:
    JpegHandlerPrivate *d;
};

class JpegHandlerPlugin : public ImageIOHandlerPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.arch.ImageDocument.JpegHandlerPlugin" FILE "jpeg.json")
public:
    QByteArray name() const Q_DECL_OVERRIDE;
    JpegHandler *create(QIODevice *device, const QMimeType &mimeType) Q_DECL_OVERRIDE;
    Capabilities capabilities(QIODevice *device, const QMimeType &mimeType) const Q_DECL_OVERRIDE;
};

#endif // JPEGHANDLER_P_H
