// ============================================================================
// Topic: Class templates — generic types (how std::vector et al. are built)
// Compile: g++ -std=c++20 -Wall -Wextra class_templates.cpp -o out && ./out
//
// Key takeaways:
//   * A class template parameterizes a class over one or more types (or values).
//   * Non-type template params (like `size_t N`) let size be a compile-time constant.
//   * Class template argument deduction (CTAD, C++17) lets you skip <T> sometimes.
//   * Member functions of a template are themselves instantiated on demand.
// ============================================================================

#include <iostream>
#include <stdexcept>

// A tiny fixed-capacity stack, generic over element type T AND capacity N.
// N is a NON-TYPE template parameter: a compile-time value baked into the type.
template <typename T, std::size_t N>
class FixedStack {
    T           data_[N];
    std::size_t size_ = 0;
public:
    void push(const T& value) {
        if (size_ >= N) throw std::overflow_error("stack full");
        data_[size_++] = value;
    }
    T pop() {
        if (size_ == 0) throw std::underflow_error("stack empty");
        return data_[--size_];
    }
    bool        empty() const { return size_ == 0; }
    std::size_t size()  const { return size_; }
    static constexpr std::size_t capacity() { return N; }
};

// A simple one-type wrapper to demonstrate CTAD below.
template <typename T>
struct Box {
    T value;
};

int main() {
    FixedStack<int, 3> s;           // T=int, N=3 -> a distinct, concrete type
    s.push(1); s.push(2); s.push(3);
    std::cout << "capacity=" << s.capacity() << " size=" << s.size() << "\n";

    try { s.push(4); }              // exceeds N=3
    catch (const std::exception& e) { std::cout << "  caught: " << e.what() << "\n"; }

    std::cout << "pop sequence: ";
    while (!s.empty()) std::cout << s.pop() << ' ';   // LIFO: 3 2 1
    std::cout << "\n";

    // --- CTAD (C++17): deduce class template args from the initializer -----
    Box b{42};          // deduced as Box<int>  (no need to write Box<int>{42})
    Box t{std::string("hi")};  // Box<std::string>
    std::cout << "Box b.value=" << b.value << " t.value=" << t.value << "\n";

    // INTERVIEW: "Non-type template parameter?" -> a compile-time value (often
    // an integer) used as a template argument, e.g. std::array<int, N>. It makes
    // the size part of the TYPE, enabling stack allocation and bounds known at
    // compile time. CTAD lets `std::vector v{1,2,3};` deduce `std::vector<int>`.
    return 0;
}
