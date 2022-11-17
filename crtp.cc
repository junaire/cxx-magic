#include <cstddef>
#include <iostream>

template <typename Derived>
class RefCounter {
  inline static size_t count = 0;

 protected:
  RefCounter() { ++count; }
  RefCounter(const RefCounter<Derived>&) { ++count; }
  RefCounter(RefCounter<Derived>&&) { ++count; }
  ~RefCounter() { --count; }

 public:
  static size_t live() { return count; }
};

struct S : public RefCounter<S> {};

// 结合使用 CRTP 和 Barton-Nackman 技术，将一部分行为分解放到基类中。
template <typename Derived>
struct EqualityComparable {
  friend bool operator!=(const Derived& lhs, const Derived& rhs) {
    return !(lhs == rhs);
  }
};

struct X : EqualityComparable<X> {
  int d;
  X(int data) : d(data) {}
  friend bool operator==(const X& lhs, const X& rhs) { return lhs.d == rhs.d; }
};

int main() {
  std::cout << "number of S: " << S::live() << "\n";
  {
    S s1;
    std::cout << "number of S: " << S::live() << "\n";

    S s2 = s1;
    std::cout << "number of S: " << S::live() << "\n";

    S s3 = std::move(s2);
    std::cout << "number of S: " << S::live() << "\n";
  }
  std::cout << "number of S: " << S::live() << "\n";

  X x1(3);
  X x2(4);

  std::cout << "\nx1 != x2: ";
  x1 != x2 ? std::cout << "true\n" : std::cout << "false\n";
}
