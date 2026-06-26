// ============================================================================
// Topic: static members, the `this` pointer, and `friend`
// Compile: g++ -std=c++20 -Wall -Wextra static_and_friends.cpp -o out && ./out
//
// Key takeaways:
//   * static data members belong to the CLASS, not to any instance (shared).
//   * static member functions have no `this` — call them as Class::func().
//   * Every non-static member function has an implicit `this` pointer.
//   * `friend` grants one function/class access to private members (use sparingly).
// ============================================================================

#include <iostream>

class Counter {
    static int count_;   // ONE shared variable for ALL Counter objects
    int        id_;

public:
    Counter() : id_(++count_) {   // ++count_ touches the shared class-wide counter
        std::cout << "  created Counter #" << id_ << "\n";
    }

    // static member function: no instance needed, no `this`, sees only statics.
    static int howMany() { return count_; }

    int id() const { return id_; }

    // Demonstrate `this`: returning *this enables method chaining.
    Counter& self() { return *this; }   // `this` is a Counter* to the caller object

    // `friend`: free function allowed to read private members.
    friend void debugPrint(const Counter& c);
};

// Define the static member exactly once, outside the class (one definition rule).
int Counter::count_ = 0;

// Because it's a friend, this function can access c.id_ (private).
void debugPrint(const Counter& c) {
    std::cout << "  [friend] Counter id_=" << c.id_ << "\n";
}

int main() {
    std::cout << "Counter::howMany() before = " << Counter::howMany() << "\n"; // 0

    Counter a, b, c;
    std::cout << "Counter::howMany() after  = " << Counter::howMany() << "\n"; // 3

    debugPrint(a);   // friend reaches into private state

    // `this` / chaining demo:
    std::cout << "a.self().id() = " << a.self().id() << "\n";

    // INTERVIEW notes:
    //   * static member function: belongs to the class, useful for factory
    //     functions and counters; can't access non-static members (no `this`).
    //   * `this` is a pointer to the current object; in const methods it's a
    //     pointer-to-const. Returning *this enables fluent/chained APIs.
    //   * `friend` breaks encapsulation deliberately — common for operator<<
    //     and tightly-coupled helper classes, but overuse is a design smell.
    return 0;
}
