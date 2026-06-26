// ============================================================================
// Topic: Modern control flow — range-for, if/switch with initializers, etc.
// Compile: g++ -std=c++20 -Wall -Wextra control_flow.cpp -o out && ./out
//
// Key takeaways:
//   * Range-based for is the default loop for containers: clear & less error-prone.
//   * `if (init; cond)` / `switch (init; cond)` scope a helper var to the branch.
//   * Loop over `const auto&` to read without copying; `auto&` to modify.
//   * Structured bindings (auto [a,b]) unpack pairs/tuples/structs cleanly.
// ============================================================================

#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    std::vector<int> nums{10, 20, 30};

    // --- Range-based for: prefer this over index loops -------------------
    // `const auto&` = read-only, no copy (important for big elements).
    std::cout << "read: ";
    for (const auto& n : nums) std::cout << n << ' ';
    std::cout << "\n";

    // `auto&` = modify elements in place.
    for (auto& n : nums) n += 1;
    std::cout << "after +1: ";
    for (const auto& n : nums) std::cout << n << ' ';
    std::cout << "\n";

    // --- if with initializer (C++17): scope a variable to the if/else ----
    // `found` only exists inside this if/else — keeps scope tight & clean.
    std::map<std::string, int> ages{{"Ada", 36}, {"Linus", 54}};
    if (auto it = ages.find("Ada"); it != ages.end()) {
        std::cout << "Found Ada, age " << it->second << "\n";
    } else {
        std::cout << "Ada not found\n";
    }

    // --- switch with initializer (C++17) ---------------------------------
    switch (int code = 2; code) {   // `code` scoped to the switch
        case 1: std::cout << "one\n";   break;
        case 2: std::cout << "two\n";   break;
        default: std::cout << "other\n"; break;
    }

    // --- Structured bindings: unpack a map entry (key, value) ------------
    std::cout << "all ages:\n";
    for (const auto& [name, age] : ages) {   // unpack pair into name & age
        std::cout << "  " << name << " = " << age << "\n";
    }

    // INTERVIEW: range-for under the hood calls begin()/end() and dereferences
    // the iterator. The if-initializer is loved because it limits a variable's
    // lifetime to exactly the branch that needs it (less state leaking around).
    return 0;
}
