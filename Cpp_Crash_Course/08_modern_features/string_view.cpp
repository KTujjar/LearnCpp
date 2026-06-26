// ============================================================================
// Topic: std::string_view — a non-owning view into character data (C++17)
// Compile: g++ -std=c++20 -Wall -Wextra string_view.cpp -o out && ./out
//
// Key takeaways:
//   * string_view = (pointer + length) into existing chars. It owns NOTHING.
//   * Take string_view params to accept std::string, char*, and literals — no copy.
//   * substr() on a view is O(1) (just adjusts the window), unlike std::string.
//   * DANGER: a view must not outlive the data it points to (no dangling!).
// ============================================================================

#include <iostream>
#include <string>
#include <string_view>

// Accepts ANY string-like source WITHOUT copying. Compare to `const std::string&`
// which would force a heap allocation+copy when called with a string literal.
void logMessage(std::string_view msg) {
    std::cout << "  [" << msg.size() << " chars] " << msg << "\n";
}

// Count vowels by SCANNING a view — no copies even when slicing.
std::size_t countVowels(std::string_view s) {
    std::size_t n = 0;
    for (char c : s)
        if (std::string_view("aeiouAEIOU").find(c) != std::string_view::npos) ++n;
    return n;
}

int main() {
    std::string owned = "hello world";
    const char* cstr  = "C-string literal";

    // All three callers — no allocation, no copy of the characters:
    logMessage(owned);             // from std::string
    logMessage(cstr);              // from const char*
    logMessage("inline literal");  // from a string literal

    // --- O(1) substrings via a window, not a copy ------------------------
    std::string_view view = owned;
    std::string_view word = view.substr(6);     // "world" — just moves the window
    std::cout << "substr view = '" << word << "' (no allocation)\n";
    std::cout << "vowels in 'world' = " << countVowels(word) << "\n";

    // --- The dangling trap (do NOT do this) ------------------------------
    // std::string_view bad = std::string("temporary");  // temp dies immediately
    // std::cout << bad;   // UB: view points at freed memory.
    std::cout << "(a view must not outlive the string it references)\n";

    // INTERVIEW: "When use string_view vs const string&?"
    //   Use string_view for read-only string PARAMETERS: it binds to std::string,
    //   char*, and literals with zero copies, and substr is O(1). Caveats: it's
    //   non-owning (watch lifetimes) and NOT guaranteed null-terminated, so don't
    //   hand .data() to C APIs expecting a terminator.
    return 0;
}
