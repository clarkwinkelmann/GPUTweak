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
#include "gpuinfowindow.h"
#include "ui_gpuinfowindow.h"

#include "gpunvidia.h"

GPUInfoWindow::GPUInfoWindow(GPU *gpu, QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::GPUInfoWindow)
{
    ui->setupUi(this);

    this->gpu = gpu;

    // Initial display of informations
    this->display();

    // To automatically update displayed informations
    connect(this->gpu, SIGNAL(updated()), this, SLOT(display()));
}

GPUInfoWindow::~GPUInfoWindow()
{
    delete ui;
}

/**
 * Updates the info window
 */
void GPUInfoWindow::display()
{
    this->setWindowTitle(QString("[%1] %2 - Informations").arg(this->gpu->getIdentifier()).arg(this->gpu->getName()));

    this->ui->nameInput         ->setText(this->gpu->getName());
    this->ui->driverVersionInput->setText(this->gpu->getDriverVersion());
    this->ui->busTypeInput      ->setText(this->gpu->getBusType());
    this->ui->busIdInput        ->setText(this->gpu->getBusId());

    if (GPUNvidia *ngpu = dynamic_cast<GPUNvidia*>(this->gpu)) {
        this->ui->cudaCoresInput->show();
        this->ui->cudaCoresInput->setText(QString::number(ngpu->getCudaCores()));
    } else {
        this->ui->cudaCoresInput->hide();
    }

    this->ui->totalMemoryInput       ->setText(QString("%1 MB") .arg(this->gpu->getTotalMemory()));

    this->ui->currentCoreTempInput   ->setText(QString("%1 °C") .arg(this->gpu->getCurrentCoreTemp()));
    this->ui->currentFanSpeedInput   ->setText(QString("%1 %")  .arg(this->gpu->getCurrentFanSpeed()));
    this->ui->currentCoreClockInput  ->setText(QString("%1 MHz").arg(this->gpu->getCurrentCoreClock()));
    this->ui->currentMemoryClockInput->setText(QString("%1 MHz").arg(this->gpu->getCurrentMemoryClock()));
    this->ui->currentCoreUseInput    ->setText(QString("%1 %")  .arg(this->gpu->getCurrentCoreUse()));
    this->ui->currentMemoryUseInput  ->setText(QString("%1 %")  .arg(this->gpu->getCurrentMemoryUse()));
}
