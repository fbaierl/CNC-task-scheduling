#include "solverworker.h"

SolverWorker::SolverWorker(int index, QObject *parent) :
    QThread(parent),
    m_index(index),
    m_finished(true)
{
}

SolverWorker::~SolverWorker()
{
}

void SolverWorker::setup(Schedule schedule)
{
    m_schedule = schedule;
}

void SolverWorker::run()
{
    m_finished = false;

    //--- CONTIGUOUS ---
    CNCSolver *solverC = new CNCSolver();
    solverC->setSchedule(m_schedule);
    solverC->setVariant(CNCSolver::CONTIGUOUS);

    Tree *treeC = new Tree();
    SolverResult resC = treeC->solve(m_schedule, solverC);

    delete solverC;
    delete treeC;


    //--- NONCONTIGUOUS ---
    CNCSolver *solverNC = new CNCSolver();
    solverNC->setSchedule(m_schedule);
    solverNC->setVariant(CNCSolver::NONCONTIGUOUS);

    Tree *treeNC = new Tree();
    SolverResult resNC = treeNC->solve(m_schedule, solverNC);

    delete solverNC;
    delete treeNC;


    m_finished = true;

    QVector<SolverResult> results;
    results.append(resC);
    results.append(resNC);

    emit signal_solverResultReady(results, m_index);
}

bool SolverWorker::hasFinished() const
{
    return m_finished;
}

