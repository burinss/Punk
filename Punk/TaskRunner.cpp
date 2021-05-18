#include "TaskRunner.hpp"
#include "PTask.hpp"

PTaskRunner::~PTaskRunner()
{
    AbortAllProcesses(true);
    ClearProcesses();
}

void PTaskRunner::UpdateTick(double deltaTime)
{
    for (int priority = PTaskPriority::FirstPriority; priority < PTaskPriority::LastPriority; priority++)
    {
        UpdateProcesses(deltaTime, (PTaskPriority)priority);
    }
}

void PTaskRunner::UpdateProcesses(double deltaTime, PTaskPriority priority)
{
    unsigned short int successCount = 0, failCount = 0;

    PTaskList::iterator it = processList_[priority].begin();
    while (it != processList_[priority].end())
    {
        std::shared_ptr<Task> process = *it;

        PTaskList::iterator thisIt = it;
        ++it;

        if (process->State() == TaskState::Uninitialized)
        {
            process->Initialize();
        }

        if (process->State() == TaskState::Running)
        {
            process->Update(deltaTime);
        }

        if (process->IsDead())
        {
            switch (process->State())
            {
            case TaskState::Finished:
            {
                std::shared_ptr<Task> child = process->RemoveChild();
                if (child)
                {
                    child->SetState(TaskState::Uninitialized);
                    if (child) AttachProcess(child);
                }
                else ++successCount;
                break;
            }
            case TaskState::Failed:
            case TaskState::Aborted:
            {
                ++failCount;
                break;
            }
            default: break;
            }

            processList_[priority].erase(thisIt);
        }
    }
}

void PTaskRunner::ClearProcesses()
{
    for (int priority = PTaskPriority::FirstPriority; priority < PTaskPriority::LastPriority; priority++)
    {
        processList_[priority].clear();
    }
}

void PTaskRunner::AttachProcess(std::shared_ptr<Task> process, PTaskPriority priority)
{
    if (std::find(processList_[priority].begin(), processList_[priority].end(), process) != processList_[priority].end())
        return;
    processList_[priority].push_back(process);
}

void PTaskRunner::AbortAllProcesses(bool immediate)
{
    for (int priority = PTaskPriority::FirstPriority; priority < PTaskPriority::LastPriority; priority++)
    {
        PTaskList::iterator it = processList_[priority].begin();
        while (it != processList_[priority].end())
        {
            PTaskList::iterator temp = it;
            ++it;

            std::shared_ptr<Task> process = *temp;
            if (process->IsAlive())
            {
                if (immediate) process->Abort();
                processList_[priority].erase(temp);
            }
        }
    }
}