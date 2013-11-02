#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->buttonCorrect->setVisible(false);
    ui->buttonWrong->setVisible(false);
}

MainWindow::~MainWindow()
{
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Caption"), QString(), "*.txt");
    if (!fileName.isEmpty())
    {
        //loadFile(fileName);
    }

}
