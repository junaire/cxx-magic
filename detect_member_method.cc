// Detect if a Type `T` has member method `Foo`.
template<typename T>
class HasFoo {
	using Yes = char[1];
	using No = char[2];

	template<typename Tp> static Yes& has(decltype(&Tp::Foo));
	template<typename Tp> static No& has(...);
public:
	static constexpr int value = (sizeof(has<T>(0)) == sizeof(Yes));
};

struct Bar {
	void Foo() {}
};

struct Baz {
	void Foo2() {}
};

int main() {
  static_assert(HasFoo<Bar>::value);
  static_assert(HasFoo<Baz>::value);
}
