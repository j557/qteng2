#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "itemcontainer.h"
#include "editdialog.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->buttonCorrect->setVisible(false);
    ui->buttonWrong->setVisible(false);
    ui->actionEdit->setEnabled(false);

    m_itemContainer = new ItemContainer();
}

MainWindow::~MainWindow()
{
    delete m_itemContainer;
    delete ui;
}

void MainWindow::on_buttonCheck_clicked()
{
    ui->buttonCorrect->setVisible(true);
    ui->buttonCheck->setVisible(false);
    ui->buttonWrong->setVisible(true);
}

void MainWindow::menuLoad()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Caption"), QString(), "*.sqlite");
    if (!filename.isEmpty())
    {
        bool loaded = m_itemContainer->load( filename );
        ui->actionEdit->setEnabled(loaded);
        if( !loaded )
        {
            QMessageBox msgBox;
            msgBox.setText("Error");
            msgBox.setInformativeText("Cannot load database");
            msgBox.setStandardButtons(QMessageBox::Close);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }
    }

}

void MainWindow::menuEdit()
{
    displayEditDialog();
}

void MainWindow::menuNew()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Caption"), QString(), "*.sqlite", 0, QFileDialog::ShowDirsOnly);
    if( filename.isEmpty() )
    {
        return;
    }

    if( filename.right(7).compare(".sqlite") )
    {
        filename.append(".sqlite");
    }
    else
    {
        //TODO:check if exists, and rename (backup)
    }
    qDebug() << filename;

    bool created = m_itemContainer->create( filename );
    ui->actionEdit->setEnabled(created);
    if( !created )
    {
        QMessageBox msgBox;
        msgBox.setText("Error");
        msgBox.setInformativeText("Cannot create database");
        msgBox.setStandardButtons(QMessageBox::Close);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
    else
    {
        displayEditDialog();
    }
}

void MainWindow::displayEditDialog()
{
    EditDialog* dlg = new EditDialog(this);
    dlg->exec();
}
