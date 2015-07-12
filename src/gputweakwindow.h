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
#ifndef GPUTWEAKWINDOW_H
#define GPUTWEAKWINDOW_H

#include <QWidget>

#include "gpu.h"

namespace Ui {
class GPUTweakWindow;
}

/**
 * Window allowing different tweaks on the given GPU
 */
class GPUTweakWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GPUTweakWindow(GPU *gpu, QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~GPUTweakWindow();

private:
    void resetValues();
    void save();

    Ui::GPUTweakWindow *ui;
    GPU *gpu;

    bool coreClockEnabled;
    int  coreClock;
    bool memoryClockEnabled;
    int  memoryClock;
    bool fanSpeedEnabled;
    int  fanSpeed;
    bool valuesChanged;

private slots:
    void display();

    void on_fanSpeedInput_textEdited(QString newText);
    void on_fanSpeedSlider_valueChanged(int newValue);
    void on_fanSpeedAutoCheckbox_stateChanged(int newState);

    void on_resetBtn_clicked();
    void on_applyBtn_clicked();
};

#endif // GPUTWEAKWINDOW_H
