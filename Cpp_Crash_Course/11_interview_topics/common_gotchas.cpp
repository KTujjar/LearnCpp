// ============================================================================
// Topic: Classic C++ gotchas interviewers love to probe
// Compile: g++ -std=c++20 -Wall -Wextra common_gotchas.cpp -o out && ./out
//
// Key takeaways:
//   * Object SLICING: copying a derived object into a base value loses the rest.
//   * Integer promotion & signed/unsigned comparison surprises.
//   * Static initialization order across translation units is unspecified.
//   * `=` vs `==`, narrowing, and shadowing are easy points to lose.
// ============================================================================

#include <iostream>
#include <vector>

struct Animal {
    virtual std::string sound() const { return "..."; }
    virtual ~Animal() = default;
};
struct Dog : Animal {
    std::string sound() const override { return "Woof"; }
};

int main() {
    // --- GOTCHA 1: object slicing ----------------------------------------
    Dog d;
    Animal sliced = d;     // COPIES only the Animal part of d -> the Dog-ness is gone
    std::cout << "1. sliced.sound() = " << sliced.sound()
              << "  (Animal, not Dog! polymorphism lost)\n";
    const Animal& ref = d; // a reference/pointer preserves the dynamic type
    std::cout << "   ref.sound()    = " << ref.sound() << "  (Dog, via reference)\n";

    // --- GOTCHA 2: signed/unsigned comparison ----------------------------
    int          s = -1;
    unsigned int u = 1;
    // -1 converts to a HUGE unsigned value, so (s < u) is FALSE. Surprise!
    // (We silence -Wsign-compare here precisely BECAUSE this mismatch is the point;
    //  in real code, heed the warning — it's flagging exactly this bug.)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
    bool surprising = (s < u);
#pragma GCC diagnostic pop
    std::cout << "2. (-1 < 1u) evaluates to " << surprising
              << "  (false! -1 converts to a large unsigned)\n";

    // Same trap with .size() (which is unsigned):
    std::vector<int> v{1, 2, 3};
    // for (int i = 0; i <= (int)v.size() - 1; ...) is safer than mixing signedness.
    std::cout << "   v.size() is unsigned; loop with care or use a signed cast\n";

    // --- GOTCHA 3: integer division & promotion --------------------------
    std::cout << "3. 5 / 2 = " << (5 / 2)            // 2, integer division
              << ", 5.0 / 2 = " << (5.0 / 2) << "\n"; // 2.5, one double operand

    // --- GOTCHA 4: narrowing caught by braces ----------------------------
    int ok{42};
    // int bad{3.9};   // ERROR with {}: narrowing. With = it would silently become 3.
    std::cout << "4. brace-init rejects narrowing (int x{3.9} won't compile)\n";
    (void)ok;

    // --- GOTCHA 5: shadowing / accidental assignment in conditions -------
    int x = 5;
    if (int y = x) { /* y is x's value, scoped to the if */ (void)y; }
    // if (x = 0) {...}   // assigns 0 then tests it -> always false. Use == !
    std::cout << "5. beware `if (x = 0)` (assignment) vs `if (x == 0)` (compare)\n";

    // INTERVIEW: object slicing is the #1 favorite — store polymorphic objects via
    // pointers/references (or smart pointers in a container), NEVER by value in a
    // base-typed variable/container, or you lose the derived behavior and data.
    return 0;
}
