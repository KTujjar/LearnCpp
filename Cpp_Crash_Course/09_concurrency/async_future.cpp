// ============================================================================
// Topic: std::async / std::future / std::promise — values across threads
// Compile: g++ -std=c++20 -Wall -Wextra -pthread async_future.cpp -o out && ./out
//
// Key takeaways:
//   * std::async runs a task (maybe on another thread) and returns a std::future.
//   * future.get() blocks until the result is ready, then returns it (once).
//   * Exceptions thrown in the task are re-thrown when you call get().
//   * promise/future is a one-shot channel to hand a value between threads.
// ============================================================================

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int slowSquare(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));  // pretend it's work
    return x * x;
}

int alwaysThrows() { throw std::runtime_error("task failed"); }

int main() {
    // --- std::async: fire off work, keep working, collect later ----------
    // std::launch::async forces a new thread (vs deferred lazy evaluation).
    std::future<int> fut = std::async(std::launch::async, slowSquare, 9);

    std::cout << "doing other work while slowSquare runs...\n";
    // ... main thread is free here ...
    int result = fut.get();   // blocks until ready, returns 81 (call get() ONCE)
    std::cout << "slowSquare(9) = " << result << "\n";

    // --- Exceptions propagate through the future -------------------------
    auto bad = std::async(std::launch::async, alwaysThrows);
    try {
        bad.get();            // the exception thrown in the task surfaces HERE
    } catch (const std::exception& e) {
        std::cout << "caught from future: " << e.what() << "\n";
    }

    // --- promise/future: manual one-shot value hand-off ------------------
    std::promise<std::string> prom;
    std::future<std::string>  f = prom.get_future();   // the receiving end

    std::thread producer([&prom] {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        prom.set_value("hello from producer");         // fulfills the future
    });

    std::cout << "received: " << f.get() << "\n";       // waits for set_value
    producer.join();

    // INTERVIEW: "async/future vs raw threads?"
    //   std::async is a higher-level abstraction: it returns a future that carries
    //   the RESULT (or exception) of the task, so you don't manage threads or
    //   shared variables by hand. Use promise/future for a custom one-shot signal
    //   between threads. For ongoing producer/consumer streams, prefer a queue +
    //   condition_variable or a higher-level concurrency library.
    return 0;
}
