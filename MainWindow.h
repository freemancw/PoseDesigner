/*
===============================================================================
File:       MainWindow.h
Author:     Clinton Freeman
Created:    May 13, 2011
===============================================================================
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QtGui>
#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionPoseDesigner_Help_triggered();
    void on_actionAbout_PoseDesigner_triggered();

    void on_buttonTakeSample_clicked();
    void on_buttonTakeSampleTimer_clicked();
    void on_buttonRemoveSample_clicked();
    void on_buttonCalculate_clicked();
    void on_buttonKinectSettings_clicked();

    void on_listWidget_currentItemChanged
    (QListWidgetItem* current, QListWidgetItem* previous);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
