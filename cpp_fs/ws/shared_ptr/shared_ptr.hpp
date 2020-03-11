// Shared pointer
// Last update: 11/03/2020
// Author: RD 78-79
/*----------------------------------------------------------------------------*/
#ifndef __SHARED_PTR_HPP__ 
#define __SHARED_PTR_HPP__
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
template <typename T>
class SharedPtr
{
public:
	SharedPtr(T* ptr_ = 0);	// non explicit by design ?
	SharedPtr(const SharedPtr& other_);
	SharedPtr& operator=(const SharedPtr& other_);
	~SharedPtr();

// to solve:
// SharedPtr<Cat> c(...);
// SharedPtr<Student> s(...);
// void Foo(SharedPtr<Animal> a_)
// Foo(c) - V
// Foo(s) - X
// SharedPtr<Animal> a = c;	V
// SharedPtr<Animal> a = s;	X

	T* operator-> () const;
	T& operator* () const;

private:
	T* m_ptr;
	size_t* m_refernceCounter;
};
/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __SHARED_PTR_HPP__