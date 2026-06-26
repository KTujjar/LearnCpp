// ============================================================================
// Topic: const correctness, constexpr, and consteval
// Compile: g++ -std=c++20 -Wall -Wextra const_and_constexpr.cpp -o out && ./out
//
// Key takeaways:
//   * `const` = "I promise not to modify this." Compiler enforces it.
//   * Read pointer-const declarations right-to-left.
//   * `constexpr` = "can be computed at COMPILE time" (zero runtime cost).
//   * `consteval` (C++20) = MUST run at compile time.
//   * const-correctness makes interfaces self-documenting and safer.
// ============================================================================

#include <iostream>

// constexpr function: usable at compile time AND run time.
constexpr int square(int n) { return n * n; }

// consteval function (C++20): MUST be evaluated at compile time, never at runtime.
consteval int cubeAtCompileTime(int n) { return n * n * n; }

// const member function: promises not to modify the object's state.
struct Point {
    int x{0}, y{0};
    int sum() const { return x + y; }  // `const` after () => doesn't change *this
};

int main() {
    // --- const variables -------------------------------------------------
    const int limit = 100;
    // limit = 50;   // ERROR: cannot assign to a const variable
    std::cout << "limit=" << limit << "\n";

    // --- const with pointers (READ RIGHT-TO-LEFT) ------------------------
    int a = 1, b = 2;

    const int* ptrToConst = &a;     // "pointer to const int": can't change *ptr,
    // *ptrToConst = 5;             //   ERROR
    ptrToConst = &b;                //   ...but CAN repoint. OK.
    std::cout << "*ptrToConst=" << *ptrToConst << "\n";

    int* const constPtr = &a;       // "const pointer to int": CAN change *ptr,
    *constPtr = 5;                  //   OK
    // constPtr = &b;               //   ...but can't repoint. ERROR.

    const int* const both = &a;     // can't change value, can't repoint.
    std::cout << "a=" << a << " b=" << b << " both=" << *both << "\n";

    // --- constexpr: computed at compile time -----------------------------
    constexpr int s = square(5);    // 25 baked into the binary, no runtime work
    int runtimeN = 6;
    int s2 = square(runtimeN);      // same function also works at runtime
    std::cout << "square(5)=" << s << " square(runtime 6)=" << s2 << "\n";

    // --- consteval: forced compile-time ----------------------------------
    constexpr int c = cubeAtCompileTime(3);  // 27 at compile time
    // int rn = 3; cubeAtCompileTime(rn);     // ERROR: argument not constexpr
    std::cout << "cube(3)=" << c << "\n";

    // --- const member functions ------------------------------------------
    const Point p{3, 4};
    std::cout << "p.sum()=" << p.sum() << "\n";  // sum() is const, so callable on const p

    // INTERVIEW: why constexpr? It moves work from runtime to compile time and
    // enables values usable where constants are required (array sizes, templates).
    return 0;
}
