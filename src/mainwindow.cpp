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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QToolButton>

#include "gpuinfowindow.h"
#include "gputweakwindow.h"
#include "gpustatswindow.h"
#include "nvidiasettingsadapter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->gpus.append(NvidiaSettingsAdapter::getGPUs());

    for(int i=0; i < this->gpus.size(); i++) {
        GPU *gpu = this->gpus.at(i);

        QHBoxLayout *hBox = new QHBoxLayout();

        hBox->addWidget(new QLabel(QString("[%1] %2").arg(gpu->getIdentifier()).arg(gpu->getName())));

        QToolButton *infoBtn = new QToolButton();
        hBox->addWidget(infoBtn);
        QAction * infoAction = new QAction("Informations", infoBtn);
        QVariant infoActionData;
        infoActionData.setValue(i);
        infoAction->setData(infoActionData);
        infoBtn->setDefaultAction(infoAction);
        connect(infoAction, SIGNAL(triggered()), this, SLOT(openInfoWindow()));

        QToolButton *statsBtn = new QToolButton();
        hBox->addWidget(statsBtn);
        QAction * statsAction = new QAction("Stats", statsBtn);
        QVariant statsActionData;
        statsActionData.setValue(i);
        statsAction->setData(statsActionData);
        statsBtn->setDefaultAction(statsAction);
        connect(statsAction, SIGNAL(triggered()), this, SLOT(openStatsWindow()));

        QToolButton *tweakBtn = new QToolButton();
        hBox->addWidget(tweakBtn);
        QAction * tweakAction = new QAction("Tweak", tweakBtn);
        QVariant tweakActionData;
        tweakActionData.setValue(i);
        tweakAction->setData(tweakActionData);
        tweakBtn->setDefaultAction(tweakAction);
        connect(tweakAction, SIGNAL(triggered()), this, SLOT(openTweakWindow()));

        this->ui->gpusLayout->addLayout(hBox);
    }
    this->tick();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Tick that refreshes the variable data from the GPUs
 */
void MainWindow::tick()
{
    QTimer::singleShot(2000, this, SLOT(tick()));

    foreach(GPU *gpu, this->gpus) {
        gpu->fetchVariables();
    }
}

void MainWindow::openInfoWindow()
{
    QAction* action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);
    int gpuInd = action->data().value<int>();

    GPUInfoWindow *window = new GPUInfoWindow(this->gpus.at(gpuInd), this, Qt::Window);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

void MainWindow::openTweakWindow()
{
    QAction* action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);
    int gpuInd = action->data().value<int>();

    GPUTweakWindow *window = new GPUTweakWindow(this->gpus.at(gpuInd), this, Qt::Window);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

void MainWindow::openStatsWindow()
{
    QAction* action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);
    int gpuInd = action->data().value<int>();

    GPUStatsWindow *window = new GPUStatsWindow(this->gpus.at(gpuInd), this, Qt::Window);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}
