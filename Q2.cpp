#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>


// QUESTION 2

const int num_robots = 5;
const int num_tool = 5;
const int task_duration = 5;

class Robot {
public:
    Robot(int id, int left_tool_idx, int right_tool_idx)
        : id_(id), left_tool_idx_(left_tool_idx), right_tool_idx_(right_tool_idx) {}

    void performTask() {
        bool left_tool_acquired = false;
        bool right_tool_acquired = false;

        while (!left_tool_acquired || !right_tool_acquired) {
            std::unique_lock<std::mutex> lock(mutex_);

            if (tools_[left_tool_idx_] && tools_[right_tool_idx_]) {
                tools_[left_tool_idx_] = false;
                tools_[right_tool_idx_] = false;
                left_tool_acquired = true;
                right_tool_acquired = true;
                lock.unlock();

                std::cout << "Robot " << id_ << " is collecting data. \n" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));

                lock.lock();
                std::cout << "Robot " << id_ << " acquired tools and begins its task. \n" << std::endl;
                lock.unlock();

                std::this_thread::sleep_for(std::chrono::seconds(task_duration));
                lock.lock();
                std::cout << "Robot " << id_ << " completed the task and returns the tools. \n\n" << std::endl;

                tools_[left_tool_idx_] = true;
                tools_[right_tool_idx_] = true;
            }
        }
    }

private:
    int id_;
    int left_tool_idx_;
    int right_tool_idx_;
    static std::vector<bool> tools_;
    static std::mutex mutex_;
};

std::vector<bool> Robot::tools_(num_tool, true);
std::mutex Robot::mutex_;

int main() {
    std::vector<std::thread> robots;
    auto start_time = std::chrono::steady_clock::now();

    for (int i = 0; i < num_robots; ++i) {
        int left_tool_idx = i;
        int right_tool_idx = (i + 1) % num_tool;
        robots.emplace_back([i, left_tool_idx, right_tool_idx]() {
            Robot robot(i, left_tool_idx, right_tool_idx);
            robot.performTask();
            });
    }

    for (auto& robot : robots) {
        robot.join();
    }

    auto end_time = std::chrono::steady_clock::now();
    auto total_runtime = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

    std::cout << "Total runtime of the program: " << total_runtime.count() << " seconds." << std::endl;

    return 0;
}


// QUESTION 5

/*

When a robot finishes a task and is immediately assigned a new one, it can create several challenges. 
One key issue is the potential for resource contention and deadlock, where multiple robots might 
compete for the same tools simultaneously, leading to a standstill. 
Additionally, this approach can result in unfair task allocation, as robots may not have 
equal opportunities to access tools and perform tasks.

A potential solution to mitigate these issues involves implementing a centralized task coordination system. 
This system could prioritize task assignments based on predefined criteria, such as task complexity 
or robot capabilities. By centralizing task management, it becomes possible to optimize resource 
utilization and avoid conflicts between robots vying for the same tools. Moreover, facilitating 
communication among robots allows for better coordination, enabling them to share information about 
task completion and resource availability, ultimately improving efficiency and fairness in task allocation.

*/
