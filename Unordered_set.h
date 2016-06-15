#pragma once
#ifndef _UNORDERED_SET_H
#define _UNORDERED_SET_H

#include "../MySTL/Algorithm.h"
#include "../MySTL/Allocator.h"
#include "../MySTL/Iterator.h"
#include "../MySTL/Memory.h"
#include "../MySTL/Functional.h"
#include "../MySTL/List.h"
#include "../MySTL/Vector.h"

namespace MySTL
{
	template <typename Key, typename Hash,
		typename KeyEqual, typename Allocator>
		class Unordered_set;
	namespace Detail
	{
		template <typename Key, typename ListIterator,
			typename Hash = std::hash<Key>,
			typename KeyEqual = MySTL::equal_to<Key>,
			typename Allocator = MySTL::allocator<Key>>
			class ust_iterator :public MySTL::iterator<
			MySTL::forward_iterator_tag, Key>
		{
		private:
			template <typename Key, typename Hash, typename KeyEqual,
				typename Allocator>
				friend class Unordered_set;
		private:
			typedef usi_iterator<Key, Hash, KeyEqual, Allocator>* cntrPtr;
			size_t _buket_index;
			ListIterator _iterator;
			cntrPtr _container;
		public:
			ust_iterator(size_t index, ListIterator it, cntrPtr ptr);
			ust_iterator& operator++();
			ust_iterator operator++(int);
			Key& operator*() { return *_iterator; }
			Key* operator->() { return &(operator*()); }
		private:
			template <typename Key, typename ListIterator, typename Hash,
				typename KeyEqual, typename Allocator>
				friend bool operator==(
					const ust_iterator<Key, ListIterator, Hash, KeyEqual, Allocator>&,
					const ust_iterator<Key, ListIterator, Hash, KeyEqual, Allocator>&);

			template <typename Key, typename ListIterator, typename Hash,
				typename KeyEqual, typename Allocator>
				friend bool operator!=(
					const ust_iterator<Key, ListIterator, Hash, KeyEqual, Allocator>&,
					const ust_iterator<Key, ListIterator, Hash, KeyEqual, Allocator>&);

		};
	}
	template <typename Key, typename Hash = std::hash<Key>,
		typename KeyEqual = MySTL::equal_to<Key>,
		typename Allocator = MySTL::allocator<Key>>
		class Unordered_set
	{
	private:
		template <typename Key, typename ListIterator, typename Hash,
			typename KeyEqual, typename Allocator>
			friend class Detail:: ust_iterator;
	public:
		typedef Key key_type;
		typedef Key value_type;
		typedef size_t size_type;
		typedef Hash haser;
		typedef KeyEqual key_equal;
		typedef Allocator allocator_type;
		typedef value_type& reference;
		typedef typename MySTL::list<key_type>::iterator local_iterator;
		typedef Detail::ust_iterator<Key, typename MySTL::list<key_type>::
			iterator, Hash, KeyEqual, Allocator> iterator;
	private:
		MySTL::vector<MySTL::list<key_type>> __buket;
		size_type __size;
		float __max_load_factor;
	};
}

#endif // !_UNORDERED_SET_H