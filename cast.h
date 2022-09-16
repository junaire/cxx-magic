// To use these classes in a type hierarchy, you must implement classof:
//
//     enum CarType { Minivan, ... };
//     struct Car { CarType type; ... };
//     struct Minivan : Car {
//       static bool classof(const Car* car) { return car->type == Minivan; }
//     };

template <typename Derived, typename Base>
bool isa(const Base* base) {
  return Derived::classof(base);
}

template <typename Derived, typename Base>
const Derived* cast(const Base* base) {
  assert(isa<Derived>(base));
  return static_cast<const Derived*>(base);
};

template <typename Derived, typename Base>
Derived* cast(Base* base) {
  assert(isa<Derived>(base));
  return static_cast<Derived*>(base);
};

template <typename Derived, typename Base>
const Derived* dyn_cast(const Base* base) {
  return isa<Derived>(base) ? static_cast<const Derived*>(base) : nullptr;
};

template <typename Derived, typename Base>
Derived* dyn_cast(Base* base) {
  return isa<Derived>(base) ? static_cast<Derived*>(base) : nullptr;
};
