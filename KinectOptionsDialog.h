#ifndef KINECTOPTIONSDIALOG_H
#define KINECTOPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
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
