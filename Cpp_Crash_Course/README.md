# C++ Crash Course (Modern C++17/20) — Interview Prep

A self-contained, hands-on refresher for modern C++, built for technical interviews.
Every file is a small, heavily-commented, **independently compilable** program with a
`main()` you can run. Comments explain *how* the code works and *why* it matters in
modern C++ (and where it shows up in interviews).

## How to compile & run

Every file works standalone. Use C++20:

```bash
g++ -std=c++20 -Wall -Wextra path/to/file.cpp -o out && ./out
```

On macOS, `g++` is Apple Clang and works fine. `clang++` works too.
A couple of files use threads — link pthreads if your platform needs it:

```bash
g++ -std=c++20 -pthread 09_concurrency/threads.cpp -o out && ./out
```

Tip: compile the whole folder at once to sanity-check everything:

```bash
for f in $(find . -name '*.cpp'); do echo "== $f =="; g++ -std=c++20 -Wall "$f" -o /tmp/o 2>&1 | head; done
```

## Suggested learning path

Go in numeric order — each builds on the last.

| #  | Folder                | What you'll learn                                               |
|----|-----------------------|----------------------------------------------------------------|
| 01 | `01_basics`           | Types, `auto`, references vs pointers, `const`/`constexpr`, functions, control flow |
| 02 | `02_memory`           | Stack vs heap, **RAII** (the heart of C++), smart pointers, ownership bugs |
| 03 | `03_oop`              | Classes, inheritance, polymorphism, **how vtables work**, static/friend |
| 04 | `04_copy_and_move`    | **Rule of 5/0**, move semantics, `std::move`, copy elision / RVO |
| 05 | `05_templates`        | Function/class templates, variadics, **C++20 concepts**, type traits |
| 06 | `06_stl_containers`   | `vector`, `map`/`set`, `array`/`span`, choosing the right container |
| 07 | `07_stl_algorithms`   | Lambdas, `<algorithm>`, iterators, **C++20 ranges** |
| 08 | `08_modern_features`  | `optional`/`variant`/`any`, `string_view`, structured bindings, `format`/`chrono` |
| 09 | `09_concurrency`      | Threads, mutexes/locks, atomics, `async`/`future` |
| 10 | `10_error_handling`   | Exceptions + RAII, `noexcept`, error-as-value patterns |
| 11 | `11_interview_topics` | Undefined behavior, Big-O cheat sheet, classic gotchas, FAQ answers |

## How to use this for interviews

1. **Read top-to-bottom.** Each header comment lists "Key takeaways" — those are the
   one-liners you want to be able to say out loud.
2. **Run every file**, then break it on purpose (comment out a `std::move`, add a copy)
   and re-run to see what changes.
3. **Folders 04, 03/vtables, and 11** are the highest-yield for interviews.
4. Treat the comments labeled `// INTERVIEW:` as flashcards.

Happy hacking. 🚀
