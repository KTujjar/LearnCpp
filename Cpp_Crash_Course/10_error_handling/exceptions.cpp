// ============================================================================
// Topic: Exceptions — throw/try/catch, RAII safety, and noexcept
// Compile: g++ -std=c++20 -Wall -Wextra exceptions.cpp -o out && ./out
//
// Key takeaways:
//   * throw an exception OBJECT (derive from std::exception); catch by const ref.
//   * Stack unwinding runs destructors of locals -> RAII makes code exception-safe.
//   * Catch more-derived BEFORE more-base; catch (...) is the last resort.
//   * noexcept promises a function won't throw; violating it calls std::terminate.
// ============================================================================

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

// Custom exception type: derive from std::exception (or a subclass).
class ConfigError : public std::runtime_error {
public:
    explicit ConfigError(const std::string& what) : std::runtime_error(what) {}
};

int parsePort(const std::string& s) {
    if (s.empty()) throw ConfigError("port is empty");
    int port = std::stoi(s);                  // may throw std::invalid_argument
    if (port < 1 || port > 65535)
        throw std::out_of_range("port out of range: " + s);
    return port;
}

// noexcept: a contract that this function will not throw. Enables optimizations
// (e.g. vector uses move ctors only if they're noexcept) — see move_semantics.
int safeAdd(int a, int b) noexcept { return a + b; }

struct Tracked {
    std::string name;
    explicit Tracked(std::string n) : name(std::move(n)) {}
    ~Tracked() { std::cout << "  ~Tracked(" << name << ") ran during unwinding\n"; }
};

int main() {
    // --- Catch order: specific -> general --------------------------------
    for (const std::string in : {"8080", "", "99999", "abc"}) {
        try {
            std::cout << "parsePort('" << in << "') = " << parsePort(in) << "\n";
        } catch (const ConfigError& e) {           // most specific first
            std::cout << "  ConfigError: " << e.what() << "\n";
        } catch (const std::out_of_range& e) {
            std::cout << "  out_of_range: " << e.what() << "\n";
        } catch (const std::exception& e) {        // base catches the rest (stoi's)
            std::cout << "  std::exception: " << e.what() << "\n";
        }
    }

    // --- RAII + exceptions = automatic cleanup ---------------------------
    try {
        Tracked local("local-resource");          // RAII object on the stack
        auto heap = std::make_unique<Tracked>("heap-resource");  // also RAII
        std::cout << "about to throw...\n";
        throw std::runtime_error("boom");
    } catch (const std::exception& e) {
        // Both Tracked destructors already ran during unwinding -> no leaks.
        std::cout << "  caught: " << e.what() << " (resources cleaned up)\n";
    }

    std::cout << "safeAdd(2,3) = " << safeAdd(2, 3) << "\n";

    // INTERVIEW: "Why catch by const reference?"
    //   Catching by value SLICES derived exceptions to the base type (losing info)
    //   and copies. `const T&` preserves the dynamic type and avoids the copy.
    //   Also: never let an exception escape a destructor (it can call terminate),
    //   and mark move ctors noexcept so std::vector can move instead of copy.
    return 0;
}
