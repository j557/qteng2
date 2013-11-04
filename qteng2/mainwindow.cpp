#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "itemcontainer.h"
#include "item.h"
#include "editdialog.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->pushButtonCorrect->setVisible(false);
    ui->pushButtonWrong->setVisible(false);
    ui->actionEdit->setEnabled(false);

    m_itemContainer = new ItemContainer();
}

MainWindow::~MainWindow()
{
    delete m_itemContainer;
    delete ui;
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
    startNewGame( false );
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

void MainWindow::startNewGame( bool reloadItemContainer )
{
    if( reloadItemContainer )
    {
        m_itemContainer->reload();
    }

    setInformationLabels();
    displayNextQuestion();
}

void MainWindow::setInformationLabels()
{
    ui->labelErrors->setText( QString("Errors: %1").arg(m_itemContainer->getNumberOfIncorrectAnswers()) );
    ui->labelQuestionsLeft->setText( QString("Left: %1").arg(m_itemContainer->getNumberOfQuestionsLeft()) );
}

void MainWindow::displayNextQuestion()
{
    m_itemContainer->prepareNextQuestion();
    const Item* curr = m_itemContainer->getCurrentQuestion();

    if( !curr )
    {
        int e = m_itemContainer->getNumberOfIncorrectAnswers();
        QMessageBox msgBox;
        msgBox.setText("Test over");
        msgBox.setInformativeText( QString("You finished test with %1 error%2.").arg(e).arg( (e==1) ? "" : "s" ) );
        msgBox.setStandardButtons(QMessageBox::Close);
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.exec();
        close();
    }
    else
    {
        setInformationLabels();
        ui->textEditQuestion->setText(curr->GetQuestion());
        ui->textEditAnswer->setText("");
        ui->pushButtonCheck->setVisible(true);
        ui->pushButtonWrong->setVisible(false);
        ui->pushButtonCorrect->setVisible(false);
    }
}

void MainWindow::on_pushButtonCheck_clicked()
{
    const Item* curr = m_itemContainer->getCurrentQuestion();
    QString ans = curr->GetAnswer() + "\n---\n" + curr->GetExample();
    ui->textEditAnswer->setText(ans);
    ui->pushButtonCheck->setVisible(false);
    ui->pushButtonWrong->setVisible(true);
    ui->pushButtonCorrect->setVisible(true);
}

void MainWindow::on_pushButtonWrong_clicked()
{
    m_itemContainer->currentQuestionAnsweredNotProperly();
    displayNextQuestion();
}

void MainWindow::on_pushButtonCorrect_clicked()
{
    m_itemContainer->currentQuestionAnsweredProperly();
    displayNextQuestion();
}

