/*!
 *  @file AboutDialog.h
 *  @author Clinton Freeman
 *  @date 5/13/2011
 *
 *  About dialog form generated by Qt Creator. Doesn't do much of anything.
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

// Qt
#include <QDialog>

namespace Ui
{
    class AboutDialog;
}

/*!
 *  @class AboutDialog
 */
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
