/*
 * This file is part of the GPUTweak project, see README
 * Copyright (C) 2015 Clark Winkelmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef NVIDIASETTINGSADAPTER
#define NVIDIASETTINGSADAPTER

#include <QString>

#include "gpu.h"

/**
 * This namespace holds global functions used to access the NVIDIA driver data trough the nvidia-settings linux tool
 */
namespace NvidiaSettingsAdapter
{
    QString cmdLineProcess(QString command);

    QString queryAtrribute(QString attribute);

    void setAttribute(QString attribute, QString value);
    void setAttribute(QString attribute, int value);

    int getValueFromAttributesList(QString list, QString key);

    QList<GPU*> getGPUs();
}

#endif // NVIDIASETTINGSADAPTER

