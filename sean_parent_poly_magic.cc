#include <iostream>
#include <memory>
#include <vector>

template <typename T>
concept Runable = requires(T x) {
  {x.Run()};
};

class Object {
 public:
  template <Runable T>
  Object(T x) : self_(std::make_unique<Model<T>>(std::move(x))) {}
  Object(const Object& x) : self_(x.self_->Clone()) {}
  Object(Object&& x) = default;
  Object& operator=(const Object& x) {
    Object tmp(x);
    *this = std::move(tmp);
    return *this;
  }
  Object& operator=(Object&& x) = default;

  void Run() const { self_->Run(); }

 private:
  struct Concept {
    virtual ~Concept() = default;
    virtual std::unique_ptr<Concept> Clone() const = 0;
    virtual void Run() const = 0;
  };

  template <typename T>
  struct Model : Concept {
    Model(T x) : data_(std::move(x)) {}
    std::unique_ptr<Concept> Clone() const override {
      return std::make_unique<Model>(*this);
    }
    void Run() const override { data_.Run(); }
    T data_;
  };
  std::unique_ptr<Concept> self_;
};

struct OwnType {
  void Run() const { std::cout << "Hello, world!\n"; }
};

struct NonType {};

int main() {
  std::vector<Object> objects;
  objects.emplace_back(OwnType());
  objects.emplace_back(NonType());

  for (const auto& object : objects) object.Run();
}
