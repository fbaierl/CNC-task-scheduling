#ifndef SOLVER_H
#define SOLVER_H

#include "schedule.h"

class Solver
{
public:
    Solver();
    virtual ~Solver(){}

    virtual int solve(TaskSequence sequence) = 0;

};

#endif // SOLVER_H
