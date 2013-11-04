#include "editdialog.h"
#include "ui_editdialog.h"
#include <QDebug>
#include "databasemanager.h"
#include "item.h"

EditDialog::EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);
    m_currentItem = NULL;
    m_currentItemRev = NULL;
    m_currentItemIdx = 0;

    if( itemCount() > 0 )
    {
        updateFieldsToCurrentElement();
    }
    else
    {
        createNewItem();
    }
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::updateCounter()
{
    ui->horizontalSliderCounter->blockSignals(true);
    ui->horizontalSliderCounter->setMinimum( 0 );
    ui->horizontalSliderCounter->setMaximum( itemCount() - 1 );
    ui->horizontalSliderCounter->setValue( m_currentItemIdx );
    ui->labelCounter->setText( QString("%1 / %2").arg(m_currentItemIdx+1).arg(itemCount()) );
    ui->horizontalSliderCounter->blockSignals(false);
}

void EditDialog::updateFieldsToCurrentElement()
{
    if( !isNewElementEdited() )
    {
        delete m_currentItem;
        delete m_currentItemRev;
        DatabaseManager::getInstance().itemsAtIndex(m_currentItemIdx, &m_currentItem, &m_currentItemRev);
    }

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
    showItemAtIndex( m_currentItemIdx + 1 );
}

void EditDialog::on_pushButtonPrev_clicked()
{
    showPrevItem();
}

void EditDialog::showPrevItem()
{
    showItemAtIndex( m_currentItemIdx - 1 );
}

int EditDialog::itemCount()
{
    return  DatabaseManager::getInstance().reversableItemCount() +
            DatabaseManager::getInstance().irreversableItemCount() +
            (isNewElementEdited() ? 1 : 0);
}

void EditDialog::on_horizontalSliderCounter_valueChanged(int val)
{
    showItemAtIndex( val );
}

void EditDialog::showItemAtIndex( int idx )
{
    if(idx < 0 || idx > itemCount() - 1 )
    {
        //TODO: assert?
        qDebug("Item index out of bounds disabled. Ignore");
        return;
    }

    updateElementInDatabase();
    m_currentItemIdx = idx;
    updateFieldsToCurrentElement();
}

void EditDialog::updateElementInDatabase()
{
    //check if any changes
    bool isNew = m_currentItem->GetId() < 0;
    bool isDifferent = isNew;
    bool isRevChecked = (ui->checkBoxReversable->checkState() == Qt::Checked);
    isDifferent = isDifferent || (bool)(ui->textEditQuestion->toPlainText().compare( m_currentItem->GetQuestion() ));
    isDifferent = isDifferent || (bool)(ui->textEditAnswer->toPlainText().compare( m_currentItem->GetAnswer() ));
    isDifferent = isDifferent || (bool)(ui->textEditExample->toPlainText().compare( m_currentItem->GetExample() ));
    isDifferent = isDifferent || (isRevChecked ^ (m_currentItemRev != NULL) );

    if( !isDifferent )
    {
        return;
    }

    //update element
    const QString& q = ui->textEditQuestion->toPlainText();
    const QString& a = ui->textEditAnswer->toPlainText();
    const QString& e = ui->textEditExample->toPlainText();
    bool r = isRevChecked;
    if( isNew )
    {
        if( !DatabaseManager::getInstance().insertItem(q, a, e, r) )
        {
            qDebug() << "Something went wrong with inserting element";
            //TODO: handle error (dialog or something)
        }
        else
        {
            delete m_currentItem;
            m_currentItem = NULL;
        }
    }
    else
    {
        int id = m_currentItem->GetId();
        unsigned int as = m_currentItem->GetAsked();
        unsigned int an = m_currentItem->GetAnswered();
        unsigned int ras = m_currentItemRev ? m_currentItemRev->GetAsked() : 0;
        unsigned int ran = m_currentItemRev ? m_currentItemRev->GetAnswered() : 0;
        if( !DatabaseManager::getInstance().updateItem(id, q, a, e, r, as, an, ras, ran) )
        {
            qDebug() << "Something went wrong with updating element " << id;
            //TODO: handle error (dialog or something)
        }
    }
}

void EditDialog::on_pushButtonNew_clicked()
{
    createNewItem();
}

void EditDialog::createNewItem( const QString& question, const QString& answer, const QString& example )
{
    //save changes in current element
    if( m_currentItem )//may be null if database is empty
    {
        updateElementInDatabase();
    }

    //prepare dialog for new element
    delete m_currentItem;
    delete m_currentItemRev;
    m_currentItemRev = NULL;
    m_currentItem = new Item(-1, question, answer, example, false, 0, 0);
    m_currentItemIdx = itemCount() - 1;
    updateFieldsToCurrentElement();
}

bool EditDialog::isNewElementEdited() const
{
    return (m_currentItem && (m_currentItem->GetId() < 0));
}
