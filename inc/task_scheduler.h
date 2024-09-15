#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

/*
 * Task - Structure representing a task with a priority and a function.
 */
struct Task {
    int priority;                             // The priority of the task.
    std::function<void()> func;              // The function to be executed.

    /*
     * operator< - Comparison operator for priority queue ordering.
     * @param other: The other Task to compare against.
     * @return: True if the current task has a lower priority than the other task.
     */
    bool operator<(const Task& other) const {
        return priority < other.priority;
    }
};

class TaskScheduler {
public:
    /*
     * TaskScheduler - Constructor that initializes the task scheduler.
     * @param: None
     * @return: None
     */
    TaskScheduler();

    /*
     * ~TaskScheduler - Destructor that stops the scheduler and ensures thread cleanup.
     * @param: None
     * @return: None
     */
    ~TaskScheduler();

    /*
     * addTask - Adds a new task to the scheduler with a specific priority.
     * @param priority: The priority of the task, with higher values being executed first.
     * @param func: The task function to be executed.
     * @return: None
     */
    void addTask(int priority, std::function<void()> func);

    /*
     * start - Starts the task scheduler by spawning a new thread that processes tasks.
     * @param: None
     * @return: None
     */
    void start();

    /*
     * stop - Stops the task scheduler and joins the thread to ensure proper cleanup.
     * @param: None
     * @return: None
     */
    void stop();

private:
    /*
     * schedulerLoop - Main loop that continuously processes tasks based on their priority.
     * @param: None
     * @return: None
     */
    void schedulerLoop();

    std::priority_queue<Task> taskQueue;    // Queue to hold tasks based on priority.
    std::mutex queueMutex;                   // Mutex for synchronizing access to the task queue.
    std::condition_variable cv;               // Condition variable for task processing.
    bool done;                                // Flag indicating whether the scheduler should stop.
    std::thread schedulerThread;              // Thread for executing the scheduler loop.
};
