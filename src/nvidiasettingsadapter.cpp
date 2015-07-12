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
#include "nvidiasettingsadapter.h"

#include <QProcess>
#include <QRegularExpression>

#include "gpunvidia.h"

/**
 * nvidia-settings command line utility path
 */
const QString NVIDIA_SETTINGS_CMD = "nvidia-settings";

/**
 * Execute the given shell command and return the output
 * @param command Command to run
 * @return String containing all the output
 */
QString NvidiaSettingsAdapter::cmdLineProcess(QString command)
{
    QProcess process;
    process.start(command);
    process.waitForFinished(-1);
    return QString(process.readAllStandardOutput());
}

/**
 * Queries the driver for a string attribute
 * @param attribute Name of the attribute
 * @return Value of the attribute
 */
QString NvidiaSettingsAdapter::queryAtrribute(QString attribute)
{
    return NvidiaSettingsAdapter::cmdLineProcess(
                QString("%1 -t -q %2") // -q for data query, -t for value only
                .arg(NVIDIA_SETTINGS_CMD)
                .arg(attribute));
}

/**
 * Get a list of all GPUs detected by this adapter
 * @return List of GPUs
 */
QList<GPU*> NvidiaSettingsAdapter::getGPUs()
{
    QString out = NvidiaSettingsAdapter::cmdLineProcess(QString("%1 -q gpus").arg(NVIDIA_SETTINGS_CMD));

    // TODO: something special if the command fail ?

    QRegularExpression gpuLine("\\[gpu:(?<id>\\d+)\\] +\\((?<name>[A-Za-z0-9 ]+)\\)");

    QRegularExpressionMatchIterator i = gpuLine.globalMatch(out);

    QList<GPU*> list;

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        list.append(new GPUNvidia(match.captured("id").toInt(), match.captured("name")));
    }

    return list;
}

/**
 * Parses an integer attribute list from the nvidia-settings utility to get a given attribute
 * @param list String containing the chained values
 * @param key Key of the requested value
 * @return Value corresponding to the key or 0 if not found
 */
int NvidiaSettingsAdapter::getValueFromAttributesList(QString list, QString key)
{
    QRegularExpression regExp(QString("%1=(?<value>[0-9]+)").arg(key));

    QRegularExpressionMatch match = regExp.match(list);
    if (match.hasMatch()) {
        return match.captured("value").toInt();
    } else {
        throw "NO MATCH"; // TODO: better exception ?
    }

    return 0;
}

/**
 * Sets an attribute trough the nvidia-settings utility
 * @param attribute Name of the attribute
 * @param value String value to set
 */
void NvidiaSettingsAdapter::setAttribute(QString attribute, QString value)
{
    QString out = NvidiaSettingsAdapter::cmdLineProcess(QString("%1 -a \"%2=%3\"").arg(NVIDIA_SETTINGS_CMD).arg(attribute).arg(value));
}

/**
 * Sets an attribute trough the nvidia-settings utility
 * @param attribute Name of the attribute
 * @param value Integer value to set
 */
void NvidiaSettingsAdapter::setAttribute(QString attribute, int value)
{
    // Converts the number to string and use the base implementation
    NvidiaSettingsAdapter::setAttribute(attribute, QString::number(value));
}
