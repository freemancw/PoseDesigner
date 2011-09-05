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

    if(!ui->sampleList->count())
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
        ui->sampleList->clear();
        ui->sampleTable->clearContents();
        ui->sampleTable->setRowCount(0);
        ui->statsTable->clearContents();
        currentFilename = QString();
        this->setWindowTitle(tr("untitled.pose - PoseDesigner"));
    }
}

// File->Open
void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Pose"), "./", tr("Pose Files (*.pose)"));

    if(filename.isEmpty())
        return;

    QFile inFile(filename);

    if(!inFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to read file " << filename << ".";
        return;
    }


    // should probably see if the current file has been modified before
    // continuing

    currentFilename = filename;
    this->setWindowTitle(filename + tr(" - PoseDesigner"));

    QDataStream inStream(&inFile);
    inStream >> currentPose;
    inFile.close();

    // clear sample list
    ui->sampleList->clear();

    // clear the sample table
    ui->sampleTable->clearContents();
    ui->sampleTable->setRowCount(0);

    // clear the stats table
    ui->statsTable->clearContents();

    // add samples to list and table
    QMap<QString, PoseSample> samples = currentPose.getSamples();
    QMap<QString, PoseSample>::iterator sIter;

    for(sIter = samples.begin(); sIter != samples.end(); ++sIter)
    {
        QListWidgetItem *wi = new QListWidgetItem(sIter.key(), ui->sampleList);
        ui->sampleList->addItem(wi);

        // add a row in the table
        int row = ui->sampleTable->rowCount();
        ui->sampleTable->insertRow(row);
        ui->sampleTable->setVerticalHeaderItem(row, new QTableWidgetItem(sIter.key()));

        // populate the columns
        QTableWidgetItem *newItem;
        for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
        {
            QVector3D vec = sIter.value().getJVector(col);
            newItem = new QTableWidgetItem(QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z()));
            ui->sampleTable->setItem(row, col, newItem);
        }
    }

    // populate the columns
    QTableWidgetItem *newItem;

    for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
    {
        QVector3D vec = currentPose.getMean().getJVector(col);
        newItem = new QTableWidgetItem(QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z()));
        ui->statsTable->setItem(0, col, newItem);
    }

    for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
    {
        QVector3D vec = currentPose.getStdDev().getJVector(col);
        newItem = new QTableWidgetItem(QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z()));
        ui->statsTable->setItem(1, col, newItem);
    }
}

static void saveFile(QString filename)
{
    QFile outFile(filename);

    if(!outFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Unable to write to file " << filename << ".";
        return;
    }

    QDataStream outStream(&outFile);
    outStream << currentPose;
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

// File->Export...
void MainWindow::on_actionExport_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Export Pose"), "./", tr("Pose Files (*.pd)"));

    if(filename.isEmpty())
        return;

    QFile exptFile(filename);
    if(!exptFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Unable to export to file " << filename << ".";
        return;
    }

    QDomDocument expt("Pose");
    QDomElement root = expt.createElement("pose");
    expt.appendChild(root);

    QDomElement mean = expt.createElement("mean");

    for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
    {
        QString s;
        s << col;
        QDomElement skelVec = expt.createElement(s);
        QDomText skelVecText = expt.createTextNode(s);
        QVector3D vec = currentPose.getMean().getJVector(col);
        skelVecText.setNodeValue(QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z()));
        skelVec.appendChild(skelVecText);
        mean.appendChild(skelVec);
    }
    root.appendChild(mean);

    QDomElement stddev = expt.createElement("stddev");

    for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
    {
        QString s;
        s << col;
        QDomElement skelVec = expt.createElement(s);
        QDomText skelVecText = expt.createTextNode(s);
        QVector3D vec = currentPose.getStdDev().getJVector(col);
        skelVecText.setNodeValue(QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z()));
        skelVec.appendChild(skelVecText);
        stddev.appendChild(skelVec);
    }
    root.appendChild(stddev);

    QTextStream ts(&exptFile);
    expt.save(ts, 2);

    exptFile.close();
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
        ui->statsTable->setItem(0, col, newItem);
    }

    for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
    {
        QVector3D vec = currentPose.getStdDev().getJVector(col);
        newItem = new QTableWidgetItem(QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z()));
        ui->statsTable->setItem(1, col, newItem);
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
        QListWidgetItem *wi = new QListWidgetItem(text, ui->sampleList);
        ui->sampleList->addItem(wi);

        // add a row in the table
        int row = ui->sampleTable->rowCount();
        ui->sampleTable->insertRow(row);
        ui->sampleTable->setVerticalHeaderItem(row, new QTableWidgetItem(text));

        // populate the columns
        QTableWidgetItem *newItem;
        for(SkeletonVector col = NECK_HEAD; col < SKEL_VEC_MAX; col++)
        {
            QVector3D vec = newSample.getJVector(col);
            newItem = new QTableWidgetItem(QString("%1, %2, %3").arg(vec.x()).arg(vec.y()).arg(vec.z()));
            ui->sampleTable->setItem(row, col, newItem);
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
    QListWidget *lw = ui->sampleList;
    QTableWidget *tw = ui->sampleTable;

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
        ui->statsTable->clearContents();
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
void MainWindow::on_sampleList_currentItemChanged
(QListWidgetItem *current, QListWidgetItem *previous)
{
    QGraphicsScene *scene = new QGraphicsScene();

    if(current == NULL)
    {
        scene->setBackgroundBrush(Qt::NoBrush);
        ui->samplePreview->setScene(scene);
        ui->samplePreview->update();
        return;
    }

    QString name = ui->sampleList->currentItem()->text();
    QImage image = currentPose.getSample(name).getImage();
    QSize size = QSize(ui->samplePreview->width()-2,
                       ui->samplePreview->height()-2);

    scene->addPixmap(QPixmap::fromImage(image.scaled(size)));

    ui->samplePreview->setScene(scene);
    ui->samplePreview->update();
}
