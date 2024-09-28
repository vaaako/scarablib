# Scarablib Development Standards
This is a guide for development standards that I use for developing my library. The focus of these practices is to improve code readability, maintainability, and performance.

## Proprietary features and dumb decisions
In this project, I tend to develop proprietary features instead of relying on existing libraries (such as custom vectors or logging systems). This approach exists because I like to code and the desire to create my own solutions for fun. While I recognize that libraries often provide better implementations, my choice is more philosophical

# C++ features
Prefer using C++ features instead of C alternatives, as they offer better abstraction, type safety, and functionality. Exceptions can be made when a specific C feature is more efficient, simpler or readable

# std::strings and char*
Use `std::string` when performing string manipulations often. `char*` should be used when only static text is needed, which is almost always a `const` string, to minimize overhead.

# static_cast vs C style casts
C++ style casts are checked by the compiler. C style casts aren't and can fail at runtime
- static_cast is recommended
- Use static_cast for code runned many times

# Constructor and Destructor
Constructors and destructors should always be declared in the header file and initialized in the C++ file. In earlier versions, destructors were also initialized in the header, but this was changed for standardization

An exception for this is header-only structures, like the vector types

# Initializing members
If a member has a default value, initialize it directly in the declaration when possible. Initializer lists should only be used for members that require the constructor. When specific initialization is needed, such as a check, initialize it in the body of the constructor.

## Examples
### OK
```cpp
class Example {
	public:
		Constructor() {}
	private:
		int value = 5;
};
```

```cpp
Constructor(const int value) : value(value) {}
```

```cpp
Window::Window(const WindowConf& config)
	: title(config.title), width(config.width), height(config.height) {}
```

### Not ok
```cpp
class Example {
	public:
		Constructor() : value(5) {}
	private:
		int value;
};
```

# Generics
When using generics (templates) in C++, both the declaration and the implementation of the generic method must be in the same file. This is due to how C++ handles templates at compile time. To achieve this, declare the template method as normally and implement it at the end of the same header file

```cpp
class EBO {
	public:
		template <typename T = uint32>
		EBO(const std::vector<T>& indices);
		~EBO();

		inline void unbind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	private:
		GLuint id;
};

// Template must be declared and implemented in the same file
template <typename T>
EBO::EBO(const std::vector<T>& indices) {
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(T), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
```


# const keyword
The `const` keyword must ALWAYS be used, in any case that is valid.

```cpp
int get_data() const {
	return this->data;
}
```

# struct and class style
Structures should be used for simple classes, where members are public (may have some exceptions) and the objects are primarily used for simple data types. In structures, members should be declared first, followed by methods and any private members, keeping the same order in the private section

```cpp
struct Example {
	int a;
	int b;

	Example();
	int get_member();
};
```

An exception to making a struct member private is when a member has no need to be public because it will never be used externally. This differs from classes, where members are private for encapsulation, even if they could technically be public
```cpp
struct Sound {
	const char* path;

	Sound();
	~Sound();

	private:
		Mix_Music* sound = nullptr; // No need to be private
};
```

If a structure is very simple, there is no need to explicitly define constructors, as the members can use default initialization. Always follow the Rule of 5

In contrast, classes should have all members set to private by default, and provide getters and setters as needed for encapsulation
```cpp
class Example {
	public:
		Example();
		int get_member();

	private:
		int a;
		int b;
};
```

This version emphasizes the use of structures for simplicity, and classes for encapsulation, with a clear distinction between when to use each. It also reinforces the importance of following the Rule of 5


# this keyword
The `this` keyword must ALWAYS be used. This improve readability and avoid ambiguity

```cpp
int get_data(const int other) const {
	if(this->validate(other)) {
		return this->data;
	}
}
```

# Reference
Using a reference is recommended when passing an object as a parameter. It is not necessary when using raw values or enums. Using references for passing objects to functions avoids unnecessary copying and can improve performance. It’s not necessary for raw values or enums where copying is inexpensive

```cpp
void do_something(const Obj& other) {
	// ...
}
```

Not necessary:
```cpp
void do_something(const int other) {
	// ...
}
```


# constexpr
The `constexpr` specifier should be used whenever possible to allow expressions to be evaluated at compile time. If a method contains only a single expression and its arguments are known at compile time, `constexpr` must be applied. `constexpr` can only be used if all the inputs are known at compile time, and the function consists of constant expressions

## Examples
### Ok
```cpp
constexpr int fib(int n) {
	return (n <= 1) ? n : fib(n-1) + fib(n-2); 
}
```

```cpp
constexpr double pitagoras(const double a, const double b) {
	return std::sqrt((a * a) + (b * b));
}
```

### Not ok
There is no single expression here, and the constructor for this struct may be complex. This is a reason to avoid using `constexpr` when the function does not consist solely of expressions.
```cpp
constexpr vec4<uint8> to_vec4() const {
	return vec4(this->red, this->green, this->blue, this->alpha);
}
```

Neither here
```cpp
bool operator!=(const T other) const noexcept {
	return !(*this == other);
}
```

# noexcept
The `noexcept` specifier should be used when coding operators to indicate that they do not throw exceptions. It is also recommended for methods in classes used as types and are called often (for example, in any `vector` implementation). Using `noexcept` in these scenarios can improve performance by allowing the compiler to make optimizations. There is no problem using `noexcept` anywhere else, but I am standardizing its use in this way to avoid boilerplate and keep the code clean

```cpp
bool operator==(const Color& other) const noexcept {
	return (this->red == other.red)
			&& (this->green == other.green)
			&& (this->blue == other.blue)
			&& (this->alpha == other.alpha);
}
```template <typename T>
EBO<T>::EBO(const std::vector<T>& indices) {

```cpp
bool operator!=(const T other) const noexcept {
	return !(*this == other);
}
```

```cpp
void set_values(T x, T y) noexcept {
	this->x = x;
	this->y = y;
}
```

# inline keyword
The `inline` keyword must always be used for getter and setter methods, and for methods smaller than 5 lines. If an extra action is needed, such as an if-check, the `inline` keyword is still used. If more than one action or more verification is needed, the `inline` keyword should not be used. The `inline` keyword should also not be used if a scope is needed, such as a `for` loop; exceptions are scopes used for auto-deleting allocations. When a variable needs to be stored, using `inline` is also not recommended

Using `inline` for small methods can reduce function call overhead, but it's best used for methods that are simple and short. For complex methods or those involving control flow, avoiding `inline` is recommended

## Examples
### Ok
```cpp
inline uint32 get_width() const {
	return this->width;
}
```

```cpp
inline void set_title(char* title) {
	this->title = title;
	SDL_SetWindowTitle(this->window, title);
}
```

```cpp
inline void set_size(const vec2<uint32>& size) {
	this->width = size.x;
	this->height = size.y;
	SDL_SetWindowSize(this->window, (int)size.x, (int)size.y);
	glViewport(0, 0, (int)size.x, (int)size.y);
}
```

### Not ok
This method stores a variable, has more than 5 lines, and contains two conditionals (one implicit)
```cpp
inline bool Keyboard::ispressed(const Keycode& key) {
	uint32 scancode = key;

	if(this->keystate.at(scancode) == KEYDOWN) {
		this->keystate[scancode] = KEYPRESSED;
		return true;
	}

	return false;
}
```

# Pointers
Use pointers when passing ownership of resources. Always pair `new` with `delete` to manage memory effectively. Manual allocation is recommended when the deletion of the allocated memory occurs in the same class. However, smart pointers are preferred when the deletion is not certain when will occur. Avoid unnecessary allocations; for example, prefer using a `std::vector` over allocating an array with `new`. Manual allocations are recommended for large objects or when it is easier to use when allocating memory

## Examples
In the case of the `mouse` and `keyboard` handlers in `window.hpp`, allocating memory is necessary since the window has getters for these objects. They need to be pointers, allowing the return of a reference to these objects in the getter methods

```cpp
Keyboard* keyboard_handler = new Keyboard();
Mouse* mouse_handler = new Mouse();
```

The memory is deleted in the destructor, so smart pointers are not necessary here
```cpp
Window::~Window() {
	delete this->keyboard_handler;
	delete this->mouse_handler;

	// ...
}
```
