#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include "schedule.h"
#include "tree.h"
#include "cncsolver.h"

#include "solverworker.h"

#include "checkscheduleform.h"

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
    void slot_solvePressed();
    void slot_solved(QVector<SolverResult> result, int index);
    void slot_chooseFilenamePressed();
    void slot_filenameChanged(QString newFilename);
    void slot_checkSchedulePressed();

private:
    Ui::MainWindow *ui;

    CheckScheduleForm *m_checkScheduleForm;

    QVector<SolverWorker*> m_worker;

    QVector<Schedule> m_schedules;

    int m_initialScheduleSize;

    QString m_filename;

    int m_finishedCounter;

    bool generateSchedules();

    int getRandom(int lower, int upper);

    void writeToFile(SolverResult result);

};

#endif // MAINWINDOW_H
