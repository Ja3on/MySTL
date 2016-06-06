#pragma once
#ifndef _DEQUE_H
#define _DEQUE_H

#include "../MySTL/Allocator.h"
#include "../MySTL/ReverseIterator.h"
#include "../MySTL/Utility.h"
#include "../MySTL/Iterator.h"

namespace MySTL
{
	template <typename T,
		typename Alloc = allocator<T>>
		class deque;
	namespace Detail
	{
		template <typename T>//deque迭代器
		class dq_iter :public 
			MySTL::iterator<MySTL::bidirectional_iterator_tag, T>
		{
		private:
			template <typename T,typename Alloc>
			friend class MySTL::deque;

		public:
			dq_iter():mapIndex_(-1),cur_(nullptr),container_(0){}
			dq_iter(size_t index,T* ptr,cntrPtr container)
				:mapIndex_(index),cur_(ptr),container_(container){}
			dq_iter(const dq_iter& rhs)
				:mapIndex_(rhs.mapIndex_),cur_(rhs.cur_),
				container_(rhs.container_){}
			dq_iter& operator=(const dq_iter& rhs);
			void swap(dq_iter&);
			reference operator*() { return *cur_; }
			const reference operator*() const { return *cur_; }
			pointer operator->() { return &(operator*()); }
			const pointer operator->() const { return &(operator*()); }
			dq_iter& operator++();
			dq_iter operator++(int);
			dq_iter& operator--();
			dq_iter operator--(int);
			bool operator==(const dq_iter&) const;
			bool operator!=(const dq_iter&) const;
		private:
			
			T* getBuckTail(size_t mapIndex) const;//缓冲区尾指针
			T* getBuckHead(size_t mapIndex) const;//缓冲区首指针
			size_t getBuckSize() const;//缓冲区大小

		private:
			typedef const ::MySTL::deque<T>* cntrPtr;
			size_t mapIndex_;//节点下标
			T* cur_;//当前元素指针
			cntrPtr container_;//当前的deque
		public:
			template <typename T>
			friend dq_iter<T> operator+(const dq_iter<T>&,
				typename dq_iter<T>::difference_type);
			template <typename T>
			friend dq_iter<T> operator+(typename dq_iter<T>::difference_type,
				const dq_iter<T>&);
			template <typename T>
			friend dq_iter<T> operator-(const dq_iter<T>&,
				typename dq_iter<T>::difference_type);
			template <typename T>
			friend dq_iter<T> operator-(typename dq_iter<T>::differenc_type,
				const dq_iter<T>&);
			template <typename T>
			friend typename dq_iter<T>::difference_type operator-(
				const dq_iter<T>&, const dq_iter<T>&);
			template <typename T>
			friend void swap(dq_iter<T>&, dq_iter<T>&);
		};
	}

	template <typename T,typename Alloc>
	class deque
	{
	private:
		template <typename T_>
		friend class ::MySTL::Detail::dq_iter;
	public:
		typedef T value_type;
		typedef Detail::dq_iter<T> iterator;
		typedef Detail::dq_iter<const T> const_iterator;
		typedef T& reference;
		typedef const reference const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Alloc allocator_type;
		typedef value_type* pointer;
		typedef pointer* map_pointer;
	private:
		typedef Alloc dataAllocator;
		enum class EBuckSize { BUCKSIZE = 64 };//限定作用域的枚举类型

	public:
		deque();
		explicit deque(size_type n, const value_type& val = value_type());
		template <typename InputIterator>
		deque(InputIterator first, InputIterator last);
		deque(const deque&);
		~deque();
		deque& operator=(const deque&);
		deque& operator=(deque&&);

		iterator begin();
		iterator end();
		iterator begin() const;
		iterator end() const;

	public:
		const size_t size() const { return end() - begin(); }
		bool empty() const { return begin() == end(); }

		reference operator[](size_type n);
		reference front();
		reference back();
		const_reference front() const;
		const_reference back() const;
		const_reference operator[](size_type n) const;

		void push_back(const value_type&);
		void push_front(const value_type&);
		void pop_back();
		void pop_front();
		void swap(deque&);
		void clear();

	private:
		T* getANewBuck();//获取新的缓冲区,并返回首指针
		T** getANewMap(const size_t& size);//
		size_t getNewMapSize(const size_t size);//
		size_t getBuckSize() const;
		void init();
		bool back_full() const;
		bool front_full() const;
		void deque_aux(size_t n, const value_type& val, std::true_type);
		template <typename Iterator>
		void deque_aux(Iterator first, Iterator last, std::false_type);
		void reallocateAndCopy();
	public:
		template <typename T,typename Alloc>
		friend bool operator==(const deque<T, Alloc>&,
			const deque<T, Alloc>&) const;
		template <typename T,typename Alloc>
		friend bool operator!=(const deque<T, Alloc>&,
			const deque<T, Alloc>&) const;
		template <typename T,typename Alloc>
		friend void swap(deque<T, Alloc>&, deque<T, Alloc>&);
	private:
		iterator beg_, end_;//
		size_t mapSize_;//map内可容纳的指针数
		map_pointer map_;//map是连续空间,其内的每个元素都是一个指针(节点)
						 //指向一个缓冲区
	};
}
#include "Detail\Deque.impl.h"
#endif // !_DEQUE_H