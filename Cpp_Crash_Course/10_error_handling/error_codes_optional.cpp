// ============================================================================
// Topic: Errors as VALUES — optional / expected-style returns vs exceptions
// Compile: g++ -std=c++20 -Wall -Wextra error_codes_optional.cpp -o out && ./out
//
// Key takeaways:
//   * Not every failure deserves an exception. "Expected" failures can be values.
//   * std::optional<T>: success carries a T; failure is std::nullopt (no reason).
//   * A Result<T,E> (like C++23 std::expected) carries the value OR an error reason.
//   * Rule of thumb: exceptions for EXCEPTIONAL/unrecoverable; values for routine,
//     expected outcomes on a hot path (parsing, lookups, validation).
// ============================================================================

#include <iostream>
#include <optional>
#include <string>
#include <variant>

// --- optional: present-or-absent, no reason for absence --------------------
std::optional<int> findIndex(const std::string& s, char c) {
    for (std::size_t i = 0; i < s.size(); ++i)
        if (s[i] == c) return static_cast<int>(i);
    return std::nullopt;          // "not found" — a normal, expected outcome
}

// --- A minimal Result<T,E> (std::expected is the C++23 standard version) ----
// We model it with std::variant here so this compiles on C++20 toolchains.
template <typename T, typename E>
using Result = std::variant<T, E>;

Result<int, std::string> divide(int a, int b) {
    if (b == 0) return std::string("division by zero");  // the error alternative
    return a / b;                                         // the value alternative
}

int main() {
    // --- optional usage --------------------------------------------------
    if (auto idx = findIndex("hello", 'l'))
        std::cout << "found 'l' at index " << *idx << "\n";   // 2
    if (!findIndex("hello", 'z'))
        std::cout << "'z' not found (no exception needed)\n";

    // --- Result usage: inspect which alternative is active ---------------
    for (auto [a, b] : {std::pair{10, 2}, std::pair{5, 0}}) {
        auto r = divide(a, b);
        if (std::holds_alternative<int>(r))
            std::cout << a << "/" << b << " = " << std::get<int>(r) << "\n";
        else
            std::cout << a << "/" << b << " ERROR: " << std::get<std::string>(r) << "\n";
    }

    // INTERVIEW: "Exceptions vs error codes/optional/expected?"
    //   Exceptions: clean happy path, automatic propagation, but cost on throw and
    //   harder to see control flow; best for truly exceptional, rare failures.
    //   Value-returns (optional/expected): explicit, cheap, force the caller to
    //   handle failure, and shine on hot paths or library boundaries where callers
    //   want to decide. C++23 adds std::expected<T,E> as the standard Result type.
    return 0;
}
