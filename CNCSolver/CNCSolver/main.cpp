#include "mainwindow.h"
#include <QApplication>

#include "solverresult.h"

Q_DECLARE_METATYPE(SolverResult)

int main(int argc, char *argv[])
{
    qRegisterMetaType<QVector<SolverResult> >("QVector<SolverResult>");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
