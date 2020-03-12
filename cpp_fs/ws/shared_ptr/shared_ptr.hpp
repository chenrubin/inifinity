// Shared pointer
// Last update: 12/03/2020
// Author: Chen
// Reviewer: Ofer
/*----------------------------------------------------------------------------*/
#ifndef __SHARED_PTR_HPP__ 
#define __SHARED_PTR_HPP__
/*----------------------------------------------------------------------------*/
#include <stddef.h>
namespace ilrd
{
/*----------------------------------------------------------------------------*/
template <typename T>
class SharedPtr
{
public:
	explicit SharedPtr(T* ptr_ = 0);	// non explicit by design
	
	SharedPtr(const SharedPtr& other_);
	template <typename U>
	SharedPtr(const SharedPtr<U>& other_);
	
	SharedPtr& operator=(const SharedPtr& other_);	

	~SharedPtr();
	
	T* operator-> () const;
	T& operator* () const;

private:
	template <typename U>
	friend class SharedPtr;
	/*
		assign m_refernceCounter and m_ptr to other's
	*/
	void AssignRefAndPtr(const SharedPtr& other_);

	/*
		This is th Dtor function. It either decreases ref pointer 
		or deletes entire object 
	*/
	void UpdaterefAndMaybeDelete();

	T* m_ptr;
	size_t* m_refernceCounter;
};
#endif // __SHARED_PTR_HPP__
/*----------------------------------------------------------------------------*/
	template <typename T>
	SharedPtr<T>::SharedPtr(T* ptr_)
		: m_ptr(ptr_)
		, m_refernceCounter(new size_t(1))
	{}

	template <typename T>
	SharedPtr<T>::SharedPtr(const SharedPtr& other_)
		: m_ptr(other_.m_ptr)
		, m_refernceCounter(other_.m_refernceCounter)
	{
		++(*m_refernceCounter);
	}

	template <typename T>
	template <typename U>
	SharedPtr<T>::SharedPtr(const SharedPtr<U>& other_)
		: m_ptr(other_.m_ptr)
		, m_refernceCounter(other_.m_refernceCounter)
	{
		++(*m_refernceCounter);
	}

	template <typename T>
	SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other_)
	{
		++(*other_.m_refernceCounter);
		UpdaterefAndMaybeDelete();		
		AssignRefAndPtr(other_);

		return *this;
	}
	
	template <typename T>
	void SharedPtr<T>::AssignRefAndPtr(const SharedPtr& other_)
	{
		m_refernceCounter = other_.m_refernceCounter;
		m_ptr = other_.m_ptr;
	}

	template <typename T>
	void SharedPtr<T>::UpdaterefAndMaybeDelete()
	{
		--(*m_refernceCounter);
		if (0 == *(m_refernceCounter))
		{
			delete m_ptr;
			delete m_refernceCounter;
		}
	}

	template <typename T>
	SharedPtr<T>::~SharedPtr()
	{
		UpdaterefAndMaybeDelete();
	}

	template <typename T>
	T& SharedPtr<T>::operator*() const
	{
		return *m_ptr;
	}

	template <typename T>
	T* SharedPtr<T>::operator->() const
	{
		return m_ptr;
	}

} // namespace ilrd
/*----------------------------------------------------------------------------*/