#pragma once

#include "PunkDefines.hpp"

class Task
{

private:

    std::shared_ptr<Task> child_;

public:

    Task() : state_(TaskState::Uninitialized) {}
    virtual ~Task() {}

    virtual void Initialize() { state_ = TaskState::Running; OnInitialize(); };
    virtual void OnInitialize() {};
    virtual void Update(double deltaTime) { OnUpdate(deltaTime); };
    virtual void OnUpdate(double deltaTime) {};
    virtual void Pause() { state_ = TaskState::Paused; OnPause(); };
    virtual void OnPause() {};
    virtual void Resume() { state_ = TaskState::Running; OnResume(); };
    virtual void OnResume() {};
    virtual void Finish() { state_ = TaskState::Finished; OnFinish(); };
    virtual void OnFinish() {};
    virtual void Fail() { state_ = TaskState::Failed; OnFail(); };
    virtual void OnFail() {};
    virtual void Abort() { state_ = TaskState::Aborted; OnAbort(); };
    virtual void OnAbort() {};
    virtual void CleanUp() {};

    std::string ID() const { return id_; }

    TaskState State() const { return state_; }
    std::shared_ptr<Task> Child() const { return child_; }

    bool IsAlive() const { return state_ == TaskState::Running || state_ == TaskState::Paused; }
    bool IsDead() const
    {
        return state_ == TaskState::Finished || state_ == TaskState::Failed ||
            state_ == TaskState::Aborted;
    }
    bool IsPaused() const { return state_ == TaskState::Paused; }

    void SetState(TaskState state) { state_ = state; }
    void AttachChild(std::shared_ptr<Task> child);
    std::shared_ptr<Task> RemoveChild();

protected:

    std::string id_;
    TaskState state_;

};