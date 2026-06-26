// ============================================================================
// Topic: The Rule of Five — the five special member functions
// Compile: g++ -std=c++20 -Wall -Wextra rule_of_five.cpp -o out && ./out
//
// Key takeaways:
//   * The five: destructor, copy ctor, copy assign, move ctor, move assign.
//   * Rule of Five: if you write ONE (because you manage a resource), you
//     probably need to handle ALL FIVE correctly.
//   * Copy = duplicate the resource. Move = steal the resource, null the source.
//   * In modern code you usually want the Rule of ZERO instead (see rule_of_zero).
// ============================================================================

#include <algorithm> // std::copy, std::swap
#include <iostream>
#include <utility>   // std::move, std::exchange

// A class that OWNS a raw heap buffer, so it must define all five to stay correct.
class Buffer {
    int*   data_;
    size_t size_;
public:
    // 1) Constructor (acquires the resource)
    explicit Buffer(size_t n) : data_(new int[n]{}), size_(n) {
        std::cout << "  ctor(" << size_ << ")\n";
    }

    // 2) Destructor (releases the resource)
    ~Buffer() { std::cout << "  dtor(" << size_ << ")\n"; delete[] data_; }

    // 3) Copy constructor — DEEP copy: allocate new storage and duplicate.
    Buffer(const Buffer& other) : data_(new int[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "  copy ctor (deep, size " << size_ << ")\n";
    }

    // 4) Copy assignment — copy-and-swap keeps it simple & exception-safe.
    Buffer& operator=(const Buffer& other) {
        std::cout << "  copy assign\n";
        Buffer tmp(other);     // do the risky copy into a temp first
        swap(tmp);             // then swap — no-throw; tmp's dtor frees old data
        return *this;
    }

    // 5) Move constructor — STEAL other's pointer; leave other in a valid empty state.
    Buffer(Buffer&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)),   // take ptr, null the source
          size_(std::exchange(other.size_, 0)) {
        std::cout << "  move ctor (stole the buffer, no allocation)\n";
    }

    // 6) Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        std::cout << "  move assign\n";
        Buffer tmp(std::move(other));
        swap(tmp);
        return *this;
    }

    void swap(Buffer& o) noexcept { std::swap(data_, o.data_); std::swap(size_, o.size_); }
    size_t size() const { return size_; }
};

int main() {
    Buffer a(3);
    Buffer b = a;             // COPY ctor: a is left intact (deep copy)
    Buffer c = std::move(a);  // MOVE ctor: c steals a's buffer; a is now empty
    std::cout << "after move, a.size()=" << a.size() << " c.size()=" << c.size() << "\n";

    b = c;                    // COPY assign
    b = std::move(c);         // MOVE assign

    // INTERVIEW: "Rule of Three/Five/Zero?"
    //   Three (pre-C++11): dtor, copy ctor, copy assign go together.
    //   Five  (C++11+):    add move ctor + move assign for performance.
    //   Zero:              design so the compiler-generated ones are correct
    //                      (hold members like std::vector/unique_ptr) — best.
    return 0;
}
