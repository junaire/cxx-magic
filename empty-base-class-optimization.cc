#include <iostream>

// C++ 标准指出，在空类被用作基类的时候，如果不给他分配内存不会
// 导致它被储存到与它同类型对象或子对象相同的地址上，则不分配内存。

class EmptyClass {};  // 分配了内存。

class Empty {
  using Int = int;
};

class EmptyFoo : public Empty {};

class EmptyBar : public EmptyFoo {};

class NonEmpty : public Empty, public EmptyFoo {};

// 当一个或两个模板参数被空的类替换时，我们会浪费一个字的内存。
template <typename T1, typename T2>
class CantUseEBCO {
  T1 a;
  T2 b;
};

// 解决办法1：
// 将数据成员变成基类继承。
// 缺点：
//     可能会给改变我们类的接口，如果模板参数类是 final 的，则无效。
template <typename T1, typename T2>
class UseEBCO1 : private T1, private T2 {};

// 解决办法2：
// 引入一个新的储存类。
template <typename Base, typename Member>
class BaseMemberPair : private Base {
  Member mem;

 public:
  BaseMemberPair(const Base& b, const Member& m) : Base(b), mem(m) {}
  Base& base() { return static_cast<const Base&>(*this); }
  const Member& member() { return this->member; }
};

template <typename T>
class UseEBCO2 {
  BaseMemberPair<T, void*> info_storage;
};

int main() {
  std::cout << "sizeof(EmptyClass): " << sizeof(EmptyClass) << "\n";

  // 大小一样。
  std::cout << "sizeof(Empty): " << sizeof(Empty) << "\n";
  std::cout << "sizeof(EmptyFoo): " << sizeof(EmptyFoo) << "\n";
  std::cout << "sizeof(EmptyBar): " << sizeof(EmptyBar) << "\n";

  // 更大一点。
  // NonEmpty 的基类和 EmptyFoo 的基类不能分配到同一地址上，否则
  // EmptyFoo 的基类 Empty 就会和 NonEmpty 的第一个基类 Empty 分配
  // 在一起。
  //
  // | Empty |              基类1 \ NonEmpty
  // | Empty | <- EmptyFoo  基类2 /
  std::cout << "sizeof(NonEmpty): " << sizeof(NonEmpty) << "\n";
}
