// ============================================================================
// Topic: std::vector — the default container, and its size/capacity model
// Compile: g++ -std=c++20 -Wall -Wextra vector.cpp -o out && ./out
//
// Key takeaways:
//   * vector = a dynamic, contiguous array. Cache-friendly; your default choice.
//   * SIZE = #elements; CAPACITY = allocated slots. Growth reallocates (amortized O(1)).
//   * reserve() up front avoids repeated reallocations when you know the count.
//   * Reallocation/erase INVALIDATES iterators, pointers, and references — careful!
// ============================================================================

#include <algorithm>  // std::remove
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;            // empty: size 0, capacity 0

    // --- Watch capacity grow (reallocation) ------------------------------
    std::cout << "growth pattern (size : capacity):\n";
    int lastCap = -1;
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
        if ((int)v.capacity() != lastCap) {   // print only when capacity changes
            std::cout << "  " << v.size() << " : " << v.capacity() << "\n";
            lastCap = v.capacity();
        }
    }
    // Capacity roughly DOUBLES on each grow -> amortized O(1) push_back.

    // --- reserve(): pre-allocate to avoid reallocations ------------------
    std::vector<int> r;
    r.reserve(100);                // one allocation; no growth until 100 elements
    std::cout << "after reserve(100): size=" << r.size()
              << " capacity=" << r.capacity() << "\n";

    // --- Access patterns -------------------------------------------------
    std::cout << "v[0]=" << v[0]                  // operator[]: fast, NO bounds check
              << " v.at(1)=" << v.at(1)           // .at(): bounds-checked, may throw
              << " front=" << v.front()
              << " back="  << v.back() << "\n";

    // --- Iterator invalidation gotcha ------------------------------------
    // A push_back that triggers reallocation moves all elements to new storage,
    // so any saved pointer/iterator/reference into the OLD storage now dangles.
    std::vector<int> w{1, 2, 3};
    int* p0 = &w[0];               // points into current buffer
    w.push_back(4);                // MAY reallocate -> p0 could now dangle
    std::cout << "(p0 may be invalid after a reallocating push_back)\n";
    (void)p0;

    // --- Efficient removal: erase-remove idiom (pre-C++20) ---------------
    std::vector<int> nums{1, 2, 3, 2, 4, 2};
    // std::erase(nums, 2);  // C++20 one-liner. Pre-C++20 use remove + erase:
    nums.erase(std::remove(nums.begin(), nums.end(), 2), nums.end());
    std::cout << "after removing 2s: ";
    for (int n : nums) std::cout << n << ' ';
    std::cout << "\n";

    // INTERVIEW: "Why is vector usually fastest?"
    //   Contiguous memory -> excellent cache locality and prefetching. push_back
    //   is amortized O(1) via geometric growth. Trade-off: inserting/erasing in
    //   the MIDDLE is O(n) (shifts), and growth can invalidate references.
    return 0;
}
