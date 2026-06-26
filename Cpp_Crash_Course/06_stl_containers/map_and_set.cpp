// ============================================================================
// Topic: Associative containers — map/set (ordered) vs unordered_* (hashed)
// Compile: g++ -std=c++20 -Wall -Wextra map_and_set.cpp -o out && ./out
//
// Key takeaways:
//   * map/set: balanced BST, keys kept SORTED, O(log n) ops.
//   * unordered_map/set: hash table, NO order, average O(1) ops.
//   * Pick unordered for raw speed; pick ordered when you need sorted traversal/ranges.
//   * operator[] on a map INSERTS a default value if the key is missing (gotcha!).
// ============================================================================

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>

int main() {
    // --- std::map: sorted by key, O(log n) -------------------------------
    std::map<std::string, int> ages{{"Charlie", 30}, {"Alice", 25}, {"Bob", 28}};
    ages["Dave"] = 40;                 // insert
    std::cout << "map iterates in SORTED key order:\n";
    for (const auto& [name, age] : ages)
        std::cout << "  " << name << " -> " << age << "\n";  // Alice, Bob, Charlie...

    // operator[] GOTCHA: reading a missing key INSERTS it with a value-initialized 0.
    std::cout << "ages[\"Zoe\"] = " << ages["Zoe"]   // inserts Zoe -> 0
              << "  (size now " << ages.size() << ")\n";
    // To check WITHOUT inserting, use .find() or .contains() (C++20):
    std::cout << "contains Alice? " << ages.contains("Alice") << "\n";

    // --- std::unordered_map: hashed, average O(1) ------------------------
    std::unordered_map<std::string, int> scores{{"x", 1}, {"y", 2}, {"z", 3}};
    scores["w"] = 4;
    std::cout << "unordered_map lookup scores[\"y\"] = " << scores["y"] << "\n";
    // Iteration order is unspecified (bucket order) -> don't rely on it.

    // --- std::set: unique, sorted elements -------------------------------
    std::set<int> s{5, 1, 3, 1, 2};    // duplicate 1 ignored
    s.insert(4);
    std::cout << "set (sorted, unique): ";
    for (int x : s) std::cout << x << ' ';   // 1 2 3 4 5
    std::cout << "\n";

    // Ordered containers support RANGE queries the hashed ones can't:
    auto lo = s.lower_bound(2);        // first element >= 2
    auto hi = s.upper_bound(4);        // first element >  4
    std::cout << "elements in [2,4]: ";
    for (auto it = lo; it != hi; ++it) std::cout << *it << ' ';
    std::cout << "\n";

    // INTERVIEW: "map vs unordered_map?"
    //   map: red-black tree, keys sorted, O(log n), supports ordered traversal
    //   and lower_bound/upper_bound range queries. unordered_map: hash table,
    //   average O(1) lookup/insert but O(n) worst case (collisions), no ordering.
    //   Default to unordered_map for speed unless you need sorted iteration.
    return 0;
}
