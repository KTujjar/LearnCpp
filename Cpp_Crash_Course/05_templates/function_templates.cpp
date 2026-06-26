// ============================================================================
// Topic: Function templates — write once, work for many types
// Compile: g++ -std=c++20 -Wall -Wextra function_templates.cpp -o out && ./out
//
// Key takeaways:
//   * A template is a recipe; the compiler STAMPS OUT a concrete function per type.
//   * Template arguments are usually DEDUCED from the call's arguments.
//   * Templates are compile-time: each instantiation is type-checked & optimized.
//   * `auto` parameters (C++20) are shorthand for a function template.
// ============================================================================

#include <iostream>
#include <string>

// A single definition that works for any type T supporting operator<.
template <typename T>
T maxOf(T a, T b) {
    return (a < b) ? b : a;
}

// Multiple type parameters; return type computed from the operands.
template <typename A, typename B>
auto add(A a, B b) -> decltype(a + b) {   // trailing return: type of (a + b)
    return a + b;
}

// C++20 abbreviated template: `auto` params == a template behind the scenes.
auto multiply(auto a, auto b) { return a * b; }

int main() {
    // The compiler DEDUCES T from the arguments and generates one function each:
    std::cout << "maxOf(3, 7)        = " << maxOf(3, 7) << "\n";          // T=int
    std::cout << "maxOf(2.5, 1.5)    = " << maxOf(2.5, 1.5) << "\n";      // T=double
    std::cout << "maxOf<string>      = "
              << maxOf(std::string("apple"), std::string("banana")) << "\n"; // T=string

    // Explicitly specifying the type when you don't want deduction:
    std::cout << "maxOf<double>(3,4) = " << maxOf<double>(3, 4) << "\n";  // forces double

    // Mixed types:
    std::cout << "add(1, 2.5)        = " << add(1, 2.5) << "\n";          // -> double
    std::cout << "multiply(3, 4)     = " << multiply(3, 4) << "\n";

    // INTERVIEW: "Are templates runtime or compile time?"
    //   Compile time. The compiler generates ("instantiates") a separate, fully
    //   type-checked function for each distinct set of template arguments used.
    //   That's why template-heavy code can bloat compile times but runs fast
    //   (each instantiation is concrete and inlinable — no runtime dispatch).
    return 0;
}
