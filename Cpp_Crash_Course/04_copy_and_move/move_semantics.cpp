// ============================================================================
// Topic: Move semantics — rvalue references and std::move
// Compile: g++ -std=c++20 -Wall -Wextra move_semantics.cpp -o out && ./out
//
// Key takeaways:
//   * An lvalue has a name/address; an rvalue is a temporary about to expire.
//   * `T&&` binds to rvalues; it's how we detect "you can steal my guts."
//   * std::move does NOT move — it just CASTS to an rvalue ref to enable moving.
//   * Moving a heavy object (e.g. a vector) is O(1) pointer theft vs O(n) copy.
// ============================================================================

#include <iostream>
#include <string>
#include <vector>

// Two overloads let us SEE which one the compiler picks:
void sink(const std::string& s) { std::cout << "  copy overload: " << s << "\n"; }
void sink(std::string&& s)      { std::cout << "  move overload: " << s << "\n"; }

int main() {
    std::string name = "Ada";

    // lvalue -> picks the const& (copy) overload
    sink(name);

    // rvalue (a temporary) -> picks the && (move) overload
    sink(std::string("temp"));

    // std::move casts an lvalue to an rvalue so the && overload is chosen.
    // We're telling the compiler "I'm done with `name`, you may pillage it."
    sink(std::move(name));
    // After being moved-from, `name` is VALID but UNSPECIFIED — don't rely on its
    // contents; you may only assign to it or destroy it safely.

    // --- Why moves are fast: vector example ------------------------------
    std::vector<int> big(1'000'000, 7);    // a million ints on the heap
    std::cout << "big.size() before = " << big.size() << "\n";

    std::vector<int> moved = std::move(big);  // O(1): just steals the heap pointer.
    // A COPY here would allocate + copy a million ints (O(n)). The move just
    // swaps three pointers and leaves `big` empty.
    std::cout << "moved.size()      = " << moved.size() << "\n";
    std::cout << "big.size() after  = " << big.size() << " (moved-from, now empty)\n";

    // --- Move into a container -------------------------------------------
    std::vector<std::string> words;
    std::string w = "expensive-to-copy-string";
    words.push_back(std::move(w));   // move w's buffer into the vector (no copy)
    std::cout << "words[0] = " << words[0] << "\n";

    // INTERVIEW: "What does std::move do?"
    //   Nothing at runtime by itself — it's a static_cast to T&&. It merely makes
    //   an object eligible for the move constructor/assignment. The ACTUAL moving
    //   is done by those move members (which steal resources and null the source).
    return 0;
}
