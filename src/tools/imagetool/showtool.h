#pragma once

#include "abstracttool.h"

class ShowTool : public AbstractTool
{
public:
    ShowTool();

    // AbstractTool interface
public:
    QByteArray id() const override;
    QString decription() const override;
    int run(const QStringList &arguments) override;
};
