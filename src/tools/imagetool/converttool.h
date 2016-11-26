#pragma once

#include "abstracttool.h"

class ConvertTool : public AbstractTool
{
    Q_DECLARE_TR_FUNCTIONS(ImageTool)
public:
    ConvertTool();

    QByteArray id() const override;
    QString decription() const override;
    int run(const QStringList &arguments) override;
};
