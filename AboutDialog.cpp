/*!
 *  @file AboutDialog.cpp
 *  @author Clinton Freeman
 *  @date 5/15/2011
 *
 *  About dialog implementation generated by the GUI editor. This dialog
 *  doesn't really do anything.
 */

// local
#include <AboutDialog.h>
#include <ui_aboutdialog.h>

/*!
 *  Create the About dialog.
 */
AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

/*!
 *  Destroy the About dialog.
 */
AboutDialog::~AboutDialog()
{
    delete ui;
}
