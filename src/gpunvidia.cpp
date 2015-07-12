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
#include "gpunvidia.h"

#include "nvidiasettingsadapter.h"

GPUNvidia::GPUNvidia(int ID, QString Name) : GPU()
{
    this->id   = ID;
    this->name = Name;

    this->fetchConstants();
    this->fetchVariables();
}

GPUNvidia::~GPUNvidia()
{
    // no-op
}

void GPUNvidia::fetchConstants()
{
    this->nvidiaDriverVersion     = this->queryStringAttribute("NvidiaDriverVersion");
    this->pcieMaxLinkWidth        = this->queryIntAttribute("PCIEMaxLinkWidth");
    this->pcieCurrentLinkWidth    = this->queryIntAttribute("PCIECurrentLinkWidth");
    this->pcieGen                 = this->queryIntAttribute("PCIEGen");
    this->pciBus                  = this->queryIntAttribute("PCIBus");
    this->pciDevice               = this->queryIntAttribute("PCIDevice");
    this->pciFunc                 = this->queryIntAttribute("PCIFunc");
    this->totalDedicatedGPUMemory = this->queryIntAttribute("TotalDedicatedGPUMemory");
    this->cudaCores               = this->queryIntAttribute("CUDACores");
}

void GPUNvidia::fetchVariables()
{
    this->gpuCoreTemp            = this->queryIntAttribute("GPUCoreTemp");

    QString currentFreqsString  = this->queryStringAttribute("GPUCurrentClockFreqsString");
    this->gpuCurrentNvClock     = NvidiaSettingsAdapter::getValueFromAttributesList(currentFreqsString, "nvclock");
    this->gpuCurrentMemoryClock = NvidiaSettingsAdapter::getValueFromAttributesList(currentFreqsString, "memclock");

    QString gpuUtilization      = this->queryStringAttribute("GPUUtilization");
    this->currentCoreUse        = NvidiaSettingsAdapter::getValueFromAttributesList(gpuUtilization, "graphics");
    this->currentMemoryUse      = NvidiaSettingsAdapter::getValueFromAttributesList(gpuUtilization, "memory");

    this->currentFanSpeed       = NvidiaSettingsAdapter::queryAtrribute(QString("[fan:%1]/GPUCurrentFanSpeed").arg(this->id)).toInt();

    this->fanControlState       = this->queryIntAttribute("GPUFanControlState") == 1;

    emit updated();
}

QString GPUNvidia::queryStringAttribute(QString attribute)
{
    return NvidiaSettingsAdapter::queryAtrribute(QString("[gpu:%1]/%2").arg(this->id).arg(attribute));
}

int GPUNvidia::queryIntAttribute(QString attribute)
{
    return this->queryStringAttribute(attribute).toInt();
}

QString GPUNvidia::getIdentifier()
{
    return QString("gpu:%1").arg(this->id);
}

QString GPUNvidia::getName()
{
    return this->name;
}

QString GPUNvidia::getDriverVersion()
{
    return this->nvidiaDriverVersion;
}

QString GPUNvidia::getBusType()
{
    return QString("PCI-E x%1 Gen%2 @ x%3")
            .arg(this->pcieMaxLinkWidth)
            .arg(this->pcieGen)
            .arg(this->pcieCurrentLinkWidth);
}

QString GPUNvidia::getBusId()
{
    return QString("PCI:%1:%2:%3")
            .arg(this->pciBus)
            .arg(this->pciDevice)
            .arg(this->pciFunc);
}

int GPUNvidia::getTotalMemory()
{
    return this->totalDedicatedGPUMemory;
}

int GPUNvidia::getCudaCores()
{
    return this->cudaCores;
}

int GPUNvidia::getCurrentCoreTemp()
{
    return this->gpuCoreTemp;
}

int GPUNvidia::getCurrentFanSpeed()
{
    return this->currentFanSpeed;
}

int GPUNvidia::getCurrentCoreClock()
{
    return this->gpuCurrentNvClock;
}

int GPUNvidia::getCurrentMemoryClock()
{
    return this->gpuCurrentMemoryClock;
}

int GPUNvidia::getCurrentCoreUse()
{
    return this->currentCoreUse;
}

int GPUNvidia::getCurrentMemoryUse()
{
    return this->currentMemoryUse;
}

bool GPUNvidia::isFanControlAvailable()
{
    return true; // TODO: there should be a check but the values do not seem to come from nvidia-settings cli
}

bool GPUNvidia::isFanControlEnabled()
{
    return this->fanControlState;
}

bool GPUNvidia::isCoreClockControlAvailable()
{
    return false; // TODO
}

bool GPUNvidia::isCoreClockControlEnabled()
{
    return false; // TODO
}

bool GPUNvidia::isMemoryClockControlAvailable()
{
    return false; // TODO
}

bool GPUNvidia::isMemoryClockControlEnabled()
{
    return false; // TODO
}

void GPUNvidia::setFanControlEnabled(bool enabled)
{
    NvidiaSettingsAdapter::setAttribute(QString("[gpu:%1]/GPUFanControlState").arg(this->id), static_cast<int>(enabled));

    this->fetchVariables();
}

void GPUNvidia::setFanSpeed(int speed)
{
    if(!this->isFanControlEnabled()) {
        // Setting fan speed when not in manual mode would trigger an error
        return;
    }

    NvidiaSettingsAdapter::setAttribute(QString("[fan:%1]/GPUCurrentFanSpeed").arg(this->id), speed);

    this->fetchVariables();
}
