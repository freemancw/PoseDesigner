/*!
 *  @file KinectOptionsDialog.h
 *  @author Clinton Freeman
 *  @date 5/13/2011
 *
 *  This file is primarily updated by the GUI editor.
 */

#ifndef KINECTOPTIONSDIALOG_H
#define KINECTOPTIONSDIALOG_H

// Qt
#include <QDialog>

namespace Ui
{
    class KinectOptionsDialog;
}

class KinectOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KinectOptionsDialog(QWidget *parent = 0);
    ~KinectOptionsDialog();

private slots:
    void on_drawBackground_toggled(bool checked);
    void on_drawSkeleton_toggled(bool checked);
    void on_drawPixels_toggled(bool checked);

private:
    Ui::KinectOptionsDialog *ui;
};

#endif // KINECTOPTIONSDIALOG_H
