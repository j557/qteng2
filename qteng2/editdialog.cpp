#include "editdialog.h"
#include "ui_editdialog.h"
#include "databasemanager.h"
#include "item.h"

EditDialog::EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);
    m_currentItemIdx = 0;

    updateFieldsToCurrentElement();
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::updateCounter()
{
    ui->horizontalSliderCounter->setMinimum( 0 );
    ui->horizontalSliderCounter->setMaximum( itemCount() - 1 );
    ui->horizontalSliderCounter->setValue( m_currentItemIdx );
    ui->labelCounter->setText( QString("%1 / %2").arg(m_currentItemIdx+1).arg(itemCount()) );
}

void EditDialog::updateFieldsToCurrentElement()
{
    DatabaseManager::getInstance().itemsAtIndex(m_currentItemIdx, &m_currentItem, &m_currentItemRev);

    const QString& q = m_currentItem ? m_currentItem->GetQuestion() : QString();
    const QString& a = m_currentItem ? m_currentItem->GetAnswer() : QString();
    const QString& e = m_currentItem ? m_currentItem->GetExample() : QString();
    bool  r = (m_currentItemRev != NULL);

    ui->textEditQuestion->setText( q );
    ui->textEditAnswer->setText( a );
    ui->textEditExample->setText( e );
    ui->checkBoxReversable->setChecked(r);

    ui->pushButtonPrev->setEnabled( m_currentItemIdx > 0 );
    ui->pushButtonNext->setEnabled( m_currentItemIdx < itemCount() - 1 );

    updateCounter();
}

void EditDialog::on_pushButtonNext_clicked()
{
    showNextItem();
}

void EditDialog::showNextItem()
{
    if(m_currentItemIdx >= itemCount() - 1 )
    {
        //TODO: assert?
        qDebug("Next button should be disabled. Ignore");
        return;
    }

    m_currentItemIdx++;
    updateFieldsToCurrentElement();
}

void EditDialog::on_pushButtonPrev_clicked()
{
    showPrevItem();
}

void EditDialog::showPrevItem()
{
    if(m_currentItemIdx <= 0 )
    {
        //TODO: assert?
        qDebug("Prev button should be disabled. Ignore");
        return;
    }

    m_currentItemIdx--;
    updateFieldsToCurrentElement();
}

int EditDialog::itemCount()
{
    return  DatabaseManager::getInstance().reversableItemCount() +
            DatabaseManager::getInstance().irreversableItemCount();
}

void EditDialog::on_horizontalSliderCounter_valueChanged(int val)
{
    m_currentItemIdx = val;
    updateFieldsToCurrentElement();
}
