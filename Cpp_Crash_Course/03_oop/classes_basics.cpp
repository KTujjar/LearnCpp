// ============================================================================
// Topic: Classes — members, access control, constructors, member init lists
// Compile: g++ -std=c++20 -Wall -Wextra classes_basics.cpp -o out && ./out
//
// Key takeaways:
//   * `class` defaults to private, `struct` defaults to public — same otherwise.
//   * Prefer member INITIALIZER LISTS over assignment in the constructor body.
//   * Default member initializers ({}) set a baseline if a ctor doesn't.
//   * `explicit` stops surprising implicit conversions from single-arg ctors.
// ============================================================================

#include <iostream>
#include <string>

class BankAccount {
    // private by default in a `class`: only members/friends can touch these.
    std::string owner_;
    double      balance_{0.0};   // default member initializer (used if ctor omits it)

public:
    // Constructor with a MEMBER INITIALIZER LIST (the part after the colon).
    // This initializes members DIRECTLY, before the body runs. For const/reference
    // members and class-type members, it avoids a wasteful default-then-assign.
    BankAccount(std::string owner, double initial)
        : owner_(std::move(owner)),   // construct owner_ directly from the argument
          balance_(initial)           // construct balance_ directly
    {
        // Body runs AFTER members are already initialized.
        if (balance_ < 0) balance_ = 0;
    }

    // `explicit` blocks implicit conversion: prevents `BankAccount a = "Bob";`
    explicit BankAccount(std::string owner) : owner_(std::move(owner)) {}

    void deposit(double amount)  { balance_ += amount; }
    bool withdraw(double amount) {
        if (amount > balance_) return false;   // encapsulation: enforce invariants
        balance_ -= amount;
        return true;
    }

    // const getters: callable on const objects, promise not to mutate state.
    double      balance() const { return balance_; }
    const std::string& owner() const { return owner_; }
};

int main() {
    BankAccount acct("Ada", 100.0);
    acct.deposit(50);
    std::cout << acct.owner() << " balance: " << acct.balance() << "\n";  // 150

    if (!acct.withdraw(1000))
        std::cout << "  withdraw 1000 rejected (insufficient funds)\n";
    acct.withdraw(30);
    std::cout << acct.owner() << " balance: " << acct.balance() << "\n";  // 120

    BankAccount empty("Linus");   // uses the explicit single-arg ctor
    std::cout << empty.owner() << " starts at " << empty.balance() << "\n"; // 0

    // INTERVIEW: "Why member initializer lists?"
    //   (1) Required for const members, reference members, and base classes.
    //   (2) Avoids default-construct-then-assign for class-type members (faster).
    //   (3) Members init in DECLARATION order, not the order you list them.
    return 0;
}
