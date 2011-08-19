/*
===============================================================================
File:       MainWindow.h
Author:     Clinton Freeman
Created:    May 13, 2011

Comments:   This file is primarily updated by the GUI editor.
===============================================================================
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QtGui>
#include <QMainWindow>

// local
#include <KinectInfo.h>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    //KinectInfo* ki;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // Menubar Actions

    // File
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionExit_triggered();

    // Edit
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionDelete_triggered();
    void on_actionSelect_All_triggered();

    // Help
    void on_actionPoseDesigner_Help_triggered();
    void on_actionAbout_PoseDesigner_triggered();

    // "Big Buttons in the Middle" Actions
    void on_buttonTakeSample_clicked();
    void on_buttonTakeSampleTimer_clicked();
    void on_buttonRemoveSample_clicked();
    void on_buttonKinectSettings_clicked();

    // Various Widget Actions
    void on_sampleList_currentItemChanged
    (QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::MainWindow *ui;
    void calculateStats();
};

#endif // MAINWINDOW_H
