// ============================================================================
// Topic: Mutexes & RAII locks — protecting shared data from data races
// Compile: g++ -std=c++20 -Wall -Wextra -pthread mutex_and_locks.cpp -o out && ./out
//
// Key takeaways:
//   * A data race (concurrent unsynchronized read+write) is Undefined Behavior.
//   * A mutex provides MUTUAL EXCLUSION: only one thread holds it at a time.
//   * NEVER lock/unlock by hand — use RAII guards (lock_guard, unique_lock,
//     scoped_lock) so the mutex is released even on early return/exception.
//   * Deadlock: lock multiple mutexes in a CONSISTENT order, or use std::scoped_lock.
// ============================================================================

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int             counter = 0;
std::mutex      mtx;          // guards `counter`

void incrementManyTimes(int times) {
    for (int i = 0; i < times; ++i) {
        // lock_guard locks mtx now and UNLOCKS automatically at scope end (RAII).
        std::lock_guard<std::mutex> guard(mtx);
        ++counter;            // critical section: only one thread in here at a time
    }   // <-- guard's destructor unlocks mtx here, even if an exception threw
}

int main() {
    // --- Without a mutex, ++counter from many threads is a DATA RACE ------
    // (incrementing is read-modify-write; concurrent ones lose updates -> UB).
    constexpr int kThreads = 4, kPerThread = 100'000;

    std::vector<std::thread> pool;
    for (int i = 0; i < kThreads; ++i)
        pool.emplace_back(incrementManyTimes, kPerThread);
    for (auto& t : pool) t.join();

    // With the mutex, every increment is serialized -> exact, deterministic total.
    std::cout << "counter = " << counter
              << " (expected " << kThreads * kPerThread << ")\n";

    // --- Locking two mutexes safely: std::scoped_lock (C++17) ------------
    // Locking m1 then m2 in one thread and m2 then m1 in another -> DEADLOCK.
    // scoped_lock locks ALL given mutexes at once using a deadlock-avoidance algo.
    std::mutex m1, m2;
    {
        std::scoped_lock lock(m1, m2);   // acquires both without deadlock risk
        std::cout << "held m1 and m2 together safely\n";
    }   // both released here

    // INTERVIEW: "How do you avoid deadlock?"
    //   (1) Always acquire multiple locks in the SAME global order. (2) Or use
    //   std::scoped_lock / std::lock to take them atomically. (3) Hold locks for
    //   as short a time as possible. (4) Prefer higher-level tools (atomics,
    //   concurrent queues) when you can avoid explicit locking entirely.
    return 0;
}
