/*
===============================================================================
File:       MainWindow.cpp
Author:     Clinton Freeman
Created:    May 13, 2011
===============================================================================
*/

// Qt
#include <QtGui>

// OpenNI
#include <XnCppWrapper.h>

// system
#include <string>
#include <vector>

// local
#include <MainWindow.h>
#include <ui_mainwindow.h>
#include <KinectOptionsDialog.h>
#include <AboutDialog.h>
#include <HelpDialog.h>

#include <KinectDrawer.h>
#include <KinectInfo.h>
#include <Pose.h>
#include <PoseSample.h>

// xml test
#include <pugixml.hpp>

static Pose currentPose; // this needs to change
static QString currentFilename;
//KinectInfo* kinectInfo;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    initKinect();
    //kinectInfo = KinectInfo::getInstance();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    //KinectInfo::destroyInstance();
    delete ui;
}

void MainWindow::on_buttonTakeSample_clicked()
{   
    if(!kinectInfo.bConnected)
        return;

    PoseSample newSample;
    QImage image;
    //GLWidget *kw;

    // shorten expressions
    GLWidget* kw = ui->kinectWidget;

    // first, grab relevant portion of GLWidget framebuffer. this is important
    // to do first because a person who is timing themselves won't be able to
    // hit confirm and have his image saved correctly
    int side = qMin(kw->width(), kw->height());
    image = kw->grabFrameBuffer().copy((kw->width()  - side) / 2,
                                       (kw->height() - side) / 2,
                                       side, side);

    // shorten expressions, this should be a pointer, really
    xn::SkeletonCapability sc = kinectInfo.userGenerator.GetSkeletonCap();
    // currently we only track the first user
    if(sc.IsTracking(1))
    {
        // defined the ++ operator for XnSkeletonJoint in PoseSample.h...
        // OpenNI was not kind enough to do this, or to at least add a
        // SKEL_MAX ending marker to their enum :|
        for(XnSkeletonJoint sj = XN_SKEL_HEAD; sj <= XN_SKEL_RIGHT_FOOT; sj++)
        {
            qDebug("%d", sj);
            sc.GetSkeletonJointPosition(1, sj, newSample.getJPositions()[sj]);
        }

        newSample.calculateVectors();
    }

    // prompt user for sample name
    bool accept;
    QString text = QInputDialog::getText(this, tr("New Sample"),
                                         tr("Sample name:"), QLineEdit::Normal,
                                         "New Sample", &accept);
    if(accept && !text.isEmpty())
    {
        // then set name and pass in image
        newSample.setName(text.toStdString());
        newSample.setImage(image);

        // expand upon this
        //QString path = "C:/kinect/" + text + ".jpg";
        //image.save(path, "JPEG");

        // add the sample to the list
        QListWidgetItem *wi = new QListWidgetItem(text, ui->listWidget);
        ui->listWidget->addItem(wi);

        // add a row in the table
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setVerticalHeaderItem(row, new QTableWidgetItem(text));

        // populate the columns
        QTableWidgetItem *newItem;
        for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
        {
            newItem = new QTableWidgetItem
            (QString::fromStdString(newSample.getJVector(col).toString()));
            ui->tableWidget->setItem(row, col, newItem);
        }

        // add it to the sample collection for this pose
        currentPose.addSample(text.toStdString(), newSample);
    }
}

void MainWindow::on_buttonTakeSampleTimer_clicked()
{
    if(!kinectInfo.bConnected)
        return;

    bool accept;
    int time = QInputDialog::getInt(this, tr("Timed Sample"),
                                    tr("How long to wait:"),
                                    0, 0, 60, 1, &accept);

    if(accept)
    {
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this,
                SLOT(on_buttonTakeSample_clicked()));
        timer->setSingleShot(true);
        timer->start(time * 1000);
    }
}

void MainWindow::on_buttonRemoveSample_clicked()
{
    if(!kinectInfo.bConnected)
        return;

    // TODO: need check here to make sure something is actually selected
    // in the list widget
    QListWidget *lw = ui->listWidget;
    QTableWidget *tw = ui->tableWidget;

    // get the name of the item
    QString name = lw->currentItem()->text();

    // remove it from the current pose's samples
    currentPose.removeSample(name.toStdString());

    // remove it from the data table
    tw->removeRow(lw->row(lw->currentItem()));

    // remove it from the list
    lw->takeItem(lw->row(lw->currentItem()));
}

void MainWindow::on_buttonCalculate_clicked()
{
    if(!kinectInfo.bConnected)
        return;

    currentPose.calculateStatistics();
    // add a row in the table
    //int row = ui->statsWidget->rowCount();
    //ui->statsWidget->insertRow(row);
    //ui->statsWidget->setVerticalHeaderItem(row, new QTableWidgetItem("Mean"));

    // populate the columns
    QTableWidgetItem *newItem;

    for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
    {
        newItem = new QTableWidgetItem
        (QString::fromStdString(currentPose.getMean().getJVector(col).toString()));
        ui->statsWidget->setItem(0, col, newItem);
    }

    for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
    {
        newItem = new QTableWidgetItem
        (QString::fromStdString(currentPose.getStdDev().getJVector(col).toString()));
        ui->statsWidget->setItem(1, col, newItem);
    }
}

void MainWindow::on_buttonKinectSettings_clicked()
{
    if(!kinectInfo.bConnected)
        return;

    KinectOptionsDialog *koDialog = new KinectOptionsDialog(this);
    koDialog->show();
}

void MainWindow::on_listWidget_currentItemChanged
(QListWidgetItem* current, QListWidgetItem* previous)
{
    QGraphicsScene *scene = new QGraphicsScene();

    if(current == NULL)
    {
        scene->setBackgroundBrush(Qt::NoBrush);
        ui->samplePreview->setScene(scene);
        ui->samplePreview->update();
        return;
    }

    std::string name = ui->listWidget->currentItem()->text().toStdString();
    QImage image = currentPose.getSample(name).getImage();
    QSize size = QSize(ui->samplePreview->width()-2,
                       ui->samplePreview->height()-2);

    scene->addPixmap(QPixmap::fromImage(image.scaled(size)));

    ui->samplePreview->setScene(scene);
    ui->samplePreview->update();
}

void MainWindow::on_actionPoseDesigner_Help_triggered()
{
    HelpDialog *hDialog = new HelpDialog(this);
    hDialog->show();
}

void MainWindow::on_actionAbout_PoseDesigner_triggered()
{
    AboutDialog *aDialog = new AboutDialog(this);
    aDialog->show();
}


void MainWindow::on_actionCapture_triggered()
{
    QFileDialog::getOpenFileName(this,
        tr("Open Pose"), "./", tr("Pose Files (*.pose)")); //bl
}

const char *SkeletonVectorNames[] =
{
    "NECK_HEAD",
    "SHOULDER_SHOULDER",
    "HIP_HIP",
    "L_SHOULDER_ELBOW",
    "L_ELBOW_HAND",
    "L_SHOULDER_WAIST",
    "L_WAIST_HIP",
    "L_HIP_KNEE",
    "L_KNEE_FOOT",
    "R_SHOULDER_ELBOW",
    "R_ELBOW_HAND",
    "R_SHOULDER_WAIST",
    "R_WAIST_HIP",
    "R_HIP_KNEE",
    "R_KNEE_FOOT",
    "SKEL_VEC_MAX"
};

void saveFile(QString filename)
{
    pugi::xml_document xmlDoc;
    pugi::xml_node pose = xmlDoc.append_child("pose");

    for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
    {
        pugi::xml_node vector = pose.append_child(SkeletonVectorNames[col]);
        pugi::xml_node mean = vector.append_child("mean");
        pugi::xml_node mvalue = mean.append_child(pugi::node_pcdata);
        mvalue.set_value(currentPose.getMean().getJVector(col).toString().c_str());
        pugi::xml_node stddev = vector.append_child(("std_dev"));
        pugi::xml_node sdvalue = stddev.append_child(pugi::node_pcdata);
        sdvalue.set_value(currentPose.getStdDev().getJVector(col).toString().c_str());
    }

    xmlDoc.save_file(filename.toStdString().c_str());
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Pose As"), "./", tr("Pose Files (*.pose)"));

    saveFile(filename);

    this->setWindowTitle(filename + " - PoseDesigner");
    currentFilename = filename;
}

void MainWindow::on_actionExit_triggered()
{

}

void MainWindow::on_actionNew_triggered()
{
    if(!ui->listWidget->count())
        return;
    else
    {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if(ret == QMessageBox::Save)
        {
            on_actionSave_triggered();
        }
        else if(ret == QMessageBox::Discard)
        {
            ui->listWidget->clear();
            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);
            ui->statsWidget->clearContents();
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(!currentFilename.isEmpty())
        saveFile(currentFilename);
    else
        on_actionSaveAs_triggered();
}
