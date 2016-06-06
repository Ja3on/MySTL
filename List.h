#pragma once
#ifndef _LIST_H
#define _LIST_H

#include "../MySTL/Algorithm.h"
#include "../MySTL/Allocator.h"
#include "../MySTL/Iterator.h"
#include "../MySTL/ReverseIterator.h"
#include "../MySTL/UninitializedFunctions.h"

#include <type_traits>

namespace MySTL
{
	template <typename T>
	class list;
	namespace Detail 
	{
		template<typename T>
		struct node
		{
			T data;
			node* next;
			node* prev;
			list<T>* container;
			node(const T& d,node* n,node* p,list<T>* c)
				:data(d),next(n),prev(p),container(c){}
			bool operator==(const node& rhs)
			{
				return (data == rhs.data &&
					prev == rhs.prev && next == rhs.next &&
					container == rhs.container);
			}
		};
		template <typename T>
		struct listIterator :public iterator<bidirectional_iterator_tag,T>
		{
			template <typename T>
			friend class list;
			typedef node<T>* nodePtr;

			nodePtr p;

			listIterator(nodePtr np = nullptr):p(np){}

			listIterator& operator++();
			listIterator operator++(int);
			listIterator& operator--();
			listIterator operator--(int);

			T& operator*() { return p->data; }
			T* operator->() { return &(operator*()); }

			template <typename T>
			friend bool operator==(const listIterator&, 
				const listIterator&) const;
			template <typename T>
			friend bool operator!=(const listIterator&,
				const listIterator&) const;
		};
	}
	template <typename T>
	class list
	{
	public:
		template <typename T>
		friend struct listIterator;
	private:
		typedef MySTL::allocator<Detail::node<T>> nodeAllocator;
		typedef Detail::node<T>* nodePtr;
	public:
		typedef T value_type;
		typedef Detail::listIterator<T> iterator;
		typedef T* pointer;
		typedef T& reference;
		typedef Detail::listIterator<const T> const_iterator;
		typedef MySTL::reverse_iterator_t<iterator> reverse_iterator;
		typedef size_t size_type;
	private:
		iterator head;
		iterator tail;
	public:
		//***********************************************************
		list();
		explicit list(const size_type& n, const value_type&);
		template <typename InputIterator>
		list(InputIterator first, InputIterator last);
		//***********************************************************
		list(const list&);
		list& operator=(const list&);
		~list();
		//***********************************************************
		bool empty() const { return isEmpty(); }
		bool isEmpty() const;
		size_type size() const;
		reference front() { return (head.p->data); }
		reference back() { return (tail.p->prev->data); }
		//***********************************************************
		void push_front(const value_type&);
		void push_back(const value_type&);
		void pop_front();
		void pop_back();
		//***********************************************************
		iterator begin();
		iterator end();
		const_iterator cbegin() const;
		const_iterator cend() const;
		reverse_iterator rbegin();
		reverse_iterator rend();
		//***********************************************************
		iterator insert(iterator, const value_type&);
		void insert(iterator, const size_type&, const value_type&);
		template <typename InputIterator>
		void insert(iterator, InputIterator, InputIterator);
		iterator erase(iterator);
		iterator erase(iterator, iterator);
		void swap(list&);
		void clear();
		void splice(iterator, list&);
		void splice(iterator, list&, iterator);
		void splice(iterator, list&, iterator, iterator);
		void remove(const value_type&);
		template<typename Predicate>
		void remove_if(Predicate);
		void unique();
		template <typename BinaryPredicate>
		void unique(BinaryPredicate);
		void merge(list&);
		template <typename Compare>
		void merge(list&, Compare);
		void sort();
		template <typename Compare>
		void sort(Compare);
		void reverse();
		//***********************************************************
	private:
		void ctorAux(const size_type& n,const value_type&,std::true_type);
		template <typename InputIterator>
		void ctorAux(InputIterator, InputIterator, std::false_type);
		nodePtr newNode(const T& val = T());
		void deleteNode(nodePtr p);
		void insert_aux(iterator, const size_type&, const value_type&,
			std::true_type);
		template <typename InputIterator>
		void insert_aux(iterator, InputIterator, InputIterator,
			std::false_type);
		const_iterator changeIteratorToConstIterator(iterator&) const;

	public:
		template <typename T_>
		friend void swap(list<T_>&, list<T_>&);
		template <typename T_>
		friend bool operator==(const list<T_>&, const list<T_>&);
		template <typename T_>
		friend bool operator!=(const list<T_>&, const list<T_>&);
 	};
}
#include "../MySTL/Detail/List.impl.h"
#endif // !_LIST_H