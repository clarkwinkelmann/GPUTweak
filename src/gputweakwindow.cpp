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
#include "gputweakwindow.h"
#include "ui_gputweakwindow.h"

GPUTweakWindow::GPUTweakWindow(GPU *gpu, QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::GPUTweakWindow)
{
    ui->setupUi(this);

    this->gpu = gpu;

    this->setWindowTitle(QString("[%1] %2 - Tweak").arg(this->gpu->getIdentifier()).arg(this->gpu->getName()));

    this->resetValues();

    this->coreClockEnabled   = this->gpu->isCoreClockControlEnabled();
    this->memoryClockEnabled = this->gpu->isMemoryClockControlEnabled();

    if(this->gpu->isFanControlEnabled()) {
        this->fanSpeedEnabled = true;
        this->fanSpeed = this->gpu->getCurrentFanSpeed();
    }

    this->valuesChanged = false;

    this->display();
}

GPUTweakWindow::~GPUTweakWindow()
{
    delete ui;
}

/**
 * Resets all values to their "default" which means no manual control and arbitrary slider values
 */
void GPUTweakWindow::resetValues()
{
    this->coreClockEnabled = false;
    this->coreClock = 300;
    this->memoryClockEnabled = false;
    this->memoryClock = 500;
    this->fanSpeedEnabled = false;
    this->fanSpeed = 60;
}

/**
 * Saves the temporary settings on the GPU and refreshes the GUI
 */
void GPUTweakWindow::save()
{
    if(!this->valuesChanged) {
        return;
    }

    if(this->fanSpeedEnabled) {
        this->gpu->setFanControlEnabled(true);
        this->gpu->setFanSpeed(this->fanSpeed);
    } else {
        this->gpu->setFanControlEnabled(false);
    }

    this->valuesChanged = false;

    this->display();
}

/**
 * Refreshes the GUI
 */
void GPUTweakWindow::display()
{
    // Fan speed

    bool fanControlAvailable = this->gpu->isFanControlAvailable();

    if(fanControlAvailable) {
        this->ui->fanSpeedInput->setText(QString::number(this->fanSpeed));
        this->ui->fanSpeedSlider->setValue(this->fanSpeed);
    }

    this->ui->fanSpeedAutoCheckbox->setChecked(!fanControlAvailable || !this->fanSpeedEnabled);
    this->ui->fanSpeedAutoCheckbox->setDisabled(!fanControlAvailable);
    this->ui->fanSpeedInput->setDisabled(!fanControlAvailable || !this->fanSpeedEnabled);
    this->ui->fanSpeedSlider->setDisabled(!fanControlAvailable || !this->fanSpeedEnabled);

    this->ui->applyBtn->setDisabled(!this->valuesChanged);
}

/**
 * Handles fan speed change from the input
 * @param newText
 */
void GPUTweakWindow::on_fanSpeedInput_textEdited(QString newText)
{
    int newValue = newText.toInt();

    if(newValue > 100) {
        newValue = 100;
    }

    if(newValue < 0) {
        newValue = 0;
    }

    if(newValue != this->fanSpeed) {
        this->fanSpeed = newValue;
        this->valuesChanged = true;

        this->display();
    }
}

/**
 * Handles fan speed change from the slider
 * @param newValue
 */
void GPUTweakWindow::on_fanSpeedSlider_valueChanged(int newValue)
{
    if(newValue != this->fanSpeed) {
        this->fanSpeed = newValue;
        this->valuesChanged = true;

        this->display();
    }
}

/**
 * Handles fan speed change from/to auto
 * @param newState
 */
void GPUTweakWindow::on_fanSpeedAutoCheckbox_stateChanged(int newState)
{
    bool enabled = newState == Qt::Unchecked;
    if(enabled != this->fanSpeedEnabled) {
        this->fanSpeedEnabled = enabled;
        this->valuesChanged = true;

        this->display();
    }
}

/**
 * Handles reset button click
 */
void GPUTweakWindow::on_resetBtn_clicked()
{
    this->resetValues();
    this->valuesChanged = true;
    this->display();
}

/**
 * Handles apply button click
 */
void GPUTweakWindow::on_applyBtn_clicked()
{
    this->save();
}
