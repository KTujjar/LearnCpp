// ============================================================================
// Topic: Copy elision, RVO/NRVO — why "return by value" is cheap now
// Compile: g++ -std=c++20 -Wall -Wextra copy_elision_rvo.cpp -o out && ./out
//
// Key takeaways:
//   * RVO = Return Value Optimization: the returned object is built DIRECTLY in
//     the caller's storage — no copy or move at all.
//   * In C++17, RVO for a returned temporary (prvalue) is GUARANTEED.
//   * So returning big objects by value is idiomatic and fast — don't return by
//     reference/pointer just to "avoid a copy."
//   * DON'T write `return std::move(x);` — it can DISABLE NRVO. Just `return x;`.
// ============================================================================

#include <iostream>

struct Tracer {
    Tracer()                       { std::cout << "  ctor\n"; }
    Tracer(const Tracer&)          { std::cout << "  COPY ctor\n"; }
    Tracer(Tracer&&) noexcept      { std::cout << "  MOVE ctor\n"; }
    ~Tracer()                      { std::cout << "  dtor\n"; }
};

// Returns a temporary (prvalue). C++17 GUARANTEES no copy/move here.
Tracer makeByValue() {
    return Tracer{};   // built directly in the caller's slot -> "ctor" only
}

// Named Return Value Optimization (NRVO): returning a named local. Allowed but
// not guaranteed; every mainstream compiler does it at -O.
Tracer makeNamed() {
    Tracer local;      // named object
    return local;      // NRVO: usually constructed in-place, no copy/move
    // return std::move(local);  // ANTI-PATTERN: forces a move, blocks NRVO.
}

int main() {
    std::cout << "makeByValue() (guaranteed elision):\n";
    Tracer a = makeByValue();   // expect: ctor ... dtor  (NO copy/move ctor)

    std::cout << "makeNamed() (NRVO):\n";
    Tracer b = makeNamed();     // expect: ctor ... dtor  (move only if NRVO off)

    std::cout << "end of main:\n";

    // INTERVIEW: "Is returning a big object by value slow?"
    //   No. With guaranteed copy elision (C++17) for prvalues, and NRVO for named
    //   locals, there's typically zero copy and zero move. Returning by value is
    //   the clean, correct default. And never `return std::move(local)` — it can
    //   pessimize by preventing NRVO.
    (void)a; (void)b;
    return 0;
}
