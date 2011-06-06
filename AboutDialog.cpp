/*
===============================================================================
File:       AboutDialog.cpp
Author:     Clinton Freeman
Created:    May 15, 2011
===============================================================================
*/

// local
#include <AboutDialog.h>
#include <ui_aboutdialog.h>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
