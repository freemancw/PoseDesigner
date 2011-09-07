/*!
 *  @file HelpDialog.cpp
 *  @author Clinton Freeman
 *  @date 6/6/2011
 *
 *  Currently unused.
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
