#pragma once
#ifndef _CIRCULARBUFFER_H
#define _CIRCULARBUFFER_H

#include "../MySTL/Allocator.h"
#include "../MySTL/Iterator.h"
#include "../MySTL/UninitializedFunctions.h"

#include <cassert>

namespace MySTL
{
	template <typename T, size_t N, typename Alloc>
	class circular_buffer;
	namespace Detail
	{
		template <typename T, size_t N, typename Alloc = MySTL::allocator<T>>
		class cb_iter :public MySTL::iterator<bidirectional_iterator_tag, T>
		{
		private:
			typedef ::MySTL::circular_buffer<T, N, Alloc> cb;
			typedef cb* cbPtr;
		public:
			cb_iter() :_ptr(nullptr), _index(0), _container(nullptr) {}
			cb_iter(T* ptr, cbPtr container)
				:_ptr(ptr), _index(ptr - container->__start),
				_container(container) {}
			cb_iter(const cb_iter& rhs) :_ptr(rhs._ptr), _index(rhs._index),
				_container(rhs._container) {}
			cb_iter& operator=(const cb_iter&);
		public:
			operator T*() { return _ptr; }//类型转换操作符
			T& operator*() { return *_ptr; }
			T* operator->() { return &(operator*()); }

			cb_iter& operator++();
			cb_iter operator++(int);
			cb_iter& operator--();
			cb_iter operator--(int);

			bool operator==(const cb_iter&) const;
			bool operator!=(const cb_iter&) const;

		private:
			void _setIndex(int index) { _index = index; }
			void _setPtr(T* ptr) { _ptr = ptr; }
			int nextIndex(int index) { return (++index) % N; }
			int prevIndex(int);

		public:
			template <typename T, size_t N, typename Alloc>
			friend cb_iter<T, N, Alloc> operator+(const cb_iter<T, N, Alloc>& cit,
				std::ptrdiff_t i);
			template <typename T, size_t N, typename Alloc>
			friend cb_iter<T, N, Alloc> operator-(const cb_iter<T, N, Alloc>&,
				std::ptrdiff_t);


		private:
			T* _ptr;
			int _index;
			cbPtr _container;
		};
	}
	template <typename T, size_t N, typename Alloc = MySTL::allocator<T>>
	class circular_buffer
	{
		template <typename T, size_t N, typename Alloc>
		friend class ::MySTL::Detail::cb_iter;

	public:
		typedef T value_type;
		typedef Detail::cb_iter<T, N, Alloc> iterator;
		typedef iterator pointer;
		typedef T& reference;
		typedef int size_type;//
		typedef ptrdiff_t difference_type;

	public:
		typedef Alloc dataAllocator;

	public:
		explicit circular_buffer(const int& n,
			const value_type& val = value_type());
		template <typename InputIterator>
		circular_buffer(InputIterator, InputIterator);
		circular_buffer(const circular_buffer&);
		circular_buffer& operator=(const circular_buffer&);
		circular_buffer& operator==(circular_buffer&&);//move 构造函数
		~circular_buffer();

		bool full() { return __size == N; }
		bool empty() { return __size == 0; }
		difference_type capacity() { return __finish - __start; }
		size_type size() const { return __size; }
		void clear();

		iterator first() { return iterator(__start + indexOfHead, this); }
		iterator last() { return iterator(__start + indexOfTail, this); }

		reference operator[](size_type i) { return *(__start + i); }
		reference front() { return *(__start + indexOfHead); }
		reference back() { return *(__start + indexOfTail); }
		void push_back(const T&);
		void pop_front();

		bool operator==(const circular_buffer&);
		bool operator!=(const circular_buffer&);

		Alloc get_allocator() { return dataAllocator; }

	private:
		void allocateAndFillN(const int&, const value_type&);
		template <typename InputIterator>
		void allocteAndCopy(InputIterator, InputIterator);
		int nextIndex(int index) { return (index + 1) % N; }
		void copyAllMembers(const circular_buffer&);
		void zeroCircular(circular_buffer&);
		void clone(const circular_buffer&);

	public:
		template <typename T,size_t N,typename Alloc>
		friend std::ostream& operator<<(std::ostream&,
			const circular_buffer<T, N, Alloc>&);

	private:
		T* __start;
		T* __finish;
		int indexOfHead;//the first position
		int indexOfTail;//the last position
		size_type __size;


	};
}

#include "../MySTL/Detail/CircularBuffer.impl.h"
#endif // !_CIRCULARBUFFER_H