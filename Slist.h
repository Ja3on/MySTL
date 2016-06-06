#pragma once
#ifndef _SLIST_H
#define _SLIST_H
#include <utility>
#include "../MySTL/Iterator.h"
#include "../MySTL/Allocator.h"
#include "../MySTL/Construct.h"

namespace MySTL
{
	namespace Detail
	{
		struct __slist_node_base//节点基类
		{
			__slist_node_base *next;
		};
		template <typename T>
		struct __slist_node :public __slist_node_base//slist节点
		{
			T data;
		};
		inline __slist_node_base* __slist_make_link(
			__slist_node_base* prev_node,
			__slist_node_base* new_node)
		{
			new_node->next = prev_node->next;
			prev_node->next = new_node;
			return new_node;
		}
		inline size_t _slist_size(__slist_node_base* node)
		{
			size_t result = 0;
			for (; node != 0; node = node->next) ++result;
			return result;
		}
		//slist的迭代器
		struct __slist_iterator_base
		{
			typedef size_t size_type;
			typedef std::ptrdiff_t difference_type;
			typedef MySTL::forward_iterator_tag iterator_category;//单向迭代器

			__slist_node_base* node;//数据成员

			__slist_iterator_base(__slist_node_base* snb) :node(snb) {}
			void incr() { node = node->next; }//前进一个节点
			bool operator==(const __slist_iterator_base& rhs)
			{
				return node = rhs.node;
			}
			bool operator!=(const __slist_iterator_base& rhs)
			{
				return !(*this == rhs);
			}
		};

		template <typename T, typename Ref, typename Ptr>
		struct __slist_iterator :public __slist_iterator_base
		{
			typedef __slist_iterator<T, T&, T*> iterator;
			typedef __slist_iterator<T, const T&, const T*> const_iterator;
			typedef __slist_iterator<T, Ref, Ptr> self;

			typedef T value_type;
			typedef Ref reference;
			typedef Ptr pointer;
			typedef __slist_node<T> list_node;

			__slist_iterator(list_node* x) :__slist_iterator_base(x) {}
			__slist_iterator() :__slist_iterator_base(0) {}
			self& operator++()
			{
				incr();
				return *this;
			}
			self operator++(int);
			{
				self temp = *this;
				++*this;
				return temp;
			}
			//调用其基类:__slist_iterator_base的operator==和operator!=
		};
	}
	

	template<typename T,typename Alloc=MySTL::alloc>
	class slist
	{
	public:
		typedef T value_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef Detail::__slist_iterator<T, T&, T*> iterator;
		typedef Detail::__slist_iterator<T, const T&, const T*> const_iterator;

	private:
		typedef Detail::__slist_node<T> list_node;
		typedef Detail::__slist_node_base list_node_base;
		typedef Detail::__slist_iterator_base iterator_base;
		typedef MySTL::allocator<list_node> list_node_allocator;
	private:
		static list_node* creat_node(const value_type& val)
		{
			list_node* node = list_node_allocator::allocate();
			construct(&node->data, val);
			node->next = 0;
			return node;
		}
		static void destroy_node(list_node* node)
		{
			list_node_allocator::destroy(&node->data);
			list_node_allocator::deallocate(node);
		}
		size_type __slist_size(list_node_base*) const;
	public:
		slist() { head = 0; }
		~slist() { clear(); }
	public:

		iterator begin() { return iterator((list_node*)head.next); }
		iterator end() { return iterator(0); }
		size_type size() const { return __slist_size(head.next); }
		bool empty() const { return head.next == nullptr; }
		void swap(slist& rhs)
		{
			list_node_base* temp = head.next;
			head.next = rhs.head.next;
			rhs.head.next = temp;
		}
		reference front() { return ((list_node*)head.next)->data; }
		void push_front(const value_type& val)
		{
			Detail::__slist_make_link(&head, creat_node(val));
		}
		void pop_front()
		{
			list_node* node = (list_node*)head.next;
			head.next = node->next;
			destroy_node(node);
		}
		void clear();
	private:
		list_node_base head;
	};

}

#endif // !_SLIST_H