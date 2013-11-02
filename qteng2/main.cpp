#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "mainwindow.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    MainWindow w;
    w.show();

    return app->exec();
}
