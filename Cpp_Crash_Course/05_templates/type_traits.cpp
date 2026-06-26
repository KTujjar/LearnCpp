// ============================================================================
// Topic: Type traits, if constexpr, and the old SFINAE pattern
// Compile: g++ -std=c++20 -Wall -Wextra type_traits.cpp -o out && ./out
//
// Key takeaways:
//   * <type_traits> answers compile-time questions about types (is_integral, ...).
//   * `if constexpr` picks a branch at COMPILE time; the dead branch is discarded.
//   * SFINAE (enable_if) = the classic pre-concepts way to enable/disable overloads.
//   * Modern code prefers `if constexpr` and concepts over SFINAE for readability.
// ============================================================================

#include <iostream>
#include <type_traits>

// --- if constexpr: one function, compile-time branch selection -------------
// The FALSE branch is not even compiled for a given T, so each branch may use
// operations valid only for its type. This replaces many template specializations.
template <typename T>
std::string describe(T value) {
    if constexpr (std::is_integral_v<T>) {
        return "integral: " + std::to_string(value);
    } else if constexpr (std::is_floating_point_v<T>) {
        return "floating: " + std::to_string(value);
    } else {
        return "other type";
    }
}

// --- SFINAE with enable_if (the historical technique) ----------------------
// "Substitution Failure Is Not An Error": if the enable_if condition is false,
// this overload is silently removed from the candidate set rather than erroring.
template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
T onlyForIntegers(T x) { return x * 2; }

int main() {
    std::cout << describe(42)    << "\n";   // integral
    std::cout << describe(3.14)  << "\n";   // floating
    std::cout << describe('A')   << "\n";   // integral (char is integral)

    // --- A few trait queries (all evaluated at compile time) -------------
    std::cout << std::boolalpha;
    std::cout << "is_integral<int>       = " << std::is_integral_v<int> << "\n";
    std::cout << "is_pointer<int*>       = " << std::is_pointer_v<int*> << "\n";
    std::cout << "is_same<int,long>      = " << std::is_same_v<int, long> << "\n";
    std::cout << "remove_const same?     = "
              << std::is_same_v<std::remove_const_t<const int>, int> << "\n";

    std::cout << "onlyForIntegers(21)    = " << onlyForIntegers(21) << "\n";
    // onlyForIntegers(3.0);  // no matching overload (SFINAE removed it for double)

    // INTERVIEW: "if constexpr vs SFINAE?"
    //   if constexpr (C++17): compile-time branching INSIDE one function; the
    //   untaken branch isn't instantiated. SFINAE (enable_if): selects/removes
    //   whole OVERLOADS based on a condition, but the syntax is gnarly. C++20
    //   concepts now express the same intent far more cleanly (see concepts.cpp).
    return 0;
}
