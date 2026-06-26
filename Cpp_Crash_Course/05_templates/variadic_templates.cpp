// ============================================================================
// Topic: Variadic templates — functions taking any number of args
// Compile: g++ -std=c++20 -Wall -Wextra variadic_templates.cpp -o out && ./out
//
// Key takeaways:
//   * A parameter pack `Args...` holds zero or more types/values.
//   * C++17 FOLD EXPRESSIONS collapse a pack with an operator in one line.
//   * Variadics power things like printf-safe logging, std::make_unique, tuples.
//   * It's all compile-time: the compiler unrolls the pack into concrete code.
// ============================================================================

#include <iostream>
#include <string>

// --- Fold expression: sum any number of arguments --------------------------
// `(args + ...)` expands to (a0 + (a1 + (a2 + ...))). Empty pack -> needs care,
// so we require at least one argument via the first parameter.
template <typename... Args>
auto sum(Args... args) {
    return (args + ...);    // unary right fold over operator+
}

// --- Fold over the stream operator: print all args, space-separated --------
template <typename... Args>
void printAll(const Args&... args) {
    // ((std::cout << arg << ' '), ...) is a fold over the comma operator:
    // it evaluates the left expression once per argument, in order.
    ((std::cout << args << ' '), ...);
    std::cout << "\n";
}

// --- Count the arguments at compile time -----------------------------------
template <typename... Args>
constexpr std::size_t countArgs(const Args&...) {
    return sizeof...(Args);   // sizeof... yields the pack size as a constant
}

int main() {
    std::cout << "sum(1,2,3,4)            = " << sum(1, 2, 3, 4) << "\n";   // 10
    std::cout << "sum(1.5, 2.5)           = " << sum(1.5, 2.5) << "\n";     // 4.0

    std::cout << "printAll: ";
    printAll(1, "two", 3.0, std::string("four"));   // heterogeneous args, all printed

    std::cout << "countArgs(a,b,c)        = "
              << countArgs(10, 20, 30) << "\n";       // 3, known at compile time

    // INTERVIEW: "How do variadic templates work?"
    //   The compiler deduces a parameter pack and expands it at compile time.
    //   Pre-C++17 you'd recurse (peel one arg, recurse on the rest); C++17 fold
    //   expressions express the same expansion in a single concise statement.
    //   `sizeof...(pack)` gives the count. Used by tuple, make_*, format, etc.
    return 0;
}
