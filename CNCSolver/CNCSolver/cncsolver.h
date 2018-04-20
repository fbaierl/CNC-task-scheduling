#ifndef CNCSOLVER_H
#define CNCSOLVER_H

#include <QVector>
#include <iomanip>

#include "solver.h"

class CNCSolver : public Solver
{
public:
    enum CNCVariant
    {
        CONTIGUOUS,
        NONCONTIGUOUS
    };

    CNCSolver();

    int solve(TaskSequence sequence);

    void setVariant(CNCVariant var);
    void setSchedule(Schedule schedule);
    bool isContiguous() const;

    QString getPlanString() const;

private:
    bool m_isContiguous;
    Schedule m_schedule;
    QVector<QVector<int> > m_plan;

    void appendColumn();

    void printPlan();
};

#endif // CNCSOLVER_H
