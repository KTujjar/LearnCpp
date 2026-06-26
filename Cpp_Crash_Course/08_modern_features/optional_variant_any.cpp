// ============================================================================
// Topic: std::optional, std::variant, std::any — vocabulary types (C++17)
// Compile: g++ -std=c++20 -Wall -Wextra optional_variant_any.cpp -o out && ./out
//
// Key takeaways:
//   * optional<T>: "a T, or nothing" — model absence without null pointers/sentinels.
//   * variant<A,B,...>: a TYPE-SAFE union — holds exactly one of several types.
//   * any: holds a value of ANY type (rarely needed; prefer variant when types known).
//   * Use std::visit for variants; these types make invalid states unrepresentable.
// ============================================================================

#include <iostream>
#include <optional>
#include <string>
#include <variant>

// optional return: "maybe a value." Clearer than -1 / nullptr sentinels.
std::optional<int> parsePositive(const std::string& s) {
    try {
        int v = std::stoi(s);
        if (v > 0) return v;     // wraps the value
    } catch (...) {}
    return std::nullopt;         // explicitly "no value"
}

int main() {
    // --- std::optional ---------------------------------------------------
    for (const std::string in : {"42", "-3", "abc"}) {
        if (auto r = parsePositive(in)) {     // optional is contextually bool
            std::cout << "parsed '" << in << "' -> " << *r << "\n";  // *r unwraps
        } else {
            std::cout << "parsed '" << in << "' -> (none)\n";
        }
    }
    std::optional<int> maybe;
    std::cout << "value_or fallback = " << maybe.value_or(-1) << "\n";  // -1 if empty

    // --- std::variant: a safe tagged union -------------------------------
    std::variant<int, double, std::string> var;
    var = 7;                         // currently holds int
    std::cout << "variant holds index " << var.index() << "\n";   // 0 = int
    var = std::string("hello");      // now holds string
    std::cout << "variant holds index " << var.index() << "\n";   // 2 = string

    // std::visit applies a callable to whichever type is active right now.
    std::visit([](const auto& value) {
        std::cout << "visited value: " << value << "\n";
    }, var);

    // std::get_if safely probes for a specific alternative (nullptr if not active).
    var = 3.14;
    if (auto* d = std::get_if<double>(&var))
        std::cout << "variant is a double = " << *d << "\n";

    // INTERVIEW: "optional vs nullptr? variant vs union?"
    //   optional<T> expresses "maybe present" without a magic sentinel or extra
    //   allocation, and the compiler forces you to check. variant is a type-safe
    //   union that always knows which alternative is active (no UB from reading
    //   the wrong member, unlike a raw C union). Together they let you encode
    //   "valid states only," catching bugs at compile time.
    return 0;
}
