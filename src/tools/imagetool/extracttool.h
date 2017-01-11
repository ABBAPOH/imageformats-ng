#pragma once

#include "abstracttool.h"
#include <QtCore/QCoreApplication>

namespace ImageTool {

class ExtractTool : public AbstractTool
{
    Q_DECLARE_TR_FUNCTIONS(ImageTool)
public:
    ExtractTool();

public: // AbstractTool interface
    QByteArray id() const override;
    QString decription() const override;
    int run(const QStringList &arguments) override;
};

} // namespace ImageTool
