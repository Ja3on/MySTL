#pragma once
#ifndef _CONSTRUCT_H
#define _CONSTRUCT_H

#include "../MySTL/TypeTraits.h"
#include <new>

namespace MySTL
{
	template<typename T1,typename T2>
	inline void construct(T1* ptr1, const T2& value)
	{
		new(ptr1) T1(value);//¶¨Î»new
	}
	
	template <typename T>
	inline void destroy(T* ptr)
	{
		ptr->~T();
	}

	template <typename ForwardIterator>
	inline void _destroy(ForwardIterator first,
		ForwardIterator last,_true_type){}

	template <typename ForwardIterator>
	inline void _destroy(ForwardIterator first,
		ForwardIterator last, _false_type)
	{
		for (; first != last; ++first)
			destroy(&*first);
	}
	template <typename ForwardIterator>
	inline void destroy(ForwardIterator first,
		ForwardIterator last)
	{
		typedef typename _type_traits<ForwardIterator>
			::is_POD_type is_POD_type;
		_destroy(first, last, is_POD_type);
	}
}

#endif // !_CONSTRUCT_H