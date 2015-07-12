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
#include "gpustatswindow.h"
#include "ui_gpustatswindow.h"

#include <QTimer>
#include <QGraphicsTextItem>

#include <math.h>

/**
 * Number of seconds showed on the graphs
 */
const int GRAPH_TIME_LENGTH_SECS = 60;
/**
 * Refresh the graph every n msecs
 */
const int GRAPH_REFRESH_MSECS = 1000;
/**
 * Rounding of units on the graphs
 */
const int GRAPH_ROUND_AT = 10;
/**
 * Default min temps
 */
const int TEMP_MIN = 30;
/**
 * Default max temp
 */
const int TEMP_MAX = 50;
/**
 * Distance between lines on a percent graph
 */
const int PERCENT_LINE_EVERY = 20;
/**
 * Distance between lines on a temp diagram
 */
const int TEMP_LINE_EVERY = 5;
/**
 * Time between cleanups of the memory
 */
const int CLEAN_AFTER_SECS = GRAPH_TIME_LENGTH_SECS / 2;
/**
 * Approx. height of a text line in the graph for margins
 */
const int TEXT_LINE_HEIGHT = 26;

/**
 * Constant for the max of a percentage
 */
const int PERCENT_MAX = 100;
/**
 * Constant for the min of a percentage
 */
const int PERCENT_MIN = 0;
/**
 * Constant for the number of msecs in a sec
 */
const int MSEC_IN_A_SEC = 1000;

GPUStatsWindow::GPUStatsWindow(GPU *gpu, QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::GPUStatsWindow)
{
    ui->setupUi(this);

    this->gpu = gpu;

    this->setWindowTitle(QString("[%1] %2 - Stats").arg(this->gpu->getIdentifier()).arg(this->gpu->getName()));

    connect(this->gpu, SIGNAL(updated()), this, SLOT(newValues()));

    this->gpuTempScene = new QGraphicsScene(this->ui->gpuTempGraphic->rect());
    this->ui->gpuTempGraphic->setFrameShape(QFrame::NoFrame);
    this->ui->gpuTempGraphic->setScene(this->gpuTempScene);

    this->gpuUseScene = new QGraphicsScene(this->ui->gpuUseGraphic->rect());
    this->ui->gpuUseGraphic->setFrameShape(QFrame::NoFrame);
    this->ui->gpuUseGraphic->setScene(this->gpuUseScene);

    this->memoryUseScene = new QGraphicsScene(this->ui->memoryUseGraphic->rect());
    this->ui->memoryUseGraphic->setFrameShape(QFrame::NoFrame);
    this->ui->memoryUseGraphic->setScene(this->memoryUseScene);

    lastCleanup.start();

    this->tick();
}

GPUStatsWindow::~GPUStatsWindow()
{
    delete ui;
}

/**
 * Computes data fro a graph then draw it
 * @param scene               Scene to update with the data
 * @param allValues           List of the historical values to compute
 * @param graphTimeLength     Length of the graph in seconds
 * @param defaultMin          Default min value to use
 * @param defaultMax          Default max value to use
 * @param roundInterval       Min and max values are rounded according to this interval
 * @param lineEveryN          Distance between the horizontal lines in the background given in the unit beeing displayed
 * @param preventLineOnBorder If the graph line touches the border, this attribute will automatically add margin to prevent it
 */
void GPUStatsWindow::updateGraph(QGraphicsScene *scene, QList<HistoryValue> allValues, int graphTimeLength, int defaultMin, int defaultMax, int roundInterval, int lineEveryN, bool preventLineOnBorder)
{
    QTime graphEnd = QTime::currentTime();
    QTime graphStart = graphEnd.addSecs(-graphTimeLength);

    int i = allValues.size()-1;

    QList<HistoryValue> drawValues;
    int minVal = defaultMin;
    int maxVal = defaultMax;

    bool valueAtMax = false;

    while(i >= 0 && allValues.at(i).time > graphStart)
    {
        HistoryValue val = allValues.at(i);
        drawValues.append(val);

        if(val.value < minVal) {
            minVal = val.value;
        }

        if(val.value >= maxVal) {
            maxVal = val.value;
            valueAtMax = true;
        }

        i--;
    }

    if(valueAtMax && preventLineOnBorder) {
        // To prevent from touching the top
        maxVal++;
    }

    maxVal = ceil(static_cast<double>(maxVal)/roundInterval) * roundInterval;
    minVal = floor(static_cast<double>(minVal)/roundInterval) * roundInterval;

    this->updateGraphScene(scene, drawValues, minVal, maxVal, graphStart, graphEnd, lineEveryN);
}

/**
 * Draws a given graph
 * @param scene      Scene to update with the data
 * @param values     Values to display on the scene
 * @param minVal     Min value to display
 * @param maxVal     Max value to display
 * @param graphStart Earlier time displayed on the graph
 * @param graphEnd   Last time displayed on the graph
 * @param lineEveryN Distance between the horizontal lines in the background given in the unit beeing displayed
 */
void GPUStatsWindow::updateGraphScene(QGraphicsScene *scene, QList<HistoryValue> values, int minVal, int maxVal, QTime graphStart, QTime graphEnd, int lineEveryN)
{
    scene->clear();

    double valInterval = static_cast<double>(maxVal - minVal);

    for(int i=0; i<scene->height(); i+= scene->height() / valInterval * lineEveryN)
    {
        if(i == 0) {
            // no need to draw on the edge
            continue;
        }

        // TODO: also prevent from drawing the last line (which is not exactly on the border due to rounding)

        scene->addLine(0, i, scene->width(), i, QPen(QColor(240, 240, 240), 1));
    }

    QGraphicsTextItem *maxValText = scene->addText(QString::number(maxVal));
    maxValText->setPos(0, 0);
    QGraphicsTextItem *minValText = scene->addText(QString::number(minVal));
    minValText->setPos(0, scene->height() - TEXT_LINE_HEIGHT);

    for(int i=0; i<values.size()-1; i++)
    {
        HistoryValue val = values.at(i);
        HistoryValue nextVal = values.at(i+1);

        double x1 = static_cast<double>(graphStart.secsTo(val.time)) / graphStart.secsTo(graphEnd) * scene->width();
        double y1 = scene->height() - static_cast<double>(val.value - minVal) / valInterval * scene->height();
        double x2 = static_cast<double>(graphStart.secsTo(nextVal.time)) / graphStart.secsTo(graphEnd) * scene->width();
        double y2 = scene->height() - static_cast<double>(nextVal.value - minVal) / valInterval * scene->height();

        scene->addLine(x1, y1, x2, y2, QPen(Qt::darkBlue, 2));

        if(i == 0) {
            QGraphicsTextItem *minValText = scene->addText(QString::number(val.value));
            minValText->setPos(scene->width() - TEXT_LINE_HEIGHT, y1 > TEXT_LINE_HEIGHT ? y1 - TEXT_LINE_HEIGHT : y1);
        }
    }
}

/**
 * Cleans the history content to prevent from eating the whole RAM if the user decides to go on vacation leaving this app open
 */
void GPUStatsWindow::cleanValues()
{
    QTime now = QTime::currentTime();

    int deleteIfMoreThanMsecs = GRAPH_TIME_LENGTH_SECS * MSEC_IN_A_SEC;

    while(this->gpuTempHistory.size() && this->gpuTempHistory.first().time.msecsTo(now) > deleteIfMoreThanMsecs) {
        this->gpuTempHistory.removeFirst();
    }

    while(this->gpuUseHistory.size() && this->gpuUseHistory.first().time.msecsTo(now) > deleteIfMoreThanMsecs) {
        this->gpuUseHistory.removeFirst();
    }

    while(this->memoryUseHistory.size() && this->memoryUseHistory.first().time.msecsTo(now) > deleteIfMoreThanMsecs) {
        this->memoryUseHistory.removeFirst();
    }
}

/**
 * Updates the GUI
 */
void GPUStatsWindow::display()
{
    this->gpuTempScene  ->setSceneRect(this->ui->gpuTempGraphic  ->rect());
    this->gpuUseScene   ->setSceneRect(this->ui->gpuUseGraphic   ->rect());
    this->memoryUseScene->setSceneRect(this->ui->memoryUseGraphic->rect());

    // GPU Temp Graph (°C)
    this->updateGraph(this->gpuTempScene,   this->gpuTempHistory,   GRAPH_TIME_LENGTH_SECS, TEMP_MIN,    TEMP_MAX,    GRAPH_ROUND_AT, TEMP_LINE_EVERY, true);
    // GPU Use Graph (%)
    this->updateGraph(this->gpuUseScene,    this->gpuUseHistory,    GRAPH_TIME_LENGTH_SECS, PERCENT_MIN, PERCENT_MAX, GRAPH_ROUND_AT, PERCENT_LINE_EVERY);
    // GPU Temp Graph (%)
    this->updateGraph(this->memoryUseScene, this->memoryUseHistory, GRAPH_TIME_LENGTH_SECS, PERCENT_MIN, PERCENT_MAX, GRAPH_ROUND_AT, PERCENT_LINE_EVERY);
}

/**
 * Store new values when they change on the GPU
 */
void GPUStatsWindow::newValues()
{
    QTime time = QTime::currentTime();

    HistoryValue gpuTemp;
    gpuTemp.time = time;
    gpuTemp.value = this->gpu->getCurrentCoreTemp();
    this->gpuTempHistory.append(gpuTemp);

    HistoryValue gpuUse;
    gpuUse.time = time;
    gpuUse.value = this->gpu->getCurrentCoreUse();
    this->gpuUseHistory.append(gpuUse);

    HistoryValue memoryUse;
    memoryUse.time = time;
    memoryUse.value = this->gpu->getCurrentMemoryUse();
    this->memoryUseHistory.append(memoryUse);

    if(lastCleanup.elapsed() > CLEAN_AFTER_SECS * MSEC_IN_A_SEC) {
        this->cleanValues();
        lastCleanup.restart();
    }
}

/**
 * Tick to refresh the GUI periodically
 */
void GPUStatsWindow::tick()
{
    QTimer::singleShot(GRAPH_REFRESH_MSECS, this, SLOT(tick()));

    this->display();
}
