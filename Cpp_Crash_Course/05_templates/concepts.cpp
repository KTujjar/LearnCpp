// ============================================================================
// Topic: C++20 Concepts — readable constraints on template parameters
// Compile: g++ -std=c++20 -Wall -Wextra concepts.cpp -o out && ./out
//
// Key takeaways:
//   * A concept is a NAMED compile-time predicate on types.
//   * Constrain templates so misuse fails with a CLEAR message, not 200 lines.
//   * `requires` clauses / concept names document "what a type must support."
//   * Concepts replace most old SFINAE tricks with readable code.
// ============================================================================

#include <concepts>   // std::integral, std::floating_point, ...
#include <iostream>
#include <string>

// Define a concept: "Number" = an integral OR floating-point type.
template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

// Constrain a template with the concept. Calling with a non-Number is a clean
// compile error ("constraints not satisfied"), not a cryptic template dump.
template <Number T>
T half(T value) { return value / T{2}; }

// `requires` clause form: spell out the exact expressions a type must support.
template <typename T>
requires requires(T a, T b) { a + b; a < b; }   // T must support + and <
T clampLow(T value, T low) { return (value < low) ? low : value; }

// A concept that checks for a member function via a requires-expression.
template <typename T>
concept HasSize = requires(const T& t) {
    { t.size() } -> std::convertible_to<std::size_t>;  // t.size() must exist & convert
};

template <HasSize C>
void reportSize(const C& c) { std::cout << "  size = " << c.size() << "\n"; }

int main() {
    std::cout << "half(10)    = " << half(10) << "\n";      // int  -> 5
    std::cout << "half(3.0)   = " << half(3.0) << "\n";     // double -> 1.5
    // half("oops");   // ERROR: 'std::string' does not satisfy 'Number' (clear!)

    std::cout << "clampLow(2, 5) = " << clampLow(2, 5) << "\n";   // 5
    std::cout << "clampLow(9, 5) = " << clampLow(9, 5) << "\n";   // 9

    std::string s = "hello";
    reportSize(s);   // std::string has .size() -> satisfies HasSize
    // reportSize(42);  // ERROR: int has no .size() -> 'HasSize' not satisfied

    // INTERVIEW: "What problem do concepts solve?"
    //   They constrain templates with named, readable requirements so (1) invalid
    //   instantiations produce short, meaningful diagnostics, (2) interfaces
    //   self-document what a type must provide, and (3) overloads can be selected
    //   by capability. They supersede most SFINAE/enable_if boilerplate.
    return 0;
}
