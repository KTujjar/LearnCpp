// ============================================================================
// Topic: Lambdas — anonymous functions and closures
// Compile: g++ -std=c++20 -Wall -Wextra lambdas.cpp -o out && ./out
//
// Key takeaways:
//   * A lambda is a compiler-generated function object (a struct with operator()).
//   * The CAPTURE list controls how it grabs surrounding variables ([=], [&], [x]).
//   * Capture by reference [&] is fast but dangles if the lambda outlives the var.
//   * std::function can store any callable, at the cost of a heap/indirection.
// ============================================================================

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

int main() {
    // --- Anatomy:  [captures](params) -> returnType { body } -------------
    auto add = [](int a, int b) { return a + b; };   // return type deduced
    std::cout << "add(2,3) = " << add(2, 3) << "\n";

    // --- Captures: pulling in local variables ----------------------------
    int factor = 10;
    auto byValue = [factor](int x) { return x * factor; };   // COPIES factor (snapshot)
    auto byRef   = [&factor](int x) { return x * factor; };  // REFERS to factor (live)

    factor = 100;     // changes after the lambdas were created
    std::cout << "byValue(2) = " << byValue(2)   // 2*10  = 20 (captured old value)
              << "  byRef(2) = " << byRef(2) << "\n";        // 2*100 = 200 (live)

    // --- mutable: allow a by-value capture to be modified inside ----------
    auto counter = [count = 0]() mutable { return ++count; };  // init-capture (C++14)
    std::cout << "counter: " << counter() << counter() << counter() << "\n"; // 123

    // --- Lambdas shine as algorithm predicates ---------------------------
    std::vector<int> nums{5, 2, 8, 1, 9, 3};
    std::sort(nums.begin(), nums.end(), [](int a, int b) { return a > b; }); // desc
    std::cout << "sorted desc: ";
    for (int n : nums) std::cout << n << ' ';
    std::cout << "\n";

    int threshold = 4;
    auto count = std::count_if(nums.begin(), nums.end(),
                               [threshold](int n) { return n > threshold; });
    std::cout << "count > " << threshold << " = " << count << "\n";

    // --- std::function: type-erased holder for any callable --------------
    // Useful for storing callbacks of differing concrete lambda types in one place.
    std::function<int(int)> op = [](int x) { return x + 1; };
    op = [](int x) { return x * x; };   // reassign to a different callable
    std::cout << "std::function op(5) = " << op(5) << "\n";   // 25

    // INTERVIEW: "[=] vs [&]?"
    //   [=] copies captured variables (a safe snapshot, but copies cost). [&]
    //   captures by reference (cheap, sees later changes, but DANGLES if the
    //   lambda outlives those variables — e.g. stored for async use). Capture
    //   exactly what you need, not blanket [=]/[&], to make intent/lifetime clear.
    return 0;
}
