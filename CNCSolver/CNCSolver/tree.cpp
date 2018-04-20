#include "tree.h"

int Tree::m_counter = 0;
int Tree::m_maxCount = 0;

Tree::Tree(Tree *parent, TaskSequence currentTaskSequence) :
    m_upperBound(0),
    m_lowerBoundIsComputed(false),
    m_parent(parent),
    m_currentTaskSequence(currentTaskSequence),
    m_trees(new QVector<Tree*>())
{
    m_counter++;
    m_maxCount = qMax(m_maxCount, m_counter);
}

Tree::~Tree()
{
    for(int i = 0; i < m_trees->size(); ++i) {
        delete m_trees->value(i);
    }
    delete m_trees;

    m_counter--;
}

Tree *Tree::getParent() const
{
    return m_parent;
}

void Tree::setUpperBound(int upper)
{
    m_upperBound = upper;
}

void Tree::setLowerBound(int lowerBound)
{
    m_lowerBound = lowerBound;
    m_lowerBoundIsComputed = true;
}

int Tree::getLowerBound() const
{
    return m_lowerBound;
}

void Tree::printTree()
{
    for(int i = 0; i < m_schedule.size(); ++i)
        std::cout << m_schedule.getTaskAt(i).getTaskNumber() << ", ";

    std::cout << std::endl;

    for(int i = 0; i < m_trees->size(); ++i)
    {
        m_trees->value(i)->printTree();
    }
}

SolverResult Tree::solve(Schedule schedule, CNCSolver *solver)
{
    //A branch with a partial schedule not shorter than the best known schedule will be eliminated

    //init
    TaskSequence initSeqence = schedule.getTaskSequence();
    m_freeTasks = initSeqence;
    m_remainingSubtrees = initSeqence;
    //m_currentTaskSequence is empty at the beginning

    m_lowerBound = 0;
    m_upperBound = 2000000000; //Some upper limit that is never exceeded

    int scheduleSize = schedule.size();

    Tree *currentTree = this;

    while(currentTree->hasRemainingSubtrees() || currentTree->getParent() != NULL)
    {
        //solve current TaskSequence
        if(!currentTree->hasComputedLowerBound())
        {
            currentTree->setLowerBound(solver->solve(currentTree->getCurrentTaskSequence()));
        }

        if(currentTree->getLowerBound() >= m_upperBound)
        {
            //drop subtree, go back to parent
            int currentTaskNumber = currentTree->getCurrentTaskNumber();

            Tree *temp = currentTree->getParent();
            temp->deleteSubtree(currentTree);
            currentTree = temp;

            currentTree->removeFromRemainingSubtrees(currentTaskNumber);
            continue;
        }

        //Create new node / handle leaf
        if(currentTree->hasRemainingSubtrees())
        {
            TaskSequence currentFreeTasks = currentTree->getFreeTasks();

            int nextTaskNumber = currentTree->getRemainingSubtrees().at(0);

            TaskSequence newTaskSequence = TaskSequence(currentTree->getCurrentTaskSequence());
            newTaskSequence.append(nextTaskNumber);

            TaskSequence newFreeTasks = TaskSequence(currentFreeTasks); //= current - chosen

            for(int i = 0; i < newFreeTasks.size(); ++i)
            {
                if(newFreeTasks.at(i) == nextTaskNumber)
                {
                    newFreeTasks.remove(i);
                    break;
                }
            }

            Tree *newNode = new Tree(currentTree, newTaskSequence);
            newNode->setTaskSequences(newFreeTasks);

            currentTree->append(newNode);

            currentTree = newNode;
            continue;
        }
        else    //in leaf
        {
            if(currentTree->getCurrentTaskSequence().size() == scheduleSize)
            {
                int lowerBound = currentTree->getLowerBound();
                if(lowerBound < m_upperBound)
                {
                    m_upperBound = lowerBound;  //new upper bound found
                    m_currentOptimalTaskSequence = currentTree->getCurrentTaskSequence();
                }
                else
                {
                    int currentTaskNumber = currentTree->getCurrentTaskNumber();
                    Tree *temp = currentTree->getParent();
                    temp->deleteSubtree(currentTree);
                    currentTree = temp;
                    currentTree->removeFromRemainingSubtrees(currentTaskNumber);

                    continue;
                }
            }

            int currentTaskNumber = currentTree->getCurrentTaskNumber();
            currentTree = currentTree->getParent();

            currentTree->removeFromRemainingSubtrees(currentTaskNumber);

            if(!currentTree->hasRemainingSubtrees())
                currentTree->deleteAllSubtrees();

            continue;
        }

    }

    return SolverResult(m_upperBound, schedule, m_currentOptimalTaskSequence, solver->isContiguous());
}

int Tree::getMaxCount()
{
    return m_maxCount;
}

void Tree::setTaskSequences(TaskSequence newTaskSequences)
{
    m_freeTasks = newTaskSequences;
    m_remainingSubtrees = newTaskSequences;
}

bool Tree::hasRemainingSubtrees() const
{
    return !m_remainingSubtrees.empty();
}

bool Tree::isLeaf() const
{
    return m_trees->empty();
}

TaskSequence Tree::getCurrentTaskSequence() const
{
    return m_currentTaskSequence;
}

TaskSequence Tree::getFreeTasks() const
{
    return m_freeTasks;
}

TaskSequence Tree::getRemainingSubtrees() const
{
    return m_remainingSubtrees;
}

void Tree::append(Tree *subtree)
{
    m_trees->append(subtree);
}

bool Tree::hasComputedLowerBound() const
{
    return m_lowerBoundIsComputed;
}

int Tree::getCurrentTaskNumber() const
{
    if(m_currentTaskSequence.size() == 0)
        return -1;

    return m_currentTaskSequence.at(m_currentTaskSequence.size() - 1);
}

void Tree::removeFromRemainingSubtrees(int taskNumber)
{
    for(int i = 0; i < m_remainingSubtrees.size(); ++i)
    {
        if(m_remainingSubtrees.at(i) == taskNumber)
        {
            m_remainingSubtrees.remove(i);
            break;
        }
    }
}

void Tree::deleteSubtree(Tree *tree)
{
    int currentTaskNumber = tree->getCurrentTaskNumber();
    for(int i = 0; i < m_trees->size(); ++i)
    {
        if(m_trees->at(i)->getCurrentTaskNumber() == currentTaskNumber)
        {
            Tree *temp = m_trees->at(i);
            m_trees->remove(i);
            delete temp;
            return;
        }
    }
}

void Tree::deleteAllSubtrees()
{
    for(int i = 0; i < m_trees->size(); ++i)
    {
            delete m_trees->at(i);
    }

    m_trees->clear();
}
