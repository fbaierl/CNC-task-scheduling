#ifndef SOLVERWORKER_H
#define SOLVERWORKER_H

#include <QThread>

#include "tree.h"
#include "schedule.h"

class SolverWorker : public QThread
{
    Q_OBJECT

public:
    explicit SolverWorker(int index, QObject *parent = 0);
    ~SolverWorker();

    void setup(Schedule schedule);

    void run();

    bool hasFinished() const;

signals:
    void signal_solverResultReady(QVector<SolverResult>, int);


private:
    Schedule m_schedule;
    int m_index;
    bool m_finished;

};

#endif // SOLVERWORKER_H
