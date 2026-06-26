// ============================================================================
// Topic: How virtual dispatch works — the vtable (an interview favorite)
// Compile: g++ -std=c++20 -Wall -Wextra vtables_explained.cpp -o out && ./out
//
// Key takeaways:
//   * Each polymorphic class has one VTABLE: an array of function pointers.
//   * Each object of such a class stores a hidden VPTR to its class's vtable.
//   * A virtual call = follow vptr -> vtable -> slot -> jump. One indirection.
//   * That's why a polymorphic object is bigger (it carries the vptr).
// ============================================================================

#include <iostream>

struct Base {
    virtual void greet() const { std::cout << "  Base::greet\n"; }
    virtual void bye()   const { std::cout << "  Base::bye\n"; }
    virtual ~Base() = default;
    int data = 0;
};

struct Derived : Base {
    void greet() const override { std::cout << "  Derived::greet\n"; }
    // bye() not overridden -> Derived's vtable slot for bye points to Base::bye
};

void callThroughBase(const Base& b) {
    // The compiler does NOT know at compile time whether b is Base or Derived.
    // It emits: load b's vptr -> index the vtable -> call whatever is in the slot.
    b.greet();   // resolved at RUNTIME via the vtable
    b.bye();
}

int main() {
    Base    base;
    Derived derived;

    std::cout << "Through references (dynamic dispatch):\n";
    callThroughBase(base);      // Base::greet,   Base::bye
    callThroughBase(derived);   // Derived::greet, Base::bye (inherited slot)

    // --- The cost of being polymorphic: a hidden vptr per object ---------
    struct Plain { int x; };                 // no virtuals -> no vptr
    struct Poly  { int x; virtual ~Poly() = default; };  // has a vptr
    std::cout << "sizeof(Plain)=" << sizeof(Plain)
              << "  sizeof(Poly)=" << sizeof(Poly)
              << "  (extra bytes are the vptr)\n";

    // --- Why `override` matters ------------------------------------------
    // Without `override`, a typo like `void greet()` (missing const) silently
    // creates a NEW function instead of overriding, and dispatch breaks.
    // `override` makes the compiler reject that mistake.

    // INTERVIEW soundbite:
    //   "Virtual functions are implemented with a per-class vtable of function
    //    pointers. Each object holds a vptr to its class's vtable. A virtual call
    //    dereferences the vptr, indexes the right slot, and jumps — one extra
    //    indirection vs a normal call, and it prevents inlining at that call site."
    return 0;
}
