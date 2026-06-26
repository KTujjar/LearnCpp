// ============================================================================
// Topic: Functions — overloading, default args, parameter passing, attributes
// Compile: g++ -std=c++20 -Wall -Wextra functions_and_overloading.cpp -o out && ./out
//
// Key takeaways:
//   * Overloading: same name, different parameter lists. Resolved at compile time.
//   * Pass big/owning types by const& to avoid copies; small types by value.
//   * Default arguments live in the declaration; callers may omit trailing args.
//   * [[nodiscard]] warns when a return value is ignored — great for error codes.
// ============================================================================

#include <iostream>
#include <string>

// --- Overloading: the compiler picks the best match by argument types --------
void show(int n)            { std::cout << "int: "    << n << "\n"; }
void show(double d)         { std::cout << "double: " << d << "\n"; }
void show(const std::string& s) { std::cout << "string: " << s << "\n"; }

// --- Default arguments -------------------------------------------------------
// Caller may omit `base`; it defaults to 10. Defaults go on the declaration only.
int addWithBase(int x, int base = 10) { return x + base; }

// --- Parameter passing guidelines --------------------------------------------
// Small, cheap-to-copy types: pass BY VALUE (int, double, pointers, small structs).
double area(double w, double h) { return w * h; }

// Large/owning types you only read: pass BY CONST REFERENCE (no copy).
size_t length(const std::string& s) { return s.size(); }

// --- [[nodiscard]]: don't silently throw away the result ---------------------
[[nodiscard]] bool didSucceed(int code) { return code == 0; }

int main() {
    // Overload resolution happens at COMPILE time based on argument types:
    show(42);                 // -> show(int)
    show(3.14);               // -> show(double)
    show(std::string{"hi"});  // -> show(const string&)

    // Default arguments:
    std::cout << "addWithBase(5)    = " << addWithBase(5)    << "\n";  // 5 + 10
    std::cout << "addWithBase(5, 1) = " << addWithBase(5, 1) << "\n";  // 5 + 1

    std::cout << "area(2,3)   = " << area(2, 3) << "\n";
    std::cout << "length(\"hello\") = " << length("hello") << "\n";

    // [[nodiscard]] in action:
    if (didSucceed(0)) std::cout << "operation succeeded\n";
    // didSucceed(1);  // would emit: warning: ignoring return value ... [-Wunused-result]

    // INTERVIEW: "How is overloading resolved?" -> at compile time via the static
    // types of the arguments (NOT runtime). Contrast with virtual dispatch (runtime),
    // covered in 03_oop. Overload sets also include const/ref qualifiers.
    return 0;
}
