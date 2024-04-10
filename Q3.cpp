#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>

// QUESTION 3

const int max_on_runway = 3;

class Aircraft {
public:
    explicit Aircraft(int id) : ac_id(id) {}

    void land() {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate landing time
    }

    int get_id() const {
        return ac_id;
    }

private:
    int ac_id;
};

class ATC {
public:
    ATC() : runways_occupied_(0) {}

    void request_landing(Aircraft& ac) {
        std::unique_lock<std::mutex> lock(mutex_);

        std::cout << "Aircraft #" << ac.get_id() << " requesting landing.\n";

        if (ac.get_id() <= 3) {
            std::cout << "Aircraft #" << ac.get_id() << " is cleared to land.\n";
            runways_occupied_++;
            lock.unlock();

            ac.land();

            lock.lock();
            runways_occupied_--;
            std::cout << "Aircraft #" << ac.get_id() << " has landed. Runway is now free.\n";
            cv_.notify_one();
        }
        else {
            if (ac.get_id() >= 7 && ac.get_id() <= 10) {
                std::cout << "Runway full. Aircraft #" << ac.get_id() << " being redirected to another airport.\n";
            }
            else {
                std::cout << "Runway full. Aircraft #" << ac.get_id() << " is waiting for landing clearance.\n";
            }
        }
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int runways_occupied_;
};

int main() {
    ATC atc;
    std::vector<Aircraft> aircrafts;
    std::vector<std::thread> threads;

    for (int i = 1; i <= 10; ++i) {
        aircrafts.emplace_back(i);
    }

    // Land aircraft 1-3 first
    for (int i = 0; i < 3; ++i) {
        atc.request_landing(aircrafts[i]);
    }

    // Create threads for each remaining aircraft's landing request
    for (int i = 3; i < aircrafts.size(); ++i) {
        threads.emplace_back([&atc, &aircrafts, i] {
            atc.request_landing(aircrafts[i]);
            });
    }

    // Join threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}



// QUESTION 6

/*

During the development of the program, two issues will arise: first, the attachment and 
detachment of threads, and second, the processing power of the computer. 
If some actions take longer than expected, there could be a hazard if an aircraft in an 
emergency cannot transmit its request to the ATC. To solve this issue, concurrent programming 
must be done accurately, ensuring that all threads join and detach at the appropriate times, 
and the processing power of the computer should be increased.

*/