#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <iostream>
#include "task.h"
#include <QVector>
#include <QString>

typedef QVector<int> TaskSequence;

class Schedule
{
public:
    Schedule();
    Schedule(const Schedule &schedule);

    ~Schedule();

    void setSchedule(TaskSequence taskSequence, QVector<int> processingTimes, QVector<int> sizes);
    void setSchedule(QVector<Task> tasks);

    void setProcessorCount(int count);
    int getProcessorCount() const;

    void append(Task task);
    void remove(Task task);

    int size() const;

    Task getTaskAt(int index) const;
    TaskSequence getTaskSequence() const;

    void printSchedule() const;

    QString toCSVString();

private:
    QVector<Task> m_tasks;

    int m_processorCount;

};

#endif // SCHEDULE_H
