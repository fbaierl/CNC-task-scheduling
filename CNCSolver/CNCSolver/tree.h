#ifndef TREE_H
#define TREE_H

#include "cncsolver.h"
#include "schedule.h"

#include "solverresult.h"

#include <QVector>
#include <iostream>

class Tree
{
public:
    Tree(Tree *parent = NULL, TaskSequence currentTaskSequence = TaskSequence());
    ~Tree();

    Tree *getParent() const;

    void setUpperBound(int upper);
    void setLowerBound(int lowerBound);
    int getLowerBound() const;

    void printTree();

    SolverResult solve(Schedule schedule, CNCSolver *solver);

    static int getMaxCount();

    void setTaskSequences(TaskSequence newTaskSequences);

    bool hasRemainingSubtrees() const;

    bool isLeaf() const;

    TaskSequence getCurrentTaskSequence() const;
    TaskSequence getFreeTasks() const;
    TaskSequence getRemainingSubtrees() const;

    void append(Tree *subtree);

    bool hasComputedLowerBound() const;

    int getCurrentTaskNumber() const;
    void removeFromRemainingSubtrees(int taskNumber);

    void deleteSubtree(Tree *tree);
    void deleteAllSubtrees();

private:
    static int m_counter;
    static int m_maxCount;

    int m_upperBound;
    int m_lowerBound;

    bool m_lowerBoundIsComputed;

    Tree *m_parent;

    Schedule m_schedule;

    TaskSequence m_currentTaskSequence; //Current already planned tasks
    TaskSequence m_freeTasks;           //Tasks that are free to be planned in the next selection of a task
    TaskSequence m_remainingSubtrees;   //tasknumbers of remaining subtrees to visit

    TaskSequence m_currentOptimalTaskSequence;

    QVector<Tree*> *m_trees;


};

#endif // TREE_H
