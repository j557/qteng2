#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

class Item;

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditDialog(QWidget *parent = 0);
    ~EditDialog();
    
private slots:
    void on_pushButtonNext_clicked();
    void on_pushButtonPrev_clicked();
    void on_horizontalSliderCounter_valueChanged(int val);
    void on_pushButtonNew_clicked();
    void on_pushButtonDone_clicked();

private:
    void updateFieldsToCurrentElement();
    void updateCounter();
    void showNextItem();
    void showPrevItem();
    void showItemAtIndex( int idx );
    int  itemCount();
    void updateElementInDatabase();
    void createNewItem( const QString& question = QString(), const QString& answer = QString(), const QString& example = QString() );
    bool isNewElementEdited() const;

private:
    Ui::EditDialog *ui;
    int m_currentItemIdx;
    Item* m_currentItem;
    Item* m_currentItemRev;

};

#endif // EDITDIALOG_H
