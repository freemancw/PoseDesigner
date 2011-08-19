/*
===============================================================================
File:       MainWindow.cpp
Author:     Clinton Freeman
Created:    May 13, 2011
===============================================================================
*/

// Qt
#include <QtGui>
#include <QtXml>

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
#include <SkeletonVector.h>

static Pose currentPose;
static QString currentFilename;

// new way
//KinectInfo* kinectInfo;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // this uses the older kinectInfo_t struct which would be nice to replace
    // with KinectInfo class
    initKinect();

    // new way of doing things, broken ATM
    //kinectInfo = KinectInfo::getInstance();

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    kinectInfo.context.Shutdown();
    // new way
    //KinectInfo::destroyInstance();
    delete ui;
}

/*
===============================================================================
Menubar Actions
===============================================================================
*/

//File->New
void MainWindow::on_actionNew_triggered()
{
    // first thing we need to know is whether or not the user has modified
    // the current document. the only way a user can modify a document is
    // to add or remove samples. TODO: we need a mechanism to flag
    // modifications.

    if(!ui->listWidget->count())
    {
        this->setWindowTitle(tr("untitled.pose - PoseDesigner"));
        return;
    }

    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Save)
    {
        on_actionSave_triggered();
        return;
    }
    else if(ret == QMessageBox::Discard)
    {
        ui->listWidget->clear();
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        ui->statsWidget->clearContents();
        currentFilename = QString();
        this->setWindowTitle(tr("untitled.pose - PoseDesigner"));
    }
}

// File->Open
void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Pose"), "./", tr("Pose Files (*.pose)"));

    QFile inFile(filename);

    if(!inFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to read file " << filename << ".";
        return;
    }

    QDataStream inStream(&inFile);

    inStream >> currentPose;








    /*
    QMap<QString, QVector3D> qm;

    inStream >> qm;

    QMap<QString, QVector3D>::iterator iter;

    qDebug() << qm.count();

    for(iter = qm.begin(); iter != qm.end(); ++iter)
    {
        qDebug() << "Key: " << iter.key() << " Value: " << iter.value();
    }
    */


    //PoseSample samp1;

    /*
    QMap<SkeletonVector, QVector3D> qm;

    inStream >> qm;

    QMap<SkeletonVector, QVector3D>::iterator iter;

    for(iter = qm.begin(); iter != qm.end(); ++iter)
    {
        QVector3D vec = iter.value();
        qDebug() << "Key: " << iter.key() << " Value: " << QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z());
    }
    */




    /*
    inStream >> samp1;

    qDebug() << samp1.getName();

    //jPositions seems to work
    QMap<XnSkeletonJoint, XnSkeletonJointPosition> qm = samp1.getJPositions();

    QMap<XnSkeletonJoint, XnSkeletonJointPosition>::iterator iter;

    for(iter = qm.begin(); iter != qm.end(); ++iter)
    {
        XnSkeletonJointPosition sjp = iter.value();
        qDebug() << "Key: " << iter.key() << " Confidence: "
                 << sjp.fConfidence << " Position: " << QString("%1, %2, %3").arg(sjp.position.X).arg(sjp.position.Y).arg(sjp.position.Z);
    }


    QMap<SkeletonVector, QVector3D> jqm = samp1.getJVectors();

    qDebug() << jqm.count() << " " << jqm.begin().key();

    QMap<SkeletonVector, QVector3D>::iterator iter2;


    for(iter2 = jqm.begin(); iter2 != jqm.end(); ++iter2)
    {
        QVector3D vec = iter2.value();
        qDebug() << "Key: " << iter2.key() << " Value: " << QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z());
    }
    */


    /*
    QMap<XnSkeletonJoint, XnSkeletonJointPosition> qm;

    inStream >> qm;

    QMap<XnSkeletonJoint, XnSkeletonJointPosition>::iterator iter;

    for(iter = qm.begin(); iter != qm.end(); ++iter)
    {
        XnSkeletonJointPosition sjp = iter.value();
        qDebug() << "Key: " << iter.key() << " Confidence: "
                 << sjp.fConfidence << " Position: " << QString("%1, %2, %3").arg(sjp.position.X).arg(sjp.position.Y).arg(sjp.position.Z);
    }
    */

    /* this works
    QMap<SkeletonVector, QVector3D> qm;

    inStream >> qm;

    QMap<SkeletonVector, QVector3D>::iterator iter;

    for(iter = qm.begin(); iter != qm.end(); ++iter)
    {
        QVector3D vec = iter.value();
        qDebug() << "Key: " << iter.key() << " Value: " << QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z());
    }
    */

    /*
    QMap<SkeletonVector, QVector3D> qm;

    inStream >> qm;

    QMap<SkeletonVector, QVector3D>::iterator iter;

    for(iter = qm.begin(); iter != qm.end(); iter++)
    {
        SkeletonVector sv = iter.key();
        QVector3D vec = iter.value();
        qDebug() << "Key: " << sv << "Value: " << QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z());
    }
    */

    /*
    XnSkeletonJoint sj;
    inStream >> sj;

    qDebug() << "SJ: " << sj;
    */
    /*
    XnSkeletonJointPosition sjp;

    inStream >> sjp;

    qDebug() << "SJP Vector: " << QString("%1, %2, %3").arg(sjp.position.X).arg(sjp.position.Y).arg(sjp.position.Z);
    qDebug() << "SJP Confidence: " << sjp.fConfidence;
    */

    //Vector3 v;

   // inStream >> v;

   // qDebug() << v;

    //inStream >> currentPose;

    //qDebug() << currentPose.getMean().getName();
    //qDebug() << currentPose.getStdDev().getName();
    //qDebug() << currentPose.getSamples().count();

    //QMap<QString, PoseSample>::iterator iter;


    /*
    for(iter = currentPose.getSamples().begin(); iter != currentPose.getSamples().end(); iter++)
    {
        qDebug() << iter.value().getName() << "blah";
    }
    */

    //qDebug() << "ahh";

    inFile.close();
}

// the stuff immediately below is for saving functionality, need to move the
// vector names elsewhere

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
    QFile outFile(filename);

    if(!outFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Unable to write to file " << filename << ".";
        return;
    }

    QDataStream outStream(&outFile);

    outStream << currentPose;

    SkeletonVector sv = R_SHOULDER_ELBOW;
    qDebug() << sv;
   // qDebug() << R_SHOULDER_ELBOW;

    /*
    QMap<QString, QVector3D> qm;

    qm.insert(QString("hello"), QVector3D(1, 11, 111));
    qm.insert(QString("hey there"), QVector3D(2, 22, 222));

    qDebug() << qm.count();

    outStream << qm;
    */

    /*
    PoseSample samp1;
    samp1.setName("Sample 1");

    //jPositions
    QMap<XnSkeletonJoint, XnSkeletonJointPosition> qm;

    XnSkeletonJointPosition pos1, pos2;

    pos1.fConfidence = 0.1;
    pos1.position.X = 1.0;
    pos1.position.Y = 11.0;
    pos1.position.Z = 111.0;

    pos2.fConfidence = 0.2;
    pos2.position.X = 2.0;
    pos2.position.Y = 22.0;
    pos2.position.Z = 222.0;

    qm.insert(XN_SKEL_LEFT_COLLAR, pos1);
    qm.insert(XN_SKEL_LEFT_HAND, pos2);

    samp1.setJPositions(qm);

    //jVectors
    QMap<SkeletonVector, QVector3D> jqm;
    jqm.insert(L_ELBOW_HAND, QVector3D(1.0, 11.0, 111.0));
    jqm.insert(R_ELBOW_HAND, QVector3D(2.0, 22.0, 222.0));

    samp1.setJVectors(jqm);

    outStream << samp1;

    //outStream << jqm;

*/
    /*
    QMap<SkeletonVector, QVector3D> qm;

    SkeletonVector sv1 = SHOULDER_SHOULDER;
    QVector3D vec1(1.0, 11.0, 111.0);
    qm.insert(sv1, vec1);

    SkeletonVector sv2 = R_ELBOW_HAND;
    QVector3D vec2(2.0, 22.0, 222.0);
    qm.insert(sv2, vec2);
    */

    /*
    QMap<XnSkeletonJoint, XnSkeletonJointPosition> qm;

    XnSkeletonJointPosition pos1, pos2;

    pos1.fConfidence = 0.1;
    pos1.position.X = 1.0;
    pos1.position.Y = 11.0;
    pos1.position.Z = 111.0;

    pos2.fConfidence = 0.2;
    pos2.position.X = 2.0;
    pos2.position.Y = 22.0;
    pos2.position.Z = 222.0;

    qm.insert(XN_SKEL_LEFT_COLLAR, pos1);
    qm.insert(XN_SKEL_LEFT_HAND, pos2);

    outStream << qm;
    */

    /*

    XnSkeletonJoint sj = XN_SKEL_WAIST;

    outStream << sj;

    */
    /*
    XnVector3D v;
    v.X = 33.2;
    v.Y = 244.9;
    v.Z = 34444.4;

    XnSkeletonJointPosition sjp;
    sjp.position = v;
    sjp.fConfidence = 0.5;

    outStream << sjp;
    */

   // Vector3 v(1.2, 2.9, 3.4);

   // outStream << v;

    //outStream << currentPose;

    //QTextStream outStream(&outFile);
    //QDomDocument outXML;

    /*
    QDomNode root = outXML.createElement("pose");
    outXML.appendChild(root);

    for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
    {
        QDomNode vector = outXML.createElement(SkeletonVectorNames[col]);
        root.appendChild(vector);
    }

    outXML.save(outStream, 0);
    */
    outFile.close();

}

// File->Save
void MainWindow::on_actionSave_triggered()
{
    if(!currentFilename.isEmpty())
        saveFile(currentFilename);
    else
        on_actionSaveAs_triggered();
}

// File->Save As...
void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Pose As"), "./", tr("Pose Files (*.pose)"));

    if(filename.isEmpty())
        return;

    saveFile(filename);

    this->setWindowTitle(filename + " - PoseDesigner");
    currentFilename = filename;
}

// File->Exit
void MainWindow::on_actionExit_triggered()
{

}

// Edit->Undo
void MainWindow::on_actionUndo_triggered(){}

// Edit->Redo
void MainWindow::on_actionRedo_triggered(){}

// Edit->Cut
void MainWindow::on_actionCut_triggered(){}

// Edit->Copy
void MainWindow::on_actionCopy_triggered(){}

// Edit->Paste
void MainWindow::on_actionPaste_triggered(){}

// Edit->Delete
void MainWindow::on_actionDelete_triggered(){}

// Edit->Select All
void MainWindow::on_actionSelect_All_triggered(){}

// Help->PoseDesigner Help
void MainWindow::on_actionPoseDesigner_Help_triggered()
{
    HelpDialog *hDialog = new HelpDialog(this);
    hDialog->show();
}

// Help->About PoseDesigner
void MainWindow::on_actionAbout_PoseDesigner_triggered()
{
    AboutDialog *aDialog = new AboutDialog(this);
    aDialog->show();
}

/*
===============================================================================
"Big Buttons in the Middle" Actions
===============================================================================
*/

// Helper for adding and removing samples
void MainWindow::calculateStats()
{
    currentPose.calculateStatistics();

    // populate the columns
    QTableWidgetItem *newItem;

    for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
    {
        QVector3D vec = currentPose.getMean().getJVector(col);
        newItem = new QTableWidgetItem(QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z()));
        ui->statsWidget->setItem(0, col, newItem);
    }

    for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
    {
        QVector3D vec = currentPose.getStdDev().getJVector(col);
        newItem = new QTableWidgetItem(QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z()));
        ui->statsWidget->setItem(1, col, newItem);
    }
}

// Take Sample
void MainWindow::on_buttonTakeSample_clicked()
{   
    if(!kinectInfo.bConnected)
        return;

    xn::SkeletonCapability sc = kinectInfo.userGenerator.GetSkeletonCap();

    // make sure we actually have data to sample, otherwise inform user
    if(!sc.IsTracking(1))
    {
        QMessageBox msgBox;
        msgBox.setText("First user not found (blue user).");
        msgBox.setInformativeText("Please register skeleton first.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    PoseSample newSample;
    QImage image;

    // shorten expressions
    GLWidget* kw = ui->kinectWidget;

    // first, grab relevant portion of GLWidget framebuffer. this is important
    // to do first because a person who is timing themselves won't be able to
    // hit confirm and have his image saved correctly
    int side = qMin(kw->width(), kw->height());
    image = kw->grabFrameBuffer().copy((kw->width()  - side) / 2,
                                       (kw->height() - side) / 2,
                                       side, side);


    // currently we only track the first user
    if(sc.IsTracking(1))
    {
        // defined the ++ operator for XnSkeletonJoint in PoseSample.h...
        // OpenNI was not kind enough to do this, or to at least add a
        // SKEL_MAX ending marker to their enum :|
        for(XnSkeletonJoint sj = XN_SKEL_HEAD; sj <= XN_SKEL_RIGHT_FOOT; sj++)
        {
            qDebug("%d", sj);
            sc.GetSkeletonJointPosition(1, sj, newSample.getJPositions_nc()[sj]); // gross hack to get around const
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
        newSample.setName(text);
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
            QVector3D vec = newSample.getJVector(col);
            newItem = new QTableWidgetItem(QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z()));
            ui->tableWidget->setItem(row, col, newItem);
        }

        // add it to the sample collection for this pose
        currentPose.addSample(text, newSample);

        // recalculate statistics
        calculateStats();
    }
}

// Take Sample Timer
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

// Remove Sample
void MainWindow::on_buttonRemoveSample_clicked()
{
    if(!kinectInfo.bConnected)
        return;

    // TODO: need check here to make sure something is actually selected
    // in the list widget
    QListWidget *lw = ui->listWidget;
    QTableWidget *tw = ui->tableWidget;

    // empty list
    if(!lw->count())
    {
        QMessageBox msgBox;
        msgBox.setText("No samples to remove.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    // populated list but nothing selected
    if(lw->selectedItems().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Please select a sample from the list and try again.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    // get the name of the item
    QString name = lw->currentItem()->text();

    // remove it from the current pose's samples
    currentPose.removeSample(name);

    // remove it from the data table
    tw->removeRow(lw->row(lw->currentItem()));

    // remove it from the list
    lw->takeItem(lw->row(lw->currentItem()));

    // if samples remain, recalculate stats
    if(lw->count())
        calculateStats();
    // otherwise clear them
    else
    {
        tw->clearContents();
        ui->statsWidget->clearContents();
    }
}

// Kinect Settings
void MainWindow::on_buttonKinectSettings_clicked()
{
    if(!kinectInfo.bConnected)
        return;

    KinectOptionsDialog *koDialog = new KinectOptionsDialog(this);
    koDialog->show();
}

/*
===============================================================================
Various Widget Actions
===============================================================================
*/

// User highlighted a different sample
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

    QString name = ui->listWidget->currentItem()->text();
    QImage image = currentPose.getSample(name).getImage();
    QSize size = QSize(ui->samplePreview->width()-2,
                       ui->samplePreview->height()-2);

    scene->addPixmap(QPixmap::fromImage(image.scaled(size)));

    ui->samplePreview->setScene(scene);
    ui->samplePreview->update();
}
