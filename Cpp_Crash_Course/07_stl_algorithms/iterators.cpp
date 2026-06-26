// ============================================================================
// Topic: Iterators — the glue between containers and algorithms
// Compile: g++ -std=c++20 -Wall -Wextra iterators.cpp -o out && ./out
//
// Key takeaways:
//   * An iterator GENERALIZES a pointer: dereference (*), advance (++), compare.
//   * Ranges are half-open [begin, end): end() is "one past the last", not an element.
//   * Iterator CATEGORIES define what you can do (forward, bidirectional, random).
//   * This abstraction is why one algorithm works across vector, list, map, etc.
// ============================================================================

#include <algorithm>  // std::copy
#include <iostream>
#include <iterator>   // std::distance, std::advance, std::back_inserter
#include <list>
#include <vector>

int main() {
    std::vector<int> v{10, 20, 30, 40};

    // --- begin()/end() and the half-open range ---------------------------
    // Loop runs while it != end(). end() points JUST PAST the last element.
    std::cout << "manual iterate: ";
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << ' ';     // *it dereferences to the element
    }
    std::cout << "\n";

    // --- Random-access iterators (vector): jump in O(1) ------------------
    auto it = v.begin();
    it += 2;                          // only valid for random-access iterators
    std::cout << "v.begin()+2 -> " << *it << "\n";          // 30
    std::cout << "distance(begin,end) = "
              << std::distance(v.begin(), v.end()) << "\n";  // 4

    // --- Bidirectional iterators (list): ++ and -- but NO + n -------------
    std::list<int> l{1, 2, 3};
    auto lit = l.begin();
    std::advance(lit, 2);             // advance() works for ANY category (steps if needed)
    std::cout << "list 3rd element = " << *lit << "\n";      // 3
    // lit += 2;  // ERROR: std::list iterators are not random-access.

    // --- reverse iterators ------------------------------------------------
    std::cout << "reversed: ";
    for (auto rit = v.rbegin(); rit != v.rend(); ++rit) std::cout << *rit << ' ';
    std::cout << "\n";

    // --- insert iterators: adapt "assign" into "push_back" ---------------
    std::vector<int> dest;
    std::copy(v.begin(), v.end(), std::back_inserter(dest));  // grows dest as it copies
    std::cout << "copied via back_inserter, size=" << dest.size() << "\n";

    // INTERVIEW: "Iterator categories?"
    //   input/output (single pass) < forward (multi-pass) < bidirectional (--)
    //   < random-access (+ n, [], O(1) jumps) < contiguous (C++20, elements adjacent
    //   in memory). Algorithms state the WEAKEST category they need; e.g. std::sort
    //   needs random-access, which is why std::list has its own .sort() member.
    return 0;
}
