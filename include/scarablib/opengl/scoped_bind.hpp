#pragma once

template <typename T>
// Provides RAII-based binding and unbinding for OpenGL objects
class ScopedBind {
	public:
		// `bound_obj` needs to have a method called `bind` and a method called `unbind`
		explicit ScopedBind(const T& bound_obj);
		~ScopedBind();

		// Prevent copying to avoid multiple bindings
		ScopedBind(const ScopedBind<T>&) = delete;
		ScopedBind& operator=(const ScopedBind<T>&) = delete;

		// Allow moving
		ScopedBind(ScopedBind<T>&&) = default;
		ScopedBind& operator=(ScopedBind<T>&&) = default;

	private:
		const T& bound_obj;
};


template <typename T>
ScopedBind<T>::ScopedBind(const T& bound_obj) : bound_obj(bound_obj) {
	// Bind at creation
	this->bound_obj.bind();
}

template <typename T>
ScopedBind<T>::~ScopedBind() {
	// Unbind when exiting scope
	this->bound_obj.unbind();
}
