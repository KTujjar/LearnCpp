// ============================================================================
// Topic: Smart pointers — unique_ptr, shared_ptr, weak_ptr
// Compile: g++ -std=c++20 -Wall -Wextra smart_pointers.cpp -o out && ./out
//
// Key takeaways:
//   * unique_ptr: SOLE owner, zero overhead, move-only. Your default choice.
//   * shared_ptr: SHARED ownership via reference counting (has overhead).
//   * weak_ptr: non-owning observer of a shared_ptr; breaks reference cycles.
//   * Prefer make_unique / make_shared over raw `new`.
// ============================================================================

#include <iostream>
#include <memory>

struct Resource {
    int id;
    Resource(int i) : id(i) { std::cout << "  Resource(" << id << ") ctor\n"; }
    ~Resource()             { std::cout << "  Resource(" << id << ") dtor\n"; }
    void use() const        { std::cout << "  using Resource(" << id << ")\n"; }
};

// unique_ptr is MOVE-ONLY: passing ownership requires std::move.
std::unique_ptr<Resource> makeResource(int id) {
    return std::make_unique<Resource>(id);   // ownership returned to caller
}

int main() {
    // --- unique_ptr: single owner, auto-deletes, no runtime cost ---------
    {
        std::cout << "unique_ptr:\n";
        auto u = std::make_unique<Resource>(1);  // preferred over new Resource(1)
        u->use();
        // auto u2 = u;          // ERROR: can't COPY a unique_ptr (sole owner)
        auto u2 = std::move(u);  // OK: MOVE transfers ownership; u is now null
        std::cout << "  u is " << (u ? "non-null" : "null") << " after move\n";
        u2->use();
    }   // <-- u2 goes out of scope -> Resource(1) auto-deleted. No delete call!

    // --- shared_ptr: shared ownership via a reference count --------------
    {
        std::cout << "shared_ptr:\n";
        auto s1 = std::make_shared<Resource>(2);     // ref count = 1
        std::cout << "  count=" << s1.use_count() << "\n";
        {
            auto s2 = s1;                            // COPY allowed; ref count = 2
            std::cout << "  count=" << s1.use_count() << "\n";
            s2->use();
        }   // s2 destroyed -> ref count back to 1 (Resource NOT yet deleted)
        std::cout << "  count=" << s1.use_count() << "\n";
    }   // s1 destroyed -> ref count 0 -> Resource(2) deleted here

    // --- weak_ptr: observe without owning --------------------------------
    {
        std::cout << "weak_ptr:\n";
        std::weak_ptr<Resource> w;     // doesn't affect the ref count
        {
            auto s = std::make_shared<Resource>(3);
            w = s;                     // observe s, but don't keep it alive
            if (auto locked = w.lock()) // .lock() promotes to shared_ptr IF alive
                locked->use();
        }   // s gone -> Resource(3) deleted, even though w still "points" there
        std::cout << "  w expired? " << w.expired() << "\n";  // 1 (true)
    }

    auto r = makeResource(4);
    r->use();

    // INTERVIEW: "unique vs shared vs weak?"
    //   unique  -> one owner, cheapest, default. shared -> many owners, atomic
    //   ref count (thread-safe count, NOT thread-safe pointee). weak -> observer
    //   that breaks ownership cycles (see ownership_gotchas.cpp).
    return 0;
}
