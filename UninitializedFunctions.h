#pragma once
#ifndef _UNINITIALIZED_FUNCTIONS_H
#define _UNINITIALIZED_FUNCTIONS_H

#include "../MySTL/Algorithm.h"
#include "../MySTL/Construct.h"
#include "../MySTL/Iterator.h"
#include "../MySTL/TypeTraits.h"

namespace MySTL
{

	//*************************************************************************

	template <typename InputIterator,typename ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first,
		InputIterator last, ForwardIterator result, _true_type);
	template <typename InputIterator,typename ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first,
		InputIterator last, ForwardIterator result, _false_type);
	template <typename InputIterator,typename ForwardIterator>
	ForwardIterator unintialized_copy(InputIterator first,
		InputIterator last, ForwardIterator result)
	{
		typedef typename MySTL::_type_traits<iterator_traits<InputIterator>
			::value_type>::is_POD_type is_POD_type;
		return _uninitialized_copy_aux(first, last, result, is_POD_type());
	}
	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first,
		InputIterator last, ForwardIterator result, _true_type)
	{
		memcpy(result, first, sizeof(*first)*(last - first));
		return result + (last - first);
	}
	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first,
		InputIterator last, ForwardIterator result, _false_type)
	{
		int i = 0;
		for (; first != last; ++first, ++i)
			construct(result + i, *first);
		return result + i;
	}

	//*************************************************************************

	template <typename ForwardIterator,typename T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& elem, _true_type);
	template <typename ForwardIterator, typename T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& elem, _false_type);
	template <typename ForwardIterator,typename T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last,
		const T& elem)
	{
		typedef typename MySTL::_type_traits<T>::_is_POD_type isPODtype;
		_uninitialized_fill_aux(first, last, elem, isPODtype());
	}

	template <typename ForwardIterator, typename T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& elem, _true_type)
	{	MySTL::fill(first, last, elem);	}

	template <typename ForwardIterator, typename T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& elem, _false_type)
	{
		int i = 0;
		for (; first != last; ++first, ++i)
			construct(first, elem);
	}


	//*************************************************************************

	template <typename ForwardIterator,typename Size,typename T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, 
		Size n,const T& elem,_true_type);

	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, 
		Size n,	const T& elem, _false_type);


	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator uninitialized_fill_n(ForwardIterator first, 
		Size n, const T& elem)
	{
		typedef typename MySTL::_type_traits<T>::_is_POD_type isPODType;
		return _uninitialized_fill_n_aux(first,n, elem, isPODType());
		
	}
	template <typename ForwardIterator, typename Size, typename T>
	void _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& elem, _true_type)
	{
		return MySTL::fill_n(first, n, elem);
	}
	template <typename ForwardIterator, typename Size, typename T>
	void _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& elem, _false_type)
	{
		int i = 0;
		for (; i < n; ++i)
			construct((T*)first+i, elem);
		return (first + i);
	}
}

#endif // !_UNINITIALIZED_FUNCTIONS_H