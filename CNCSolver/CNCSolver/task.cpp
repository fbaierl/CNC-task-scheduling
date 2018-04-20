#include "task.h"

Task::Task()
{
}

Task::Task(int taskNumber, int processingTime, int size) :
    m_taskNumber(taskNumber),
    m_processingTime(processingTime),
    m_size(size)
{
}

int Task::getTaskNumber() const
{
    return m_taskNumber;
}

int Task::getProcessingTime() const
{
    return m_processingTime;
}

int Task::getSize() const
{
    return m_size;
}
