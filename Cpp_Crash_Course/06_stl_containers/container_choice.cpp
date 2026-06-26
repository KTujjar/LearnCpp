// ============================================================================
// Topic: Choosing the right STL container (a classic interview decision)
// Compile: g++ -std=c++20 -Wall -Wextra container_choice.cpp -o out && ./out
//
// Key takeaways:
//   * Default to std::vector — contiguous memory wins on modern hardware.
//   * Need key->value lookup? unordered_map (speed) or map (sorted).
//   * Need a queue/stack? Use the std::queue/std::stack ADAPTERS, not raw lists.
//   * "Which container?" is really "what operations dominate, and do I need order?"
// ============================================================================

#include <deque>
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <vector>

int main() {
    // The cheat-sheet below is the real content; the code just shows the adapters.

    // --- Sequence containers ---------------------------------------------
    std::vector<int> v{1, 2, 3};   // contiguous; fast index & iterate; grows at end
    std::deque<int>  d{1, 2, 3};   // fast push/pop at BOTH ends; not fully contiguous
    std::list<int>   l{1, 2, 3};   // doubly-linked; O(1) splice/insert anywhere,
                                   // but poor cache locality -> usually AVOID

    // --- Container ADAPTERS (built on top of the above) ------------------
    std::stack<int> st;            // LIFO; defaults to std::deque underneath
    st.push(1); st.push(2);
    std::cout << "stack top (LIFO) = " << st.top() << "\n";   // 2

    std::queue<int> q;             // FIFO
    q.push(1); q.push(2);
    std::cout << "queue front (FIFO) = " << q.front() << "\n"; // 1

    std::priority_queue<int> pq;   // max-heap by default
    pq.push(3); pq.push(1); pq.push(2);
    std::cout << "priority_queue top (max) = " << pq.top() << "\n"; // 3

    std::cout << "(see the comment table below for the decision guide)\n";
    (void)v; (void)d; (void)l;

    // ----------------------------------------------------------------------
    // DECISION GUIDE (memorize this for interviews):
    //
    //   Need...                              | Use
    //   -------------------------------------+--------------------------------
    //   default / index access / iterate     | std::vector
    //   push & pop at BOTH ends              | std::deque
    //   key -> value, fastest lookup         | std::unordered_map  (avg O(1))
    //   key -> value, sorted / range queries | std::map            (O(log n))
    //   unique elements, fast membership     | std::unordered_set
    //   unique elements, sorted              | std::set
    //   LIFO                                 | std::stack
    //   FIFO                                 | std::queue
    //   always pop the largest/smallest      | std::priority_queue (heap)
    //   fixed compile-time size              | std::array
    //
    //   Reach for std::list ONLY when you need O(1) splicing of nodes and stable
    //   references on insert — its cache behavior is usually worse than vector.
    // ----------------------------------------------------------------------

    // INTERVIEW: when asked "which container?", state the dominant operation
    // (lookup? push at ends? ordered traversal?), then justify with complexity
    // AND cache locality. "vector unless I have a specific reason" is a great default.
    return 0;
}
