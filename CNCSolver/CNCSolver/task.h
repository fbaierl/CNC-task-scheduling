#ifndef TASK_H
#define TASK_H


class Task
{
public:
    Task();
    Task(int taskNumber, int processingTime, int size);

    int getTaskNumber() const;
    int getProcessingTime() const;

    int getSize() const;

private:
    int m_taskNumber;
    int m_processingTime;
    int m_size;
};

#endif // TASK_H
