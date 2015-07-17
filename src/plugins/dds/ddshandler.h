/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Copyright (C) 2013 Ivan Komissarov.
** Contact: http://www.qt-project.org/legal
**
** This file is part of the DDS plugin in the Qt ImageFormats module.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DDSHANDLER_H
#define DDSHANDLER_H

#include <imageiohandler.h>
#include "ddsheader.h"

class DDSHandler : public ImageIOHandler
{
public:
    DDSHandler();

    QByteArray name() const;

    bool canRead() Q_DECL_OVERRIDE;
    bool readHeader(ImageContents &contents);
    bool read(ImageContents &contents, const ReadOptions &options);
    bool write(const ImageContents &contents, const WriteOptions &options);

//    QVariant option(QImageIOHandler::ImageOption option) const;
//    void setOption(ImageOption option, const QVariant &value);
//    bool supportsOption(QImageIOHandler::ImageOption option) const;

    static bool canRead(QIODevice *device);

private:
    bool doScan() const;
    bool verifyHeader(const DDSHeader &dds) const;

private:
    enum ScanState {
        ScanError = -1,
        ScanNotScanned = 0,
        ScanSuccess = 1,
    };

    DDSHeader m_header;
    int m_format;
    DDSHeaderDX10 m_header10;
    int m_currentImage;
    mutable ScanState m_scanState;
};

class DdsHandlerPlugin : public ImageIOHandlerPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY(DdsHandlerPlugin)
    Q_PLUGIN_METADATA(IID "org.arch.ImageDocument.DdsHandlerPlugin" FILE "dds.json")
public:
    DdsHandlerPlugin() {}
    DDSHandler *create() { return new DDSHandler(); }
    Capabilities capabilities() const Q_DECL_OVERRIDE;
};

#endif // DDSHANDLER_H
