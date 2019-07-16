#include <pathwatch.h>

#include <iostream>
#include <thread>
#include <mutex>


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage " << argv[0] << " /path/to/file/to/observ" << std::endl;
        return 0;
    }

    std::string file = argv[1];

    pathwatch::PathWatcher pathWatcher;

    std::mutex m;
    std::condition_variable cv;

    bool changed = false;
    pathWatcher.watch(file, [&]() {
        std::cout << "File changed" << std::endl;
        changed = true;
        cv.notify_one();
    });

    std::cout << "Waiting for a change" << std::endl;
    
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [&] { return changed; });
    

    return 0;
} 