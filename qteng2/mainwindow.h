#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ItemContainer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void menuNew();
    void menuLoad();
    void menuEdit();
    void on_pushButtonCheck_clicked();
    void on_pushButtonWrong_clicked();
    void on_pushButtonCorrect_clicked();

private:
    void displayEditDialog();
    void startNewGame( bool reloadItemContainer );
    void setInformationLabels();
    void displayNextQuestion();

private:
    ItemContainer* m_itemContainer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
