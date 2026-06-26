// ============================================================================
// Topic: std::array (fixed-size) and std::span (non-owning view, C++20)
// Compile: g++ -std=c++20 -Wall -Wextra array_and_span.cpp -o out && ./out
//
// Key takeaways:
//   * std::array<T,N>: a fixed-size array with STL niceties (size(), iterators).
//     No heap allocation; size is part of the type. Prefer it over C arrays.
//   * std::span<T>: a lightweight (pointer + length) VIEW over contiguous data.
//     It owns nothing — perfect for function parameters that accept "any contiguous
//     sequence" (array, vector, C array) without templating or copying.
// ============================================================================

#include <array>
#include <iostream>
#include <span>
#include <vector>

// One function that accepts ANY contiguous int sequence — no copy, no template.
// Works for std::array, std::vector, raw arrays. This is span's superpower.
long sumAll(std::span<const int> data) {
    long total = 0;
    for (int x : data) total += x;   // span is iterable and knows its length
    return total;
}

int main() {
    // --- std::array: stack-allocated, size in the type -------------------
    std::array<int, 4> arr{10, 20, 30, 40};
    std::cout << "array size=" << arr.size()
              << " front=" << arr.front()
              << " back=" << arr.back() << "\n";
    arr.at(1) = 25;                         // bounds-checked access like vector

    // --- std::vector and a C array for comparison ------------------------
    std::vector<int> vec{1, 2, 3, 4, 5};
    int cArray[] = {100, 200, 300};

    // --- One span-based function handles all three -----------------------
    std::cout << "sum(array)  = " << sumAll(arr)    << "\n";   // span from std::array
    std::cout << "sum(vector) = " << sumAll(vec)    << "\n";   // span from std::vector
    std::cout << "sum(C arr)  = " << sumAll(cArray) << "\n";   // span from C array

    // A span can also view a SUB-RANGE without copying:
    std::span<int> whole{vec};
    auto middle = whole.subspan(1, 3);      // elements [1..3]: {2,3,4}
    std::cout << "middle subspan sum = " << sumAll(middle) << "\n";

    // INTERVIEW: "Why std::span?"
    //   It decouples algorithms from the container type. Instead of templating on
    //   the container or passing (pointer, length) pairs, take a std::span: it's a
    //   non-owning, bounds-aware view over any contiguous storage. Zero copy, and
    //   it prevents the classic C bug of passing length separately and wrong.
    return 0;
}
