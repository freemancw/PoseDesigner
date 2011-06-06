/*
===============================================================================
File:       HelpDialog.h
Author:     Clinton Freeman
Created:    June 6, 2011
===============================================================================
*/

#ifndef HELPDIALOG_H
#define HELPDIALOG_H

// Qt
#include <QDialog>

namespace Ui
{
    class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = 0);
    ~HelpDialog();

private:
    Ui::HelpDialog *ui;
};

#endif // HELPDIALOG_H
