#pragma once
#ifndef _COWPTR_IMPL_H
#define _COWPTR_IMPL_H

//#include "../MySTL/COWPtr.h"
//#include "../COWPtr.h"

namespace MySTL
{
	//template <typename T_>
	//using cow_ptr = MySTL::cow_ptr<T_>;
	namespace Detail
	{
		//template <typename T_>
		//using proxy = MySTL::Detail::proxy<T_>;
		template <typename T>
		const T& proxy<T>::operator *() const
		{	return *(cp_->ptr_);	}
		template <typename T>
		T& proxy<T>::operator*()
		{
			auto t = *(cp_->ptr_);
			cp_->ptr_ = MySTL::make_shared<T>(t);
			return *(cp_->ptr_);
		}
		template <typename T>
		const T* proxy<T>::operator->() const
		{
			return cp_->ptr_.operator->();
		}
		template <typename T>
		T* proxy<T>::operator->()
		{
			auto t = *(cp_->ptr_);
			cp_->ptr_ = MySTL::make_shared<T>(t);
			return cp_->ptr_.operator->();
		}
		template <typename T>
		proxy<T>::operator T() const { return *(cp_->ptr_); }
	}
	template <typename T>
	cow_ptr<T>::cow_ptr(T *p=nullptr) :ptr_(p){}
	template <typename T>
	template <typename D>
	cow_ptr<T>::cow_ptr(T* p,D d):ptr_(p,d){}
	template <typename T>
	cow_ptr<T>::cow_ptr(const cow_ptr<T>& rhs)
		:ptr_(rhs.ptr_)	{}
	template <typename T>
	cow_ptr<T>& cow_ptr<T>::operator=(const cow_ptr<T>& rhs)
	{
		if (this != &rhs)
		{
			ptr_.decrease_ref();
			ptr_ = rhs.ptr_;
		}
		return *this;
	}
	template <typename T>
	const typename cow_ptr<T>::element_type* cow_ptr<T>::get() const
	{	return ptr_.get();	}
	template <typename T>
	typename cow_ptr<T>::element_type* cow_ptr<T>::get()
	{	return ptr_.get();	}
	template <typename T>
	cow_ptr<T>::operator bool() const
	{	return ptr != nullptr;	}
	template <typename T>
	const typename cow_ptr<T>::proxy<T> cow_ptr<T>:: operator*() const
	{
		return proxy<T>(const_cast<cow_ptr* const>(this));
	}
	template <typename T>
	typename cow_ptr<T>::proxy<T> cow_ptr<T>::operator*()
	{
		return proxy<T>(this);
	}
	template <typename T>
	const typename cow_ptr<T>::proxy<T> cow_ptr<T>::operator->() const
	{
		return proxy<T>(const_cast<cow_ptr* const>(this));
	}
	template <typename T>
	typename cow_ptr<T>::proxy<T> cow_ptr<T>::operator->()
	{
		return proxy<T>(this);
	}
	template <typename T1,typename T2>
	bool operator==(const cow_ptr<T1>& lhs,
		const cow_ptr<T2>& rhs) const
	{	return lhs.ptr_ == rhs.ptr_; }
	template <typename T>
	bool operator==(const cow_ptr<T>& lhs,
		nullptr_t p) const
	{	return lhs.ptr_ == p;	}
	template <typename T>
	bool operator==(nullptr_t p,
		const cow_ptr<T>& rhs) const
	{	return rhs == p;	}

	template <typename T1, typename T2>
	bool operator!=(const cow_ptr<T1>& lhs,
		const cow_ptr<T2>& rhs) const
	{
		return !(lhs.ptr_ == rhs.ptr_);
	}
	template <typename T>
	bool operator!=(const cow_ptr<T>& lhs,
		nullptr_t p) const
	{
		return lhs.ptr_ != p;
	}
	template <typename T>
	bool operator!=(nullptr_t p,
		const cow_ptr<T>& rhs) const
	{
		return rhs != p;
	}

}

#endif // !_COWPTR_IMPL_H