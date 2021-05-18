#include "PTask.hpp"

void Task::AttachChild(std::shared_ptr<Task> child)
{
    if (child_) RemoveChild();
    child_ = child;
}

std::shared_ptr<Task> Task::RemoveChild()
{
    std::shared_ptr<Task> child;
    if (child_)
    {
        child = child_;
        child_->SetState(TaskState::Removed);
        child_ = nullptr;
    }
    return child;
}
