// ============================================================================
// Topic: "Explain X" — concise answers to classic C++ interview questions
// Compile: g++ -std=c++20 -Wall -Wextra faq_answers.cpp -o out && ./out
//
// This file is mostly a curated Q&A you can read like flashcards. The tiny
// demos below illustrate a couple of the trickier answers. Read every // Q/A.
// ============================================================================

#include <iostream>

int main() {
    std::cout << "Read the Q&A comments in this file — they're interview flashcards.\n";

    // Q: What's the difference between a pointer and a reference?
    // A: A reference is an alias to an existing object: can't be null, can't be
    //    rebound, must be initialized. A pointer is a variable holding an address:
    //    can be null, reassigned, and supports arithmetic. (See 01_basics.)

    // Q: struct vs class in C++?
    // A: Only the DEFAULT access: struct members/inheritance are public by default,
    //    class is private. Otherwise identical. Convention: struct for plain data.

    // Q: What does `static` mean? (it's overloaded!)
    // A: (1) At file scope -> internal linkage (visible only in this .cpp).
    //    (2) Local variable -> persists across calls, initialized once.
    //    (3) Class member -> shared by all instances, not per-object.
    static int callCount = 0;          // (2) keeps its value between calls
    std::cout << "static local demo, callCount = " << ++callCount << "\n";

    // Q: When is a virtual destructor required?
    // A: Whenever you delete a derived object through a base-class pointer. Without
    //    a virtual dtor only the base dtor runs -> the derived part leaks (UB).

    // Q: What is RAII?
    // A: Resource Acquisition Is Initialization: tie a resource's lifetime to an
    //    object; acquire in the ctor, release in the dtor. Gives automatic,
    //    exception-safe cleanup. It underpins smart pointers, locks, fstream, etc.

    // Q: unique_ptr vs shared_ptr?
    // A: unique_ptr = single owner, zero overhead, move-only (default). shared_ptr
    //    = shared ownership via an atomic reference count (overhead); use weak_ptr
    //    to observe without owning and to break reference cycles.

    // Q: What does std::move actually do?
    // A: Nothing at runtime by itself — it's a cast to an rvalue reference (T&&),
    //    marking an object as eligible to be moved-from. The move ctor/assignment
    //    does the real work of stealing resources.

    // Q: Why pass big objects by const reference?
    // A: To avoid copying. const promises you won't modify the caller's object and
    //    lets the function accept const arguments and temporaries.

    // Q: Difference between compile-time and run-time polymorphism?
    // A: Compile-time: templates & function overloading (resolved by the compiler).
    //    Run-time: virtual functions via the vtable (resolved by the object's
    //    dynamic type). Templates = zero dispatch cost; virtuals = one indirection.

    // Q: What's the Rule of Five / Zero?
    // A: If you manually manage a resource and write one of {dtor, copy ctor, copy
    //    assign, move ctor, move assign}, you likely need all five. Better: compose
    //    RAII members so you need NONE (Rule of Zero) — the modern default.

    // Q: const vs constexpr?
    // A: const = won't be modified after init (runtime or compile time). constexpr
    //    = computable at compile time (and usable where constants are required).

    // Q: Stack vs heap?
    // A: Stack = automatic lifetime, fast, scope-bound, limited size. Heap = manual
    //    or smart-pointer-managed lifetime, flexible, slower, can fragment/leak.

    // Q: What is a memory leak and how do you prevent it?
    // A: Allocated memory that's never freed. Prevent with RAII / smart pointers /
    //    standard containers so cleanup is automatic; avoid raw new/delete.

    // INTERVIEW META-TIP: answer in two beats — a one-sentence definition, then a
    // "why it matters / when to use it." Tie answers back to RAII and ownership;
    // most modern-C++ questions reduce to "who owns this and when is it freed?"
    return 0;
}
