// ============================================================================
// Topic: std::format (C++20) and <chrono> basics
// Compile: g++ -std=c++20 -Wall -Wextra format_and_chrono.cpp -o out && ./out
//
// NOTE: std::format needs a recent compiler/stdlib (GCC 13+, Clang 17+ / libc++).
// If <format> is unavailable on your toolchain, see the FALLBACK comment below.
//
// Key takeaways:
//   * std::format("{}...", args) = type-safe, Python-like string formatting.
//   * It replaces clumsy iostream chains and unsafe printf format strings.
//   * <chrono> gives strong types for durations and time points (no raw ints).
//   * chrono literals (1s, 500ms) make time code self-documenting and safe.
// ============================================================================

#include <chrono>
#include <format>     // FALLBACK: if missing, swap std::format lines for std::cout
#include <iostream>
#include <string>

int main() {
    // --- std::format: positional & named-ish placeholders ----------------
    std::string name = "Ada";
    int age = 36;
    std::cout << std::format("Hello, {}! You are {} years old.\n", name, age);

    // Reorder arguments with indices, and format numbers precisely:
    std::cout << std::format("{1} comes before {0}\n", "second", "first");
    std::cout << std::format("pi ~= {:.3f}\n", 3.14159);          // 3.142
    std::cout << std::format("hex={:#x}  bin={:#b}\n", 255, 5);   // 0xff, 0b101
    std::cout << std::format("padded |{:>8}|{:<8}|\n", "right", "left");

    // --- <chrono>: strongly-typed durations ------------------------------
    using namespace std::chrono;
    using namespace std::chrono_literals;     // enables 1s, 500ms, etc.

    auto timeout = 2s + 500ms;                // a duration; units tracked by the type
    std::cout << "timeout = " << duration_cast<milliseconds>(timeout).count()
              << " ms\n";

    // The type system PREVENTS unit mistakes: you can't accidentally add seconds
    // to bytes, and converting needs an explicit duration_cast.
    minutes m = 90min;
    std::cout << "90 minutes = " << duration_cast<hours>(m).count()
              << "h " << (m % 1h).count() << "m\n";   // 1h 30m

    // --- Timing a code block ---------------------------------------------
    auto start = steady_clock::now();         // monotonic clock, best for intervals
    long acc = 0;
    for (int i = 0; i < 1'000'000; ++i) acc += i;
    auto elapsed = steady_clock::now() - start;
    std::cout << std::format("loop took {} us (acc={})\n",
                             duration_cast<microseconds>(elapsed).count(), acc);

    // INTERVIEW: "Why std::format over iostreams/printf?"
    //   Type-safe like iostreams (no printf %-mismatch UB) but with concise,
    //   readable format strings and easy alignment/precision. <chrono> brings the
    //   same safety to time: durations carry their units in the TYPE, so the
    //   compiler catches unit errors and conversions are explicit.
    return 0;
}
