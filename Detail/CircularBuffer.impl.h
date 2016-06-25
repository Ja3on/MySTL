#pragma once
#ifndef _CIRCULARBUFFER_IMPL_H
#define _CIRCULARBUFFER_IMPL_H

namespace MySTL
{
	namespace Detail
	{
		template <typename T, size_t N, typename Alloc>
		int cb_iter<T, N, Alloc>::prevIndex(int index)
		{
			--index;
			index = (index == -1 ? index + N : index);
			return index;
		}
		template <typename T, size_t N, typename Alloc>
		bool cb_iter<T, N, Alloc>::operator==(const cb_iter& it) const
		{
			return (_container == it._container &&
				_ptr == it._ptr&&_index == it._index);
		}
		template <typename T, size_t N, typename Alloc>
		bool cb_iter<T, N, Alloc>::operator!=(const cb_iter& rhs) const
		{
			return !(*this == rhs);
		}

		template <typename T, size_t N, typename Alloc>
		cb_iter<T, N, Alloc>& cb_iter<T, N, Alloc>::operator++()
		{
			_setIndex(nextIndex(_index));
			_setPtr(_container->__start + _index);
			return *this;
		}

		template <typename T, size_t N, typename Alloc>
		cb_iter<T, N, Alloc> cb_iter<T, N, Alloc>::operator++(int)
		{
			auto temp = *this;
			++(*this);
			return temp;
		}

		template <typename T, size_t N, typename Alloc>
		cb_iter<T, N, Alloc>& cb_iter<T, N, Alloc>::operator--()
		{
			_setIndex(prevIndex(_index));
			_setPtr(_container->__start + _index);
			return *this;
		}

		template <typename T, size_t N, typename Alloc>
		cb_iter<T, N, Alloc> cb_iter<T, N, Alloc>::operator--(int)
		{
			auto temp = *this;
			--(*this);
			return temp;
		}

		template <typename T, size_t N, typename Alloc>
		cb_iter<T, N, Alloc>& cb_iter<T, N, Alloc>::
			operator=(const cb_iter& rhs)
		{
			if (this != &rhs)
			{
				_ptr = rhs._ptr;
				_container = rhs._container;
				_index = rhs._index;
			}
			return *this;
		}

		template <typename T, size_t N, typename Alloc>
		cb_iter<T, N, Alloc>& operator+(const cb_iter<T, N, Alloc>& cit,
			std::ptrdiff_t i)
		{
			int real_i = i % (std::ptrdiff_t)N;//i>=0
			if (real_i < 0) real_i += 5;
			cb_iter<T, N, Alloc> res = cit;
			res._setIndex(res._index + real_i);
			res._setPtr(res._ptr + res._index);
			return res;
		}
		template <typename T, size_t N, typename Alloc>
		cb_iter<T, N, Alloc>& operator-(const cb_iter<T, N, Alloc>& cit,
			std::ptrdiff_t i)
		{
			cb_iter<T, N, Alloc> res = cit;
			return (res + (-i));
		}

	}
	//****************************************************************
	template <typename T, size_t N, typename Alloc>
	circular_buffer<T, N, Alloc>::~circular_buffer()
	{
		clear();
		dataAllocator::deallocate(__start, __size);
	}

	template <typename T, size_t N, typename Alloc>
	circular_buffer<T, N, Alloc>::circular_buffer(const int& n,
		const value_type& val = value_type())
	{
		assert(n != 0);
		allocateAndFillN(n, val);
	}
	template <typename T, size_t N, typename Alloc>
	template <typename InputIterator>
	circular_buffer<T, N, Alloc>::circular_buffer(InputIterator first,
		InputIterator last)
	{
		assert(first != last);
		allocteAndCopy(first, last);
	}
	template <typename T, size_t N, typename Alloc>
	circular_buffer<T, N, Alloc>::
		circular_buffer(const circular_buffer& rhs)
	{ clone(rhs); }
	template <typename T, size_t N, typename Alloc>
	circular_buffer<T, N, Alloc>& circular_buffer<T, N, Alloc>::
		operator=(const circular_buffer<T, N, Alloc>& rhs)
	{
		if (*this != rhs)	clone(rhs);
		return *this;
	}
	template <typename T, size_t N, typename Alloc>
	circular_buffer<T, N, Alloc>& circular_buffer<T, N, Alloc>::
		operator=(circular_buffer<T, N, Alloc>&& rhs)
	{
		if (*this != rhs)
		{
			copyAllMembers(rhs);
			zeroCircular(rhs);
		}
		return *this;
	}
	//****************************************************************
	//****************************************************************
	template <typename T, size_t N, typename Alloc>
	void circular_buffer<T, N, Alloc>::push_back(const value_type& val)
	{
		if (full())
		{
			indexOfTail = nextIndex(indexOfTail);
			dataAllocator::construct(__start + indexOfTail, val);
			indexOfHead = nextIndex(indexOfHead);
		}
		else
		{
			indexOfTail = nextIndex(indexOfTail);
			dataAllocator::construct(__start + indexOfTail, val);
			++__size;
		}
	}
	template <typename T,size_t N,typename Alloc>
	void circular_buffer<T, N, Alloc>::pop_front()
	{
		if (empty()) throw;
		dataAllocator::destroy(__start + indexOfHead);
		indexOfHead = nextIndex(indexOfHead);
		--__size;
	}
	template <typename T,size_t N,typename Alloc>
	std::ostream& operator<<(std::ostream& os,
		circular_buffer<T, N, Alloc>& rhs)
	{
		typename circular_buffer<T, N, Alloc>::size_type size = rhs.size();
		if (!rhs.empty())
			os << "(";
		for (auto it = rhs.first(); it != rhs.last() && size != 0; ++it, --size)
			os << *it << ", ";
		os << *rhs.last() << ")";
		return os;
	}
	template <typename T, size_t N, typename Alloc>
	void circular_buffer<T, N, Alloc>::clear()
	{
		for (; !empty(); indexOfHead = nextIndex(indexOfHead), --__size)
			dataAllocator::destroy(__start++indexOfHead);
		indexOfHead = indexOfTail = nullptr;
	}
	template <typename T, size_t N, typename Alloc>
	bool circular_buffer<T, N, Alloc>::operator==(const circular_buffer& rhs)
	{
		auto it1 = first(), it2 = rhs.first();
		for (; it1 != last() && it2 != rhs.last(); ++it1, ++it2)
			if (*it1 != *it2) return false;
		return (it1 == last() && it2 == rhs.last() &&
			*last() == *rhs.last());
	}
	template <typename T, size_t N, typename Alloc>
	bool circular_buffer<T, N, Alloc>::operator!=(const circular_buffer& rhs)
	{ return !(*this != rhs);	}
	template <typename T, size_t N, typename Alloc>
	void circular_buffer<T, N, Alloc>::allocateAndFillN(const size_type& n,
		const value_type& val)
	{
		__start = dataAllocator::allocate(N);
		__finish = __start + N;
		indexOfHead = nullptr;
		if (N <= n)
		{
			__finish = MySTL::uninitialized_fill_n(__start, N, val);
			indexOfTail = __start + N;
			__size = N;
		}
		else
		{
			__finish = MySTL::uninitialized_fill_n(__start, n, val);
			__finish = MySTL::uninitialized_fill_n(__finish, N - n, 
				value_type());
			indexOfTail = n - 1;
			__size = n;
		}
	}
	template <typename T, size_t N, typename Alloc>
	template <typename InputIterator>
	void circular_buffer<T, N, Alloc>::allocteAndCopy(InputIterator first,
		InputIterator last)
	{
		int n = last - first;
		__start = dataAllocator::allocate(N);
		indexOfHead = nullptr;
		if (N <= n)
		{
			__finish = MySTL::unintialized_copy(first, last, __start);
			indexOfTail = __start + N;
			__size = N;
		}
		else
		{
			__finish = MySTL::unintialized_copy(first, last, __start);
			__finish = MySTL::uninitialized_fill_n(__finish, N - n, 
				value_type());
			indexOfTail = n - 1;
			__size = n;
		}
	}
	template <typename T, size_t N, typename Alloc>
	void circular_buffer<T, N, Alloc>::copyAllMembers(const circular_buffer& rhs)
	{
		__start = rhs.__start;
		__finish = rhs.__finish;
		indexOfHead = rhs.indexOfHead;
		indexOfTail = rhs.indexOfTail;
		__size = rhs.__size;
	}
	template <typename T, size_t N, typename Alloc>
	void circular_buffer<T, N, Alloc>::zeroCircular(circular_buffer& rhs)
	{
		rhs.__start = rhs.__finish = nullptr;
		rhs.indexOfHead = rhs.indexOfTail = 0;
		rhs.__size = 0;
	}
	template <typename T, size_t N, typename Alloc>
	void circular_buffer<T, N, Alloc>::clone(const circular_buffer& rhs)
	{
		__start = dataAllocator::allocate(N);
		__finish = __start + N;
		__size = N;
		indexOfHead = rhs.indexOfHead;
		indexOfTail = rhs.indexOfTail;
		MySTL::unintialized_copy(rhs.__start, rhs.__finish, __start);
	}


}

#endif // !_CIRCULARBUFFER_H