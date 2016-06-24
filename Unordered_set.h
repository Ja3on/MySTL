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
			typedef Unordered_set<Key, Hash, KeyEqual, Allocator>* cntrPtr;
			size_t _bucket_index;
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
		MySTL::vector<MySTL::list<key_type>> __bucket;
		size_type __size;
		float __max_load_factor;

#define PRIME_LIST_SIZE 28
		static size_t prime_list_[PRIME_LIST_SIZE];
	public:
		explicit Unordered_set(size_t bucket_count);
		template <typename InputIterator>
		Unordered_set(InputIterator first, InputIterator last);
		Unordered_set(const Unordered_set&);
		Unordered_set& operator =(const Unordered_set&);

		size_type size() const;
		bool empty() const;
		size_type bucket_count() const;
		size_type bucket_size(size_type) const;
		size_type bucket(const key_type&) const;
		//size_type bucket_index(const key_type& key) const;
		float load_factor() const;
		float max_load_factor() const;
		void max_load_factor(float);
		void rehash(size_type);

		iterator begin();
		iterator end();
		local_iterator begin(size_type i);
		local_iterator end(size_type);

		iterator find(const key_type&);
		size_type count(const key_type&);

		MySTL::pair<iterator, bool> insert(const value_type&);
		template <typename InputIterator>
		void insert(InputIterator first, InputIterator last);
		iterator erase(iterator position);
		size_type erase(const key_type& key);

		haser hash_function() const;
		key_equal key_eq() const;
		allocator_type get_allocator() const;
 
	private:
		size_type __next_prime(size_type n) const;
		size_type __bucket_index(const key_type& key) const;
		bool has_key(const key_type&);
	public:
		template <typename Key, typename Hash, typename KeyEqual,
			typename Allocator>
			friend void swap(Unordered_set<Key, Hash, KeyEqual, Allocator>& lhs,
				Unordered_set<Key, Hash, KeyEqual, Allocator>& rhs);
	};
}

#include "../MySTL/Detail/Unordered_set.impl.h"
#endif // !_UNORDERED_SET_H