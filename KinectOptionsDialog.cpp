/*
===============================================================================
File:       KinectOptionsDialog.cpp
Author:     Clinton Freeman
Created:    May 15, 2011
===============================================================================
*/

// local
#include <KinectOptionsDialog.h>
#include <ui_kinectoptionsdialog.h>

#include <KinectInfo.h>

KinectOptionsDialog::KinectOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KinectOptionsDialog)
{
    ui->setupUi(this);

    ui->drawBackground->setChecked(kinectInfo.bDrawBackground);
    ui->drawSkeleton->setChecked(kinectInfo.bDrawSkeleton);
    ui->drawPixels->setChecked(kinectInfo.bDrawPixels);
}

KinectOptionsDialog::~KinectOptionsDialog()
{
    delete ui;
}

void KinectOptionsDialog::on_drawBackground_toggled(bool checked)
{
    kinectInfo.bDrawBackground = checked;
    //kinectInfo->setDrawBackground(checked);
}

void KinectOptionsDialog::on_drawSkeleton_toggled(bool checked)
{
    kinectInfo.bDrawSkeleton = checked;
    //kinectInfo->setDrawSkeleton(checked);
}

void KinectOptionsDialog::on_drawPixels_toggled(bool checked)
{
    kinectInfo.bDrawPixels = checked;
    //kinectInfo->setDrawPixels(checked);
}
