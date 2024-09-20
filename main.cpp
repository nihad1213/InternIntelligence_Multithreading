#include "task_scheduler.h"
#include <iostream>
#include <chrono>

/*
 * TaskScheduler::TaskScheduler - Constructor that initializes the task scheduler.
 * @param: None
 * @return: None
 */
TaskScheduler::TaskScheduler() : done(false) {}

/*
 * TaskScheduler::~TaskScheduler - Destructor that stops the scheduler and ensures thread cleanup.
 * @param: None
 * @return: None
 */
TaskScheduler::~TaskScheduler() {
    stop();
}

/*
 * TaskScheduler::addTask - Adds a new task to the scheduler with a specific priority.
 * @param priority: The priority of the task, with higher values being executed first.
 * @param func: The task function to be executed.
 * @return: None
 */
void TaskScheduler::addTask(int priority, std::function<void()> func) {
    std::lock_guard<std::mutex> lock(queueMutex);
    taskQueue.push(Task{ priority, func });
    cv.notify_one();
}

/*
 * TaskScheduler::start - Starts the task scheduler by spawning a new thread that processes tasks.
 * @param: None
 * @return: None
 */
void TaskScheduler::start() {
    schedulerThread = std::thread(&TaskScheduler::schedulerLoop, this);
}

/*
 * TaskScheduler::stop - Stops the task scheduler and joins the thread to ensure proper cleanup.
 * @param: None
 * @return: None
 */
void TaskScheduler::stop() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        done = true;
    }
    cv.notify_one();
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

/*
 * TaskScheduler::schedulerLoop - Main loop that continuously processes tasks based on their priority.
 * @param: None
 * @return: None
 */
void TaskScheduler::schedulerLoop() {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);

        cv.wait(lock, [this] { return !taskQueue.empty() || done; });

        if (done && taskQueue.empty()) {
            return;
        }

        Task task = taskQueue.top();
        taskQueue.pop();
        lock.unlock();

        task.func();
    }
}

int main() {
    TaskScheduler scheduler;

    // Start the task scheduler
    scheduler.start();

    // Add some tasks with varying priorities
    scheduler.addTask(2, []() {
        std::cout << "Task with priority 2 is running." << std::endl;
    });

    scheduler.addTask(1, []() {
        std::cout << "Task with priority 1 is running." << std::endl;
    });

    scheduler.addTask(3, []() {
        std::cout << "Task with priority 3 is running." << std::endl;
    });

    // Allow some time for tasks to be executed
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Stop the task scheduler
    scheduler.stop();

}