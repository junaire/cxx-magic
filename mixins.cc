#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Mixins 是一种可以定制一个类型的行为当时不需要从其进行继承的方法。
// Mixins 反转了继承方向，因为新的类型被作为了类模板的基类混合进了
// 继承层级，而不是创建一个派生类。
template <typename... Mixins>
struct PointMix : Mixins... {
  double x = 0.0;
  double y = 0.0;
  PointMix() : Mixins()... {}
  PointMix(double _x, double _y) : Mixins()..., x(_x), y(_y) {}
};

struct Label {
  std::string label = "label";
};

struct Color {
  std::string color = "Red";
};

template <typename... Mixins>
struct Polygon {
  std::vector<PointMix<Mixins...>> points;
};

struct NonVirutal {};

struct Virtual {
  virtual void foo(){};
};

template <typename... Mixins>
struct Base : Mixins... {
  void foo() { std::cout << "Base::foo()\n"; }
};

template <typename... Mixins>
struct Derived : Base<Mixins...> {
  void foo() { std::cout << "Derived::foo()\n"; }
};

int main() {
  std::unique_ptr<Base<NonVirutal>> p1(new Derived<NonVirutal>);
  p1->foo();

  std::unique_ptr<Base<Virtual>> p2(new Derived<Virtual>);
  p2->foo();
}
