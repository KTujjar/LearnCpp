// ============================================================================
// Topic: std::thread — launching and joining threads
// Compile: g++ -std=c++20 -Wall -Wextra -pthread threads.cpp -o out && ./out
//   (the -pthread flag is required for std::thread on Linux/macOS gcc)
//
// Key takeaways:
//   * std::thread runs a callable on a new OS thread; it starts immediately.
//   * You MUST join() (wait for) or detach() every thread before it's destroyed,
//     or the program std::terminate()s.
//   * Arguments are COPIED into the thread by default; use std::ref to pass refs.
//   * std::jthread (C++20) auto-joins in its destructor — prefer it when available.
// ============================================================================

#include <iostream>
#include <thread>
#include <vector>

void worker(int id) {
    std::cout << "  worker " << id << " running on thread "
              << std::this_thread::get_id() << "\n";
}

// Takes a reference: caller must pass it via std::ref, else it'd be copied.
void addInto(int& total, int value) { total += value; }

int main() {
    // --- Launch a single thread ------------------------------------------
    std::thread t(worker, 1);   // starts immediately; args copied into the thread
    t.join();                   // block until t finishes (REQUIRED before t dies)

    // --- A pool of threads -----------------------------------------------
    std::vector<std::thread> pool;
    for (int i = 2; i <= 4; ++i)
        pool.emplace_back(worker, i);     // construct threads in place
    for (auto& th : pool) th.join();      // join EVERY thread

    // --- Passing a reference with std::ref -------------------------------
    int total = 0;
    std::thread adder(addInto, std::ref(total), 42);  // without ref -> copies total
    adder.join();
    std::cout << "total after thread = " << total << "\n";   // 42

    // --- std::jthread (C++20): RAII join, no manual .join() needed --------
    // Guarded because some standard libraries (e.g. Apple libc++) ship jthread late.
#ifdef __cpp_lib_jthread
    {
        std::jthread auto_joined(worker, 99);  // destructor joins automatically
    }   // <-- auto_joined.join() happens here implicitly. Exception-safe.
#else
    std::cout << "(std::jthread not available in this stdlib; use std::thread+join)\n";
#endif

    std::cout << "main thread = " << std::this_thread::get_id() << "\n";

    // INTERVIEW: "join vs detach?"
    //   join(): wait for the thread to complete (you keep a handle on its lifetime).
    //   detach(): let it run independently; you lose the handle and must ensure it
    //   doesn't outlive data it uses. Forgetting BOTH -> std::terminate when the
    //   std::thread destructs while still joinable. std::jthread fixes this via RAII.
    return 0;
}
