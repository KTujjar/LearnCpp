// ============================================================================
// Topic: Inheritance & runtime polymorphism — virtual, override, abstract bases
// Compile: g++ -std=c++20 -Wall -Wextra inheritance_polymorphism.cpp -o out && ./out
//
// Key takeaways:
//   * `virtual` enables runtime dispatch: the DERIVED override runs via a base ptr/ref.
//   * Always add `override` — it makes the compiler verify you actually override.
//   * A pure virtual (`= 0`) makes the class ABSTRACT (cannot be instantiated).
//   * A base class used polymorphically needs a VIRTUAL DESTRUCTOR (or you leak).
// ============================================================================

#include <iostream>
#include <memory>
#include <vector>

// Abstract base class: defines an interface, can't be instantiated directly.
class Shape {
public:
    virtual double area() const = 0;          // pure virtual => abstract
    virtual void   describe() const {         // virtual with a default impl
        std::cout << "  A shape with area " << area() << "\n";
    }
    virtual ~Shape() = default;               // VIRTUAL DTOR: essential for safe
                                              // `delete` through a Shape* base ptr.
};

class Circle : public Shape {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }  // override checked
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    void describe() const override {          // customize the virtual too
        std::cout << "  Rectangle " << w_ << "x" << h_ << " = " << area() << "\n";
    }
};

int main() {
    // Store different concrete types behind a single base interface.
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(2.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));

    // POLYMORPHISM: same call, different behavior chosen at RUNTIME by the
    // actual object type — this is dynamic dispatch via the vtable.
    for (const auto& s : shapes) {
        s->describe();   // Circle uses Shape::describe; Rectangle uses its own
    }

    // Shape s;  // ERROR: cannot instantiate an abstract class (has a pure virtual)

    // INTERVIEW: "Why must a polymorphic base have a virtual destructor?"
    //   `delete basePtr;` on a non-virtual dtor only runs the BASE destructor,
    //   leaking the derived part (UB). `virtual ~Shape()` ensures the full chain
    //   of destructors runs. See vtables_explained.cpp for the mechanism.
    return 0;
}
