// ============================================================================
// Topic: Fundamental types, initialization styles, and `auto` type deduction
// Compile: g++ -std=c++20 -Wall -Wextra types_and_auto.cpp -o out && ./out
//
// Key takeaways:
//   * Prefer brace-init {} : it catches narrowing conversions at compile time.
//   * Use fixed-width types (<cstdint>) when size matters across platforms.
//   * `auto` deduces the type for you — great for verbose iterator/lambda types.
//   * `auto` strips references/const by default; add &, const, etc. explicitly.
// ============================================================================

#include <cstdint>   // int32_t, uint64_t, ... — sizes guaranteed across platforms
#include <iostream>
#include <typeinfo>
#include <vector>

int main() {
    // --- Initialization styles -------------------------------------------
    int a = 5;        // copy-initialization (C-style, still fine)
    int b(5);         // direct-initialization
    int c{5};         // brace / "uniform" initialization  <-- PREFER THIS
    // int d{3.5};    // ERROR: narrowing double->int caught by {}. This is WHY
                      // braces are safer: `int d = 3.5;` would silently truncate.
    std::cout << "a=" << a << " b=" << b << " c=" << c << "\n";

    // --- Fixed-width integer types ---------------------------------------
    // `int`/`long` sizes vary by platform. When you need an exact size
    // (file formats, network protocols, bit manipulation), be explicit.
    std::int32_t  signed32   = -1;
    std::uint64_t unsigned64 = 18'000'000'000ULL;  // ' is a digit separator (C++14)
    std::cout << "int32=" << signed32 << " uint64=" << unsigned64 << "\n";

    // --- auto: let the compiler write the type ---------------------------
    auto x = 42;            // -> int
    auto y = 3.14;          // -> double
    auto z = "hello";       // -> const char*  (NOT std::string! a common gotcha)
    auto s = std::string{"hello"}; // -> std::string, what you usually want

    // The classic win: iterator types are ugly; `auto` hides that noise.
    std::vector<int> v{1, 2, 3};
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << "\n";

    // --- auto and references/const ---------------------------------------
    // Bare `auto` COPIES and drops const/&. Be deliberate when you want a view.
    const int original = 100;
    auto        copy      = original;  // int        (const dropped -> a copy)
    const auto& reference = original;  // const int& (no copy, read-only view)
    std::cout << "copy=" << copy << " reference=" << reference << "\n";

    // INTERVIEW: "What does auto deduce here?"  auto str = "x";  -> const char*.
    // Knowing auto drops top-level const & references is a frequent question.
    std::cout << "type of x is int? " << (typeid(x) == typeid(int)) << "\n";
    std::cout << "y=" << y << "\n";
    (void)z; (void)s;  // silence unused-variable warnings
    return 0;
}
