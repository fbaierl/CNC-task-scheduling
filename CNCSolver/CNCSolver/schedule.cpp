#include "schedule.h"

Schedule::Schedule() :
    m_tasks(QVector<Task>()),
    m_processorCount(0)
{
}

Schedule::Schedule(const Schedule &schedule) :
    m_tasks(QVector<Task>()),
    m_processorCount(0)
{
    for(int i = 0; i < schedule.size(); ++i)
        m_tasks.append(schedule.getTaskAt(i));

    m_processorCount = schedule.getProcessorCount();
}

Schedule::~Schedule()
{
}

void Schedule::setSchedule(TaskSequence taskSequence, QVector<int> processingTimes, QVector<int> sizes)
{
    QVector<Task> tasks;
    for(int i = 0; i < taskSequence.size(); ++i)
    {
        Task t = Task(taskSequence.at(i), processingTimes.at(i), sizes.at(i));
        tasks.append(t);
    }

    setSchedule(tasks);
}

void Schedule::setSchedule(QVector<Task> tasks)
{
    m_tasks.clear();

    for(int i = 0; i < tasks.size(); ++i)
        m_tasks.append(tasks.value(i));
}

void Schedule::setProcessorCount(int count)
{
    m_processorCount = count;
}

int Schedule::getProcessorCount() const
{
    return m_processorCount;
}

void Schedule::append(Task task)
{
    m_tasks.append(task);
}

void Schedule::remove(Task task)
{
    for(int i = 0; i < m_tasks.size(); ++i)
    {
        if(m_tasks.at(i).getTaskNumber() == task.getTaskNumber())
        {
            m_tasks.remove(i);
            return;
        }
    }
}

int Schedule::size() const
{
    return m_tasks.size();
}

Task Schedule::getTaskAt(int index) const
{
    return m_tasks.at(index);
}

TaskSequence Schedule::getTaskSequence() const
{
    TaskSequence seq;

    for(int i = 0; i < m_tasks.size(); ++i)
    {
        seq.append(m_tasks.at(i).getTaskNumber());
    }

    return seq;
}

void Schedule::printSchedule() const
{
    if(m_tasks.empty())
        return;

    std::cout << "Schedule: ";
    for(int i = 0; i < m_tasks.size(); ++i)
    {
        std::cout << m_tasks.at(i).getTaskNumber() << " ";
    }
    std::cout << std::endl << "          ";

    for(int i = 0; i < m_tasks.size(); ++i)
    {
        std::cout << m_tasks.at(i).getProcessingTime() << " ";
    }
    std::cout << std::endl;
}

QString Schedule::toCSVString()
{
    QString s;
    for(int i = 0; i < m_tasks.size(); ++i)
    {
        Task t = m_tasks.at(i);
        s.append("(" + QString::number(t.getTaskNumber()) + "," +
                 QString::number(t.getProcessingTime()) + "," +
                 QString::number(t.getSize()) + ")");
    }

    return s;
}

