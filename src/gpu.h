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
#ifndef GPU_H
#define GPU_H

#include <QObject>
#include <QString>

/**
 * Abstract class for a GPU of any brand or using any driver
 */
class GPU : public QObject
{
    Q_OBJECT

public:
    /**
     * Fetches all data that does not change during operation
     */
    virtual void    fetchConstants() = 0;
    /**
     * Fetches all data that can change during operation
     * Should emit an "updated" event if a value has changed
     */
    virtual void    fetchVariables() = 0;

    virtual QString getIdentifier() = 0;    // ex: gpu:0
    virtual QString getName() = 0;          // ex: GeForce GT 530
    virtual QString getDriverVersion() = 0; // ex: 331.113
    virtual QString getBusType() = 0;       // ex; PCI-E x16 Gen2 @ x16
    virtual QString getBusId() = 0;         // ex: PCI:1:0:0
    virtual int     getTotalMemory() = 0;   // MB

    virtual int     getCurrentCoreTemp() = 0;    // °C
    virtual int     getCurrentFanSpeed() = 0;    // %
    virtual int     getCurrentCoreClock() = 0;   // MHz
    virtual int     getCurrentMemoryClock() = 0; // MHz
    virtual int     getCurrentCoreUse() = 0;     // %
    virtual int     getCurrentMemoryUse() = 0;   // %

    virtual bool    isFanControlAvailable() = 0;
    virtual bool    isFanControlEnabled() = 0;
    virtual bool    isCoreClockControlAvailable() = 0;
    virtual bool    isCoreClockControlEnabled() = 0;
    virtual bool    isMemoryClockControlAvailable() = 0;
    virtual bool    isMemoryClockControlEnabled() = 0;

    /**
     * Enables manual control ofthe fans
     * @param enabled True to enable
     */
    virtual void    setFanControlEnabled(bool enabled) = 0;
    /**
     * Sets the fan speed
     * @param speed Speed in %
     */
    virtual void    setFanSpeed(int speed) = 0;

signals:
    /**
     * Signal that should be emitted whenever an attribute has changed
     */
    void updated();
};

#endif // GPU_H
