#include "solverresult.h"

SolverResult::SolverResult(int solverResult, Schedule schedule, TaskSequence optimalSequence, bool isContiguous) :
    m_solverResult(solverResult),
    m_schedule(schedule),
    m_optimalSequence(optimalSequence),
    m_isContiguous(isContiguous)
{
}

SolverResult::~SolverResult()
{
}

int SolverResult::getSolverResult() const
{
    return m_solverResult;
}

Schedule SolverResult::getSchedule() const
{
    return m_schedule;
}

TaskSequence SolverResult::getOptimalSequence() const
{
    return m_optimalSequence;
}

bool SolverResult::isContiguous() const
{
    return m_isContiguous;
}

