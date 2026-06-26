// ============================================================================
// Topic: Ownership bugs — dangling references, use-after-free, shared_ptr cycles
// Compile: g++ -std=c++20 -Wall -Wextra ownership_gotchas.cpp -o out && ./out
//
// Key takeaways:
//   * Never return a reference/pointer to a local — it dangles after return.
//   * Use-after-free and double-free are Undefined Behavior (crashes/exploits).
//   * Two shared_ptrs pointing at each other LEAK — break cycles with weak_ptr.
//   * The fixes are all about being clear on WHO OWNS the object.
// ============================================================================

#include <iostream>
#include <memory>

// ---------------------------------------------------------------------------
// GOTCHA 1: returning a reference to a local variable.
// The local dies when the function returns; the reference dangles. UB to use.
// ---------------------------------------------------------------------------
// int& danglingRef() { int local = 42; return local; }  // <- DON'T. Compiler warns.
int safeByValue() { int local = 42; return local; }       // return a COPY instead

// ---------------------------------------------------------------------------
// GOTCHA 2: shared_ptr reference CYCLE -> memory leak.
// A owns B and B owns A via shared_ptr; neither ref count ever hits 0.
// ---------------------------------------------------------------------------
struct Node {
    int value;
    std::shared_ptr<Node> next;   // strong link
    // FIX: make the BACK-link weak so it doesn't contribute to the ref count.
    std::weak_ptr<Node>   prev;   // weak link breaks the cycle
    Node(int v) : value(v) { std::cout << "  Node(" << v << ") ctor\n"; }
    ~Node()                { std::cout << "  Node(" << value << ") dtor\n"; }
};

int main() {
    // --- Safe return by value --------------------------------------------
    std::cout << "safeByValue() = " << safeByValue() << "\n";

    // --- Demonstrating the cycle FIX -------------------------------------
    std::cout << "linked nodes (weak back-link):\n";
    {
        auto a = std::make_shared<Node>(1);
        auto b = std::make_shared<Node>(2);
        a->next = b;     // a strongly owns b
        b->prev = a;     // b only WEAKLY observes a  -> no cycle
        // If b->prev were a shared_ptr<Node>, both destructors below would NEVER
        // run: each keeps the other's count at 1 forever (a classic leak).
    }   // both destructors DO run here because the cycle was broken. ✔

    // --- Use-after-free illustration (described, not executed) -----------
    // std::unique_ptr<int> p = std::make_unique<int>(5);
    // int* raw = p.get();   // raw observes, does NOT own
    // p.reset();            // frees the int
    // std::cout << *raw;    // UB: use-after-free. raw now dangles.
    std::cout << "(use-after-free example left commented; it is UB)\n";

    // INTERVIEW: "How can shared_ptr leak?" -> reference cycles. Fix: make exactly
    // one direction of the cycle a weak_ptr (typically parent/back pointers).
    return 0;
}
