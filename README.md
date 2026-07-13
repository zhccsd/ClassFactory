# ClassFactory

A header-only C++11 class factory that eliminates `if/else` and `switch` chains for runtime object creation.

## Features

- **No macros** — pure template-based type registration
- **Type-safe** — compile-time `static_assert` ensures `DERIVED_CLASS` inherits from `BASE_CLASS`
- **Variadic arguments** — supports arbitrary constructor parameters via parameter packs
- **Duplicate detection** — `registerType()` returns `false` if the key already exists
- **OOM handling** — returns `nullptr` on allocation failure instead of throwing

## Requirements

- C++11 or later
- RTTI enabled (required for `dynamic_cast`)

The header includes a compile-time guard that produces a clear error if RTTI is disabled.

## API

```cpp
template<typename KEY, typename... ARGS>
class ClassFactory
```

| Method | Description |
|--------|-------------|
| `bool exists(const KEY& key)` | Check if a type is registered under the given key |
| `std::list<KEY> keys()` | Return all registered keys |
| `bool registerType<BASE, DERIVED>(const KEY& key)` | Register a derived type. Returns `false` if the key already exists or allocation fails |
| `CLASS* create<CLASS>(const KEY& key, ARGS... args)` | Create an instance. Returns `nullptr` if the key is not found or the type mismatches |

## Usage

### With constructor arguments

```cpp
#include "classfactory.hpp"

struct Animal {
    virtual ~Animal() = default;
    virtual void speak() const = 0;
};

struct Dog : Animal {
    Dog(const std::string& name) : _name(name) {}
    void speak() const override { printf("%s says woof!\n", _name.c_str()); }
private:
    std::string _name;
};

struct Cat : Animal {
    Cat(const std::string& name) : _name(name) {}
    void speak() const override { printf("%s says meow!\n", _name.c_str()); }
private:
    std::string _name;
};

int main() {
    ClassFactory<std::string, const std::string&> factory;

    factory.registerType<Animal, Dog>("dog");
    factory.registerType<Animal, Cat>("cat");

    Animal* pet = factory.create<Animal>("dog", "Buddy");
    pet->speak();  // Buddy says woof!
    delete pet;
}
```

### Without constructor arguments

```cpp
#include "classfactory.hpp"

struct Shape {
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

struct Circle : Shape {
    void draw() const override { printf("○\n"); }
};

struct Square : Shape {
    void draw() const override { printf("□\n"); }
};

int main() {
    ClassFactory<std::string> factory;  // no ARGS

    factory.registerType<Shape, Circle>("circle");
    factory.registerType<Shape, Square>("square");

    Shape* s = factory.create<Shape>("circle");
    s->draw();  // ○
    delete s;
}
```

## Build

```bash
# Build with RTTI (default)
cmake -B build && cmake --build build

# Verify RTTI guard (should fail with a clear error)
cmake -B build -DENABLE_RTTI=OFF && cmake --build build
```

## License

MIT — see [LICENSE](LICENSE) for details.
