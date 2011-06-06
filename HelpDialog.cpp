/*
===============================================================================
File:       HelpDialog.cpp
Author:     Clinton Freeman
Created:    June 6, 2011
===============================================================================
*/

// local
#include <HelpDialog.h>
#include <ui_helpdialog.h>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
}

HelpDialog::~HelpDialog()
{
    delete ui;
}
