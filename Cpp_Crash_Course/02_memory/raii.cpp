// ============================================================================
// Topic: RAII — Resource Acquisition Is Initialization (the heart of C++)
// Compile: g++ -std=c++20 -Wall -Wextra raii.cpp -o out && ./out
//
// Key takeaways:
//   * RAII ties a resource's lifetime to an object's lifetime (scope).
//   * Acquire in the constructor; release in the destructor.
//   * Cleanup happens AUTOMATICALLY at scope exit — even when exceptions throw.
//   * Every standard resource wrapper (vector, string, lock_guard, fstream,
//     unique_ptr) is RAII. Understanding RAII explains all of them.
// ============================================================================

#include <iostream>
#include <stdexcept>

// A toy RAII wrapper around a "file handle". Replace with any resource:
// sockets, locks, DB connections, GPU buffers — the pattern is identical.
class FileHandle {
    std::string name_;
public:
    explicit FileHandle(std::string name) : name_(std::move(name)) {
        // ACQUIRE the resource in the constructor.
        std::cout << "  OPEN  " << name_ << "\n";
    }
    ~FileHandle() {
        // RELEASE the resource in the destructor. Runs no matter HOW we leave scope.
        std::cout << "  CLOSE " << name_ << "\n";
    }
    void write(const std::string& data) {
        std::cout << "  WRITE " << name_ << ": " << data << "\n";
    }
};

void normalPath() {
    std::cout << "normalPath():\n";
    FileHandle f("config.txt");
    f.write("hello");
}   // <-- destructor closes the file here. No explicit close() call needed.

void exceptionPath() {
    std::cout << "exceptionPath():\n";
    try {
        FileHandle f("data.txt");
        f.write("start");
        throw std::runtime_error("boom!");   // something goes wrong mid-function
        f.write("never reached");
    } catch (const std::exception& e) {
        // KEY POINT: even though we threw, f's destructor already ran during
        // stack unwinding, so the file was CLOSED. No leak. This is why RAII +
        // exceptions are safe, while manual new/delete + exceptions leak.
        std::cout << "  caught: " << e.what() << " (file already closed!)\n";
    }
}

int main() {
    normalPath();
    exceptionPath();

    // INTERVIEW: "What is RAII and why does it matter?"
    //   It binds resource cleanup to object destruction, which the compiler
    //   guarantees at scope exit (including during exception stack unwinding).
    //   Result: no manual cleanup, no leaks, exception-safe code. It's the
    //   foundation of modern C++ resource management (smart pointers, locks...).
    return 0;
}
