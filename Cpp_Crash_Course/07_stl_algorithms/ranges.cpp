// ============================================================================
// Topic: C++20 Ranges & Views — composable, lazy data pipelines
// Compile: g++ -std=c++20 -Wall -Wextra ranges.cpp -o out && ./out
//
// Key takeaways:
//   * Ranges algorithms take a whole container — no more begin()/end() pairs.
//   * VIEWS are lazy, non-owning adaptors you compose with the | (pipe) operator.
//   * Lazy = work happens only as you iterate; nothing is copied until you collect.
//   * Pipelines read top-to-bottom like the data flow, unlike nested algorithm calls.
// ============================================================================

#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // --- Range algorithm: pass the container directly --------------------
    std::ranges::sort(nums);   // no nums.begin(), nums.end() needed
    std::cout << "sorted: ";
    for (int n : nums) std::cout << n << ' ';
    std::cout << "\n";

    namespace views = std::views;

    // --- A composed VIEW pipeline ----------------------------------------
    // Read it as a pipeline: take nums -> keep evens -> square them -> first 3.
    // Nothing is computed or copied until the for-loop pulls elements through.
    auto pipeline = nums
                  | views::filter([](int n) { return n % 2 == 0; })  // 2,4,6,8,10
                  | views::transform([](int n) { return n * n; })    // 4,16,36,64,100
                  | views::take(3);                                  // 4,16,36

    std::cout << "evens^2, first 3: ";
    for (int n : pipeline) std::cout << n << ' ';   // 4 16 36 (computed lazily here)
    std::cout << "\n";

    // --- A few handy views -----------------------------------------------
    std::cout << "reversed: ";
    for (int n : nums | views::reverse) std::cout << n << ' ';
    std::cout << "\n";

    std::cout << "drop(7):  ";
    for (int n : nums | views::drop(7)) std::cout << n << ' ';   // skip first 7
    std::cout << "\n";

    // iota view: generate a sequence lazily (great for ranges-based counting).
    std::cout << "iota 0..4 squared: ";
    for (int n : views::iota(0, 5) | views::transform([](int x){ return x*x; }))
        std::cout << n << ' ';
    std::cout << "\n";

    // INTERVIEW: "What do ranges/views give you?"
    //   Composability and clarity: pipelines via | read in data-flow order and
    //   are LAZY (no intermediate containers), so filter|transform|take touches
    //   each element once and stops early. They also remove the begin()/end()
    //   boilerplate and reduce iterator-pair mistakes.
    return 0;
}
