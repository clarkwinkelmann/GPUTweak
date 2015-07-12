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
#ifndef GPUINFOWINDOW_H
#define GPUINFOWINDOW_H

#include <QWidget>

#include "gpu.h"

namespace Ui {
class GPUInfoWindow;
}

/**
 * Window displaying "all" values for a given GPU
 */
class GPUInfoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GPUInfoWindow(GPU *gpu, QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~GPUInfoWindow();

private:
    Ui::GPUInfoWindow *ui;
    GPU *gpu;

private slots:
    void display();

};

#endif // GPUINFOWINDOW_H
