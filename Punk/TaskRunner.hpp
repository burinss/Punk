#pragma once

#include "PunkDefines.hpp"

class Task;

class PTaskRunner
{

private:

    void ClearProcesses();

public:

    ~PTaskRunner();

    void UpdateTick(double deltaTime);
    void UpdateProcesses(double deltaTime, PTaskPriority priority);
    void AttachProcess(std::shared_ptr<Task> process, PTaskPriority priority = PTaskPriority::FirstPriority);
    void AbortAllProcesses(bool immediate);

    unsigned int ProcessCount(PTaskPriority priority) const {
        assert(priority >= PTaskPriority::FirstPriority && priority < PTaskPriority::LastPriority);
        return processList_[priority].size();
    }

protected:

    PTaskList processList_[PTaskPriority::LastPriority];

};