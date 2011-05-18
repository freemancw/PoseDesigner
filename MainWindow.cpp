/*
===============================================================================
File:       MainWindow.cpp
Author:     Clinton Freeman
Created:    May 13, 2011
===============================================================================
*/

// Qt includes
#include <QtGui>

// OpenNI includes
#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>

// system includes
#include <string>
#include <vector>

// local includes
#include <MainWindow.h>
#include <KinectOptionsDialog.h>
#include <AboutDialog.h>
#include <ui_mainwindow.h>
#include <KinectDrawer.h>
#include <KinectInfo.h>
#include <PoseSample.h>

static std::vector<PoseSample> samples;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionPoseDesigner_Help_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Yo dawg.");
    msgBox.exec();
}

void MainWindow::on_actionAbout_PoseDesigner_triggered()
{
    AboutDialog *aDialog = new AboutDialog(this);
    aDialog->show();

    /*
    QMessageBox msgBox;
    msgBox.setIconPixmap(QPixmap("C:/Users/Clinton/Desktop/biglogo.png"));
    msgBox.setDefaultButton(QMessageBox::Ok);
    std::string msg;

    msg  = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. ";
    msg += "Donec accumsan erat tristique orci sagittis adipiscing. ";
    msg += "Maecenas sit amet lorem risus, vitae auctor velit. ";
    msg += "Cras accumsan nisi ac massa consectetur non egestas justo interdum. ";
    msg += "Fusce eu nisi nunc. Suspendisse at nisi quis elit congue pharetra. ";

    msgBox.setText(msg.c_str());
    msgBox.exec();
    */
}

void MainWindow::on_buttonTakeSample_clicked()
{
    // first, grab relevant portion of GLWidget framebuffer, this is important
    // to do first because a person who is timing themselves won't be able to
    // hit confirm and have his image saved correctly
    PoseSample newSample;
    GLWidget *kw = ui->kinectWidget;
    QImage image;

    int side = qMin(kw->width(), kw->height());
    image = kw->grabFrameBuffer().copy((kw->width()  - side) / 2,
                                       (kw->height() - side) / 2,
                                       side, side);

    xn::SkeletonCapability sc = ki.userGenerator.GetSkeletonCap();
    if(sc.IsTracking(1))
    {
        // this only works because it's contiguous
        XnSkeletonJoint j;
        for(int i = XN_SKEL_HEAD; i <= XN_SKEL_RIGHT_FOOT; i++)
        {
            j = (XnSkeletonJoint)i;
            sc.GetSkeletonJointPosition(1, j, newSample.getJPositions()[j]);
        }
        newSample.calculateVectors();
    }

    bool accept;
    QString text = QInputDialog::getText(this, tr("New Sample"),
                                         tr("Sample name:"), QLineEdit::Normal,
                                         "New Sample", &accept);
    if(accept && !text.isEmpty())
    {
        // then set name and pass in image
        newSample.setName(text.toStdString());
        newSample.setImage(image);

        QString path = "C:/kinect/" + text + ".jpg";
        image.save(path, "JPEG");

        QListWidgetItem *wi = new QListWidgetItem(text, ui->listWidget);
        //wi->setFlags(wi->flags() | Qt::ItemIsEditable);
        ui->listWidget->addItem(wi);


        int row = ui->tableWidget->rowCount();
        int col = 0;

        // init the row
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setVerticalHeaderItem(row, new QTableWidgetItem(text));


        QTableWidgetItem *newItem;
        SkeletonVector sv;
        for(col = 0; col <= R_KNEE_FOOT; col++)
        {
            sv = (SkeletonVector)col;
            newItem = new QTableWidgetItem
            (QString::fromStdString(newSample.getJVector(sv).toString()));
            ui->tableWidget->setItem(row, col, newItem);
        }
        /*
        newItem = new QTableWidgetItem
        (QString::fromStdString(newSample.getJVector(NECK_HEAD).toString()));
        ui->tableWidget->setItem(row, col++, newItem);

        newItem = new QTableWidgetItem
        (QString::fromStdString(newSample.getJVector(SHOULDER_SHOULDER).toString()));
        ui->tableWidget->setItem(row, col++, newItem);

        newItem = new QTableWidgetItem
        (QString::fromStdString(newSample.getJVector(HIP_HIP).toString()));
        ui->tableWidget->setItem(row, col++, newItem);

        newItem = new QTableWidgetItem
        (QString::fromStdString(newSample.getJVector(L_SHOULDER_ELBOW).toString()));
        ui->tableWidget->setItem(row, col++, newItem);

        newItem = new QTableWidgetItem
        (QString::fromStdString(newSample.getJVector(L_ELBOW_HAND).toString()));
        ui->tableWidget->setItem(row, col++, newItem);

        for()
        */

        samples.push_back(newSample);
    }
}

void MainWindow::on_buttonTakeSampleTimer_clicked()
{
    bool accept;
    int time = QInputDialog::getInt(this, tr("Timed Sample"),
                                    tr("How long to wait:"),
                                    0, 0, 60, 1, &accept);

    if(accept)
    {
        // create timer to constantly redraw this widget
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(on_buttonTakeSample_clicked()));
        timer->setSingleShot(true);
        timer->start(time * 1000);
    }
}

void MainWindow::on_buttonRemoveSample_clicked()
{
    // TODO: need check here to make sure something is actually selected
    // in the list widget
    QListWidget *lw = this->ui->listWidget;
    lw->takeItem(lw->row(lw->currentItem()));
    ui->tableWidget->removeRow(samples.size()-1);
   // qDebug("%d", samples.size());
    samples.pop_back();
   // qDebug("%d", samples.size());
}

void MainWindow::on_buttonCalculate_clicked()
{

}

void MainWindow::on_buttonKinectSettings_clicked()
{
    KinectOptionsDialog *koDialog = new KinectOptionsDialog(this);
    koDialog->show();
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    QGraphicsScene *scene = new QGraphicsScene();

    if(current == NULL)
    {
        scene->setBackgroundBrush(Qt::NoBrush);
        ui->samplePreview->setScene(scene);
        ui->samplePreview->update();
        return;
    }

    int row = ui->listWidget->row(current);
    scene->setBackgroundBrush(Qt::black);
    scene->addPixmap(QPixmap::fromImage(samples.at(row).getImage().scaled(QSize(ui->samplePreview->width()-2, ui->samplePreview->height()-2))));

    ui->samplePreview->setScene(scene);
    ui->samplePreview->update();
}
