// ============================================================================
// Topic: Structured bindings — unpack tuples/pairs/structs (C++17)
// Compile: g++ -std=c++20 -Wall -Wextra structured_bindings.cpp -o out && ./out
//
// Key takeaways:
//   * `auto [a, b] = expr;` names the pieces of a pair/tuple/struct directly.
//   * Cleaner than .first/.second or std::get<>; reads like the data's shape.
//   * Use `auto&` / `const auto&` to bind by reference (modify / avoid copies).
//   * Pairs with map iteration and multi-return functions become very readable.
// ============================================================================

#include <iostream>
#include <map>
#include <string>
#include <tuple>

// A function returning several values — bundle them in a tuple/struct.
std::tuple<int, double, std::string> getStats() {
    return {42, 3.14, "ok"};
}

// A small struct: structured bindings work on public data members too.
struct Point { int x; int y; };

int main() {
    // --- Unpack a tuple --------------------------------------------------
    auto [count, ratio, status] = getStats();   // names instead of std::get<0>, ...
    std::cout << "count=" << count << " ratio=" << ratio
              << " status=" << status << "\n";

    // --- Unpack a struct -------------------------------------------------
    Point p{3, 7};
    auto [px, py] = p;          // px=3, py=7 (copies of the members)
    std::cout << "point: x=" << px << " y=" << py << "\n";

    // --- Bind by reference to MODIFY in place ----------------------------
    auto& [rx, ry] = p;         // rx, ry alias p.x, p.y
    rx = 100;                   // writes through to p.x
    std::cout << "after rx=100, p.x=" << p.x << "\n";   // 100

    // --- The killer use case: iterating a map ----------------------------
    std::map<std::string, int> inventory{{"apples", 5}, {"pears", 2}};
    std::cout << "inventory:\n";
    for (const auto& [item, qty] : inventory) {   // vs it->first / it->second
        std::cout << "  " << item << " x" << qty << "\n";
    }

    // --- Pair returned by map::insert ------------------------------------
    auto [it, inserted] = inventory.insert({"apples", 99});  // apples already exists
    std::cout << "insert apples again -> inserted? " << inserted   // false
              << ", existing qty=" << it->second << "\n";          // 5

    // INTERVIEW: structured bindings declare names bound to the SUBOBJECTS of a
    // tuple-like/aggregate. They make multi-return values and map traversal far
    // more readable, and with auto&/const auto& you control copy vs. reference.
    return 0;
}
