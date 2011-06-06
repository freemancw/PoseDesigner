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
}

KinectOptionsDialog::~KinectOptionsDialog()
{
    delete ui;
}

void KinectOptionsDialog::on_drawBackground_toggled(bool checked)
{
    ki.bDrawBackground = checked;
}

void KinectOptionsDialog::on_drawSkeleton_toggled(bool checked)
{
    ki.bDrawSkeleton = checked;
}

void KinectOptionsDialog::on_drawPixels_toggled(bool checked)
{
    ki.bDrawPixels = checked;
}
