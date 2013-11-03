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
    void on_buttonCheck_clicked();
    void menuLoad();

private:
    ItemContainer* m_itemContainer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
