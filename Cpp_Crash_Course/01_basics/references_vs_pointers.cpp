// ============================================================================
// Topic: References vs Pointers — when and why to use each
// Compile: g++ -std=c++20 -Wall -Wextra references_vs_pointers.cpp -o out && ./out
//
// Key takeaways:
//   * Reference = an alias for an existing object. Can't be null, can't rebind.
//   * Pointer   = a variable holding an address. Can be null, can be reassigned.
//   * Prefer references for "must exist" params; pointers for "optional/rebindable".
//   * Use nullptr (not NULL or 0) for null pointers.
// ============================================================================

#include <iostream>

// Pass-by-reference: no copy, and the function can modify the caller's variable.
void doubleInPlace(int& n) { n *= 2; }

// Pass-by-const-reference: no copy, but read-only. The default for big objects.
void print(const int& n) { std::cout << "value=" << n << "\n"; }

// Pass-by-pointer: caller can pass nullptr to mean "no value here".
void incrementIfPresent(int* p) {
    if (p != nullptr) {   // pointers must be null-checked; references never can be
        (*p)++;
    }
}

int main() {
    int value = 10;

    // --- Reference basics ------------------------------------------------
    int& ref = value;   // ref is ANOTHER NAME for value; no new object created.
    ref = 20;           // writing through ref writes to value
    std::cout << "value after ref=20: " << value << "\n";   // 20

    // A reference MUST be initialized and can never be re-seated to another var:
    int other = 99;
    ref = other;        // this does NOT rebind ref to `other` — it COPIES
                        // other's value (99) INTO value. ref still aliases value.
    std::cout << "value after ref=other: " << value << "\n"; // 99

    // --- Pointer basics --------------------------------------------------
    int* ptr = &value;  // ptr stores the ADDRESS of value
    *ptr = 5;           // dereference to read/write the pointee
    std::cout << "value after *ptr=5: " << value << "\n";    // 5

    ptr = &other;       // pointers CAN be reassigned to point elsewhere
    *ptr = 7;
    std::cout << "other after *ptr=7: " << other << "\n";    // 7

    int* nothing = nullptr;  // explicit "points to nothing"

    // --- Using them through functions ------------------------------------
    doubleInPlace(value);            // value: 5 -> 10
    print(value);                    // 10
    incrementIfPresent(&value);      // 10 -> 11
    incrementIfPresent(nothing);     // safely does nothing (null-checked)
    print(value);                    // 11

    // INTERVIEW: "Reference vs pointer?"
    //   - Reference: cannot be null, cannot rebind, cleaner syntax -> use when the
    //     argument must always refer to a valid object.
    //   - Pointer: can be null and reassigned -> use for optional params, linked
    //     structures (trees/lists), or when you need pointer arithmetic.
    return 0;
}
