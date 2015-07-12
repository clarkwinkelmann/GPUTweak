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
#ifndef GPUNVIDIA_H
#define GPUNVIDIA_H

#include <QString>

#include "gpu.h"

/**
 * NVIDIA Card using the nvidia proprietary driver
 * Data is accessed trough the nvidia-settings command-line utility
 */
class GPUNvidia : public GPU
{
public:
    GPUNvidia(int id, QString name);
    ~GPUNvidia();

    void    fetchConstants();
    void    fetchVariables();

    QString getIdentifier();
    QString getName();
    QString getDriverVersion();
    QString getBusType();
    QString getBusId();
    int     getTotalMemory();
    int     getCudaCores();

    int     getCurrentCoreTemp();
    int     getCurrentFanSpeed();
    int     getCurrentCoreClock();
    int     getCurrentMemoryClock();
    int     getCurrentCoreUse();
    int     getCurrentMemoryUse();

    bool    isFanControlAvailable();
    bool    isFanControlEnabled();
    bool    isCoreClockControlAvailable();
    bool    isCoreClockControlEnabled();
    bool    isMemoryClockControlAvailable();
    bool    isMemoryClockControlEnabled();

    void    setFanControlEnabled(bool enabled);
    void    setFanSpeed(int speed);

private:
    QString queryStringAttribute(QString attribute);
    int     queryIntAttribute(QString attribute);

    // Constants
    int     id;   // nvidia id of the gpu (0-based)
    QString name; // card name, comes from the cards list

    // Constants, fetched from nvidia-settings
    QString nvidiaDriverVersion;     // ex: 331.113
    int     pcieMaxLinkWidth;        // ex: 16
    int     pcieCurrentLinkWidth;    // ex: 16
    int     pcieGen;                 // ex: 2
    int     pciBus;                  // 1st part of PCI address
    int     pciDevice;               // 2nd part of PCI address
    int     pciFunc;                 // 3rd part of PCI address
    int     totalDedicatedGPUMemory; // ex: 2048
    int     cudaCores;               // ex: 96

    // Variables, fetched from nvidia-settings
    int     gpuCoreTemp;             // ex: 37
    int     gpuCurrentNvClock;       // MHz
    int     gpuCurrentMemoryClock;   // MHz
    int     currentCoreUse;          // %
    int     currentMemoryUse;        // %
    int     currentFanSpeed;         // %
    bool    fanControlState;
};

#endif // GPUNVIDIA_H
