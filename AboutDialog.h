/*
===============================================================================
File:       AboutDialog.h
Author:     Clinton Freeman
Created:    May 13, 2011
===============================================================================
*/

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

// Qt
#include <QDialog>

namespace Ui
{
    class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
