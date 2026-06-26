// ============================================================================
// Topic: Undefined Behavior (UB) — what it is and the classics to recognize
// Compile: g++ -std=c++20 -Wall -Wextra undefined_behavior.cpp -o out && ./out
//
// Key takeaways:
//   * UB = the standard imposes NO requirements. Anything may happen: crash,
//     wrong result, "works on my machine," or a security hole.
//   * The compiler may ASSUME UB never happens and optimize accordingly.
//   * Most UB examples below are COMMENTED OUT on purpose — they are traps to
//     recognize and avoid, not to execute.
//   * Tools that catch UB: -fsanitize=address,undefined (ASan/UBSan) and valgrind.
// ============================================================================

#include <iostream>
#include <vector>

int main() {
    std::cout << "Common sources of Undefined Behavior:\n";

    // 1) Out-of-bounds access (operator[] does NOT check)
    std::vector<int> v{1, 2, 3};
    // int x = v[10];          // UB: reads past the end. Use .at(10) to get an exception.
    std::cout << "  1. v[10] would be out-of-bounds UB (use .at() for checked access)\n";

    // 2) Use-after-free / dangling pointer
    // int* p = new int(5); delete p; std::cout << *p;   // UB: dereference freed memory
    std::cout << "  2. dereferencing a pointer after delete/free is UB\n";

    // 3) Reading an uninitialized variable
    // int n; std::cout << n;  // UB: n has an indeterminate value
    std::cout << "  3. reading an uninitialized local is UB (always initialize!)\n";

    // 4) Signed integer overflow (UNSIGNED overflow is defined/wraps; signed is UB)
    // int big = INT_MAX; big = big + 1;   // UB. Unsigned would wrap legally.
    std::cout << "  4. signed integer overflow is UB (unsigned wraps, that's defined)\n";

    // 5) Dereferencing nullptr
    // int* np = nullptr; std::cout << *np;  // UB
    std::cout << "  5. dereferencing nullptr is UB\n";

    // 6) Returning a reference/pointer to a local (dangles after return)
    std::cout << "  6. returning a reference to a local variable is UB to use\n";

    // 7) Modifying a variable twice without a sequence point (pre-C++17 classic)
    // int i = 0; i = i++ + ++i;   // unsequenced modification -> UB historically
    std::cout << "  7. unsequenced multiple modifications (i = i++ + ++i) is UB\n";

    // 8) Data race: concurrent unsynchronized access with at least one write
    std::cout << "  8. a data race (see 09_concurrency) is UB\n";

    // INTERVIEW: "What is undefined behavior and why care?"
    //   It's behavior the standard leaves entirely unconstrained. The optimizer is
    //   allowed to assume it never occurs, so UB can cause spooky action-at-a-
    //   distance bugs and security vulnerabilities. Defend with initialization,
    //   bounds checks, smart pointers/RAII, and run AddressSanitizer/UBSan in CI.
    return 0;
}
