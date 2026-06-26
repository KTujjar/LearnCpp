// ============================================================================
// Topic: std::atomic — lock-free synchronization for simple shared state
// Compile: g++ -std=c++20 -Wall -Wextra -pthread atomics.cpp -o out && ./out
//
// Key takeaways:
//   * std::atomic<T> makes individual operations indivisible — no torn reads/writes.
//   * For a single counter/flag, an atomic is faster & simpler than a mutex.
//   * fetch_add, exchange, compare_exchange are atomic read-modify-write ops.
//   * Memory ORDER controls visibility of OTHER memory around the atomic op.
// ============================================================================

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

std::atomic<int> counter{0};   // increments are atomic -> no data race, no mutex

void bump(int times) {
    for (int i = 0; i < times; ++i)
        counter.fetch_add(1, std::memory_order_relaxed);  // atomic ++; relaxed: we
        // only need atomicity of the count here, not ordering vs other memory.
}

int main() {
    constexpr int kThreads = 4, kPer = 250'000;

    std::vector<std::thread> pool;
    for (int i = 0; i < kThreads; ++i) pool.emplace_back(bump, kPer);
    for (auto& t : pool) t.join();

    // Exact result with NO mutex: each fetch_add was indivisible.
    std::cout << "atomic counter = " << counter.load()
              << " (expected " << kThreads * kPer << ")\n";

    // --- compare_exchange: the building block of lock-free algorithms ----
    // "If the value is still `expected`, set it to `desired`; else update expected."
    std::atomic<int> value{10};
    int expected = 10;
    bool ok = value.compare_exchange_strong(expected, 20);  // CAS
    std::cout << "CAS 10->20 succeeded? " << ok
              << ", value now " << value.load() << "\n";

    // --- atomic flag as a simple spinlock signal -------------------------
    std::atomic<bool> ready{false};
    std::thread producer([&]{ ready.store(true, std::memory_order_release); });
    while (!ready.load(std::memory_order_acquire)) { /* spin until ready */ }
    producer.join();
    std::cout << "consumer saw ready=true\n";

    // INTERVIEW: "atomic vs mutex?"
    //   Atomics synchronize a SINGLE variable lock-free and are cheaper for simple
    //   counters/flags. A mutex protects a CRITICAL SECTION (multiple variables /
    //   compound invariants). Memory order (relaxed/acquire/release/seq_cst) tunes
    //   how surrounding non-atomic memory becomes visible across threads; seq_cst
    //   (the default) is easiest to reason about, relaxed is fastest but trickiest.
    return 0;
}
