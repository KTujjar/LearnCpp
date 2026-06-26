// ============================================================================
// Topic: Big-O cheat sheet for STL containers & algorithms
// Compile: g++ -std=c++20 -Wall -Wextra complexity_cheatsheet.cpp -o out && ./out
//
// Key takeaways:
//   * Know the complexity of the operations you USE — it drives container choice.
//   * "Amortized O(1)" (vector push_back) = O(1) on average despite rare O(n) regrows.
//   * Average vs worst case matters: hash tables are avg O(1) but worst O(n).
//   * The comment tables below are the studyable content; run it for a quick demo.
// ============================================================================

#include <iostream>

int main() {
    std::cout << "See the comment tables in this file for the cheat sheet.\n";

    // ----------------------------------------------------------------------
    // CONTAINER OPERATION COMPLEXITY
    //
    //   Container          | Access  | Search  | Insert       | Erase
    //   -------------------+---------+---------+--------------+--------------
    //   vector             | O(1)    | O(n)    | O(1) at end* | O(1) end, O(n) mid
    //   deque              | O(1)    | O(n)    | O(1) ends    | O(1) ends, O(n) mid
    //   list               | O(n)    | O(n)    | O(1) w/ iter | O(1) w/ iter
    //   map / set (tree)   | --      | O(log n)| O(log n)     | O(log n)
    //   unordered_* (hash) | --      | O(1)avg | O(1) avg     | O(1) avg
    //                      |         | O(n)wc  | O(n) worst   | O(n) worst
    //   array              | O(1)    | O(n)    | --           | --
    //
    //   * vector push_back is AMORTIZED O(1): capacity doubles, so the occasional
    //     O(n) reallocation averages out to O(1) per push.
    // ----------------------------------------------------------------------

    // ----------------------------------------------------------------------
    // COMMON ALGORITHM COMPLEXITY
    //
    //   std::sort                 -> O(n log n)   (introsort)
    //   std::stable_sort          -> O(n log n)   (extra memory)
    //   std::find / std::count    -> O(n)         (linear scan)
    //   std::binary_search        -> O(log n)     (REQUIRES sorted range)
    //   std::lower_bound          -> O(log n)     (sorted range)
    //   std::nth_element          -> O(n) average (quickselect)
    //   std::accumulate           -> O(n)
    //   std::push_heap/pop_heap   -> O(log n)
    //   std::make_heap            -> O(n)
    // ----------------------------------------------------------------------

    // ----------------------------------------------------------------------
    // BIG-O GROWTH (n = input size), slowest-growing first:
    //   O(1) < O(log n) < O(n) < O(n log n) < O(n^2) < O(2^n) < O(n!)
    // Rough "operations at n = 1,000,000":
    //   O(log n) ~ 20      O(n) ~ 1e6      O(n log n) ~ 2e7      O(n^2) ~ 1e12 (too slow)
    // ----------------------------------------------------------------------

    // INTERVIEW: interviewers love "what's the complexity?" follow-ups. Always
    // state BOTH time and (when relevant) space, note average vs worst case for
    // hash-based structures, and mention cache locality when comparing vector to
    // node-based containers (list/map) — asymptotics aren't the whole story.
    return 0;
}
