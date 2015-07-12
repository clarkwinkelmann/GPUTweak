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
#ifndef GPUSTATSWINDOW_H
#define GPUSTATSWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QTime>

#include "gpu.h"

namespace Ui {
class GPUStatsWindow;
}

/**
 * @brief Window displaying data in graphs
 */
class GPUStatsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GPUStatsWindow(GPU *gpu, QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~GPUStatsWindow();

    /**
     * Structure holding integer data along with the time it was stored
     */
    struct HistoryValue {
        QTime time;
        int   value;
    };

private:
    void updateGraph(QGraphicsScene *scene, QList<HistoryValue> allValues, int graphTimeLength, int defaultMin, int defaultMax, int roundInterval, int lineEveryN, bool preventLineOnBorder = false);
    void updateGraphScene(QGraphicsScene *scene, QList<HistoryValue> values, int minVal, int maxVal, QTime graphStart, QTime graphEnd, int lineEveryN);

    void cleanValues();

    Ui::GPUStatsWindow *ui;
    GPU *gpu;

    // Lists of stored data
    QList<HistoryValue> gpuTempHistory;
    QList<HistoryValue> gpuUseHistory;
    QList<HistoryValue> memoryUseHistory;

    // Pointers to the Scenes used by the graphs
    QGraphicsScene *gpuTempScene;
    QGraphicsScene *gpuUseScene;
    QGraphicsScene *memoryUseScene;

    QTime lastCleanup;

private slots:
    void display();
    void newValues();
    void tick();
};

#endif // GPUSTATSWINDOW_H
