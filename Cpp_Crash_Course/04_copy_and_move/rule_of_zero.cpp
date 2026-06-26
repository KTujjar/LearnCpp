// ============================================================================
// Topic: The Rule of Zero — let the compiler manage everything
// Compile: g++ -std=c++20 -Wall -Wextra rule_of_zero.cpp -o out && ./out
//
// Key takeaways:
//   * Rule of Zero: design classes that need NONE of the five special members.
//   * Achieve it by holding RAII members (vector, string, unique_ptr, ...).
//   * The compiler-generated copy/move/dtor then "just work" member-by-member.
//   * This is the MODERN default — only write the five when wrapping a raw resource.
// ============================================================================

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// This class manages NO raw resources directly. Every member is already an RAII
// type that knows how to copy, move, and destroy itself. So we write ZERO of the
// special members — the compiler synthesizes correct ones automatically.
class Profile {
    std::string                  name_;     // owns its own buffer
    std::vector<int>             scores_;    // owns its own heap array
    std::shared_ptr<std::string> bio_;       // shared, ref-counted

public:
    Profile(std::string name, std::vector<int> scores, std::string bio)
        : name_(std::move(name)),
          scores_(std::move(scores)),
          bio_(std::make_shared<std::string>(std::move(bio))) {}

    // NO destructor, NO copy ctor/assign, NO move ctor/assign written here.
    // Copying a Profile deep-copies name_ and scores_, and shares bio_.
    // Moving a Profile moves each member. All generated, all correct.

    void print() const {
        std::cout << "  " << name_ << " | scores=" << scores_.size()
                  << " | bio=\"" << *bio_ << "\"\n";
    }
    void addScore(int s) { scores_.push_back(s); }
};

int main() {
    Profile original("Ada", {90, 85}, "pioneer");
    original.print();

    Profile copy = original;          // compiler-generated COPY: works correctly
    copy.addScore(100);               // independent vector -> doesn't affect original
    std::cout << "after copy.addScore(100):\n";
    original.print();                 // still 2 scores
    copy.print();                     // 3 scores

    Profile moved = std::move(copy);  // compiler-generated MOVE: members moved
    moved.print();

    // INTERVIEW: "When do you follow the Rule of Five vs Zero?"
    //   Rule of Zero whenever possible: compose RAII members and write none of
    //   the five. Reach for Rule of Five ONLY when your class directly owns a raw
    //   resource (raw pointer, file descriptor, OS handle) the compiler can't
    //   reason about. Fewer hand-written special members = fewer bugs.
    return 0;
}
