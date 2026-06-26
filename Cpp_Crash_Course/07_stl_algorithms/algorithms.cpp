// ============================================================================
// Topic: <algorithm> & <numeric> — prefer standard algorithms over raw loops
// Compile: g++ -std=c++20 -Wall -Wextra algorithms.cpp -o out && ./out
//
// Key takeaways:
//   * Standard algorithms are correct, optimized, and express INTENT clearly.
//   * They operate on iterator RANGES [begin, end), so they work on any container.
//   * Common set: sort, find, count, accumulate, transform, all_of, min/max_element.
//   * "No raw loops" — Sean Parent's mantra: reach for an algorithm first.
// ============================================================================

#include <algorithm>
#include <iostream>
#include <numeric>     // std::accumulate, std::iota
#include <vector>

int main() {
    std::vector<int> v{5, 3, 8, 1, 9, 2, 7};

    // --- sort -------------------------------------------------------------
    std::sort(v.begin(), v.end());           // ascending by default
    std::cout << "sorted: ";
    for (int n : v) std::cout << n << ' ';
    std::cout << "\n";

    // --- find / binary_search --------------------------------------------
    auto it = std::find(v.begin(), v.end(), 8);   // linear search, any container
    std::cout << "found 8? " << (it != v.end()) << "\n";
    std::cout << "binary_search(7)? "
              << std::binary_search(v.begin(), v.end(), 7) << "\n";  // needs sorted

    // --- accumulate: fold a range into a single value --------------------
    int sum  = std::accumulate(v.begin(), v.end(), 0);             // 0 + each element
    int prod = std::accumulate(v.begin(), v.end(), 1,              // custom op
                               [](int acc, int x) { return acc * x; });
    std::cout << "sum=" << sum << " product=" << prod << "\n";

    // --- transform: map each element to a new value ----------------------
    std::vector<int> squares(v.size());
    std::transform(v.begin(), v.end(), squares.begin(),
                   [](int x) { return x * x; });
    std::cout << "squares: ";
    for (int n : squares) std::cout << n << ' ';
    std::cout << "\n";

    // --- predicates: all_of / any_of / none_of ---------------------------
    bool allPositive = std::all_of(v.begin(), v.end(), [](int x){ return x > 0; });
    std::cout << "all positive? " << allPositive << "\n";

    // --- min/max_element --------------------------------------------------
    std::cout << "min=" << *std::min_element(v.begin(), v.end())
              << " max=" << *std::max_element(v.begin(), v.end()) << "\n";

    // --- iota: fill with an increasing sequence --------------------------
    std::vector<int> seq(5);
    std::iota(seq.begin(), seq.end(), 1);    // 1,2,3,4,5
    std::cout << "iota: ";
    for (int n : seq) std::cout << n << ' ';
    std::cout << "\n";

    // INTERVIEW: "Why prefer algorithms over hand-written loops?"
    //   They communicate intent ("sort", "count_if"), are well-tested and
    //   optimized, reduce off-by-one/iterator bugs, and compose with lambdas.
    //   They also express the right complexity and let the library pick the best
    //   implementation. C++20 ranges make them even cleaner (see ranges.cpp).
    return 0;
}
