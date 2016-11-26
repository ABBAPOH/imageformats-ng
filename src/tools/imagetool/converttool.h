#pragma once

#include "abstracttool.h"

class ConvertTool : public AbstractTool
{
public:
    ConvertTool();

    QByteArray id() const override;
    QString decription() const override;
    int run(const QStringList &arguments) override;
};
