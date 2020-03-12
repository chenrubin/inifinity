#include "shared_ptr.hpp"

namespace ilrd
{
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
		T* temp_ptr = m_ptr;

		++(*other_.m_refernceCounter);
		UpdateData();		
		ReferenceHandle(other_);
	}
	
	template <typename T>
	void SharedPtr<T>::ReferenceHandle(const SharedPtr& other_)
	{
		m_refernceCounter = other_.m_refernceCounter;
		m_ptr = other_.m_ptr;
	}

	template <typename T>
	void SharedPtr<T>::UpdateData()
	{
		if (1 == *(m_refernceCounter))
		{
			delete m_ptr;
		}
		else
		{
			--(m_refernceCounter);
			m_ptr = NULL;
		} 
	}

	template <typename T>
	SharedPtr<T>::~SharedPtr()
	{
		UpdateData();
	}
}