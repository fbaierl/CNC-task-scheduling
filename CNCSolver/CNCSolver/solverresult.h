#ifndef SOLVERRESULT_H
#define SOLVERRESULT_H

#include "schedule.h"

class SolverResult
{
public:
    SolverResult(){}
    SolverResult(int solverResult, Schedule schedule, TaskSequence optimalSequence, bool isContiguous);
    ~SolverResult();

    int getSolverResult() const;
    Schedule getSchedule() const;
    TaskSequence getOptimalSequence() const;
    bool isContiguous() const;

private:
    int m_solverResult;    //solver result
    Schedule m_schedule;
    TaskSequence m_optimalSequence;
    bool m_isContiguous;

};

#endif // SOLVERRESULT_H
