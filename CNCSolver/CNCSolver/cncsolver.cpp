#include "cncsolver.h"

CNCSolver::CNCSolver() :
    m_isContiguous(true)
{

}

int CNCSolver::solve(TaskSequence sequence)
{
    if(sequence.empty())
        return 0;

    int processorCount = m_schedule.getProcessorCount();
    m_plan = QVector<QVector<int> >(processorCount);

    appendColumn();

    for(int s = 0; s < sequence.size(); ++s)
    {
        bool planned = false;
        int taskNumber = sequence.at(s);
        Task t = m_schedule.getTaskAt(taskNumber);

        for(int i = 0; i < m_plan.at(0).size(); ++i)
        {
            QVector<int> candidates;

            for(int j = 0; j < processorCount; ++j)
            {
                if(m_plan.at(j).at(i) == -1)
                {
                    candidates.append(j);
                }
            }

            int j = 0;
            while(j < candidates.size())
            {
                int candidate = candidates.at(j);
                for(int q = i; q < i + t.getProcessingTime(); ++q)
                {
                    if(m_plan.at(candidate).size() <= q)  //New: at end of list -> enough space
                        break;

                    if(m_plan.at(candidate).at(q) != -1)
                    {
                        candidates.remove(j);
                        --j;    //indices changed because the remove
                        break;
                    }
                }

                ++j;
            }

            if(candidates.size() >= t.getSize())
            {
                if(m_isContiguous)
                {
                    QVector<int> newCandidates;

                    int lastCandidate = candidates.at(0);
                    newCandidates.append(candidates.at(0));

                    for(int c = 1; c < candidates.size(); ++c)
                    {
                        int candidate = candidates.at(c);
                        if((candidate - lastCandidate) == 1)
                        {
                            newCandidates.append(candidate);
                            lastCandidate = candidate;
                        }
                        else
                        {
                            if(newCandidates.size() >= t.getSize())    //found enough contiguous processors
                            {
                                break;
                            }
                            else    //not enough -> continue search
                            {
                                newCandidates.clear();
                                newCandidates.append(candidate);
                            }

                        }
                    }

                    if(newCandidates.size() >= t.getSize())
                        candidates = newCandidates;
                    else
                    {
                        break;  //go to next column
                    }
                }

                int candidatesSize = candidates.size();
                int diff = candidatesSize - t.getSize();
                if(diff > 0)
                    candidates.remove(candidatesSize - diff, diff);


                for(int c = 0; c < candidates.size(); ++c)
                {
                    int candidate = candidates.at(c);
                    for(int q = i; q < i + t.getProcessingTime(); ++q)
                    {
                        if(m_plan.at(candidate).size() <= q)
                            appendColumn();

                        m_plan[candidate][q] = taskNumber;
                    }
                }

                planned = true;

                break;  //do next task
            }
            else
            {
                continue;
            }

            break;
        }

        if(!planned)
        {
            //Doesn't fit anywhere -> append

            int planSize = m_plan.at(0).size();
            for(int c = 0; c < t.getSize(); ++c)
            {
                for(int q = planSize; q < planSize + t.getProcessingTime(); ++q)
                {
                    if(m_plan.at(c).size() <= q)
                        appendColumn();

                    m_plan[c][q] = taskNumber;
                }
            }
        }

        //printPlan(&plan);
    }

    return m_plan.at(0).size();
}

void CNCSolver::setVariant(CNCVariant var)
{
    switch (var) {
    case CONTIGUOUS:
        m_isContiguous = true;
        break;
    case NONCONTIGUOUS:
        m_isContiguous = false;
        break;
    default:
        m_isContiguous = true;
        break;
    }
}

void CNCSolver::setSchedule(Schedule schedule)
{
    m_schedule = schedule;
}

bool CNCSolver::isContiguous() const
{
    return m_isContiguous;
}

QString CNCSolver::getPlanString() const
{
    QString s;
    for(int i = m_plan.size() - 1; i >= 0; --i)
    {
        s.append(QString::number(i).rightJustified(2, ' ') + " |");

        for(int j = 0; j < m_plan.at(i).size(); ++j)
        {
            int t = m_plan.at(i).at(j);

            if(t == -1)
                s.append("  -");
            else
                s.append(QString::number(m_plan.at(i).at(j)).rightJustified(3, ' '));
        }
        s.append("\n");
    }

    return s;
}

void CNCSolver::appendColumn()
{
    for(int i = 0; i < m_plan.size(); ++i)
    {
        (m_plan)[i].append(-1);
    }
}

void CNCSolver::printPlan()
{
    for(int i = m_plan.size() - 1; i >= 0; --i)
    {
        std::cout << i << " | ";

        for(int j = 0; j < m_plan.at(i).size(); ++j)
        {
            int t = m_plan.at(i).at(j);

            if(t == -1)
                std::cout << "  -  ";
            else
                std::cout << std::setw(3) << m_plan.at(i).at(j) << "  ";
        }

        std::cout << std::endl;
    }
}

