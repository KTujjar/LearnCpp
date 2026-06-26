// ============================================================================
// Topic: Stack vs Heap — storage duration and where objects live
// Compile: g++ -std=c++20 -Wall -Wextra stack_vs_heap.cpp -o out && ./out
//
// Key takeaways:
//   * Stack: automatic lifetime, freed when scope ends, very fast, limited size.
//   * Heap: manual lifetime via new/delete, larger, slower, risk of leaks.
//   * Modern C++ rule: AVOID raw new/delete — use containers or smart pointers.
//   * Every `new` needs exactly one matching `delete` (and new[]/delete[]).
// ============================================================================

#include <iostream>

struct Widget {
    int id;
    Widget(int i) : id(i) { std::cout << "  Widget(" << id << ") constructed\n"; }
    ~Widget()            { std::cout << "  Widget(" << id << ") destroyed\n"; }
};

void stackExample() {
    std::cout << "stackExample():\n";
    Widget w(1);   // lives on the STACK. Destroyed AUTOMATICALLY at scope exit.
    std::cout << "  using w.id=" << w.id << "\n";
}   // <-- w's destructor runs here, no cleanup code needed. This is the magic.

void heapExampleManual() {
    std::cout << "heapExampleManual():\n";
    Widget* w = new Widget(2);   // lives on the HEAP. You OWN its lifetime.
    std::cout << "  using w->id=" << w->id << "\n";
    delete w;   // MUST delete or you leak. If an exception threw before this -> leak.
    // This is exactly why we prefer smart pointers (see smart_pointers.cpp).
}

int main() {
    stackExample();        // ctor + automatic dtor
    heapExampleManual();   // ctor + MANUAL dtor

    // --- Why does the heap exist? ----------------------------------------
    //  * Objects whose size is known only at runtime (e.g., user input).
    //  * Objects that must OUTLIVE the function that created them.
    //  * Very large objects (the stack is small, often ~1-8 MB).
    int n = 5;
    int* arr = new int[n];           // dynamic array, size decided at runtime
    for (int i = 0; i < n; ++i) arr[i] = i * i;
    std::cout << "heap array: ";
    for (int i = 0; i < n; ++i) std::cout << arr[i] << ' ';
    std::cout << "\n";
    delete[] arr;                    // array form: delete[] matches new[]

    // INTERVIEW: "Stack vs heap?"
    //   Stack -> automatic storage, LIFO, fast allocation (just move a pointer),
    //            freed at scope end, limited size, no fragmentation.
    //   Heap  -> dynamic storage, flexible lifetime, slower, you manage it,
    //            can fragment and leak. Modern C++ wraps it (vector, unique_ptr).
    return 0;
}
