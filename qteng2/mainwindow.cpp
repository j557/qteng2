#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "itemcontainer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->buttonCorrect->setVisible(false);
    ui->buttonWrong->setVisible(false);

    m_itemContainer = new ItemContainer();
    m_itemContainer->load("data_1383436307662.sqlite");
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Caption"), QString(), "*.sqlite");
    if (!fileName.isEmpty())
    {
        //loadFile(fileName);
    }

}
