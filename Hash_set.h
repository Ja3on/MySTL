#pragma once
#ifndef _HASH_SET_H
#define _HASH_SET_H

#include "../MySTL/HashTable.h"
#include "../MySTL/Allocator.h"
#include <algorithm>
#include <hash_map>


namespace MySTL
{
	template <typename Value,
		typename HashFcn = std::hash<Value>,
		typename EqualKey = std::equal_to<Value>,
		typename Alloc = MySTL::alloc>

		class hash_set
	{
	private:
		typedef hashtable<Value, Value, HashFcn, std::identity<Value>,
			EqualKey, Alloc> ht;
		ht rep;//
	public:
		typedef typename ht::key_type key_type;
		typedef typename ht::value_type value_type;
		typedef typename ht::hasher hasher;
		typedef typename ht::key_equal key_equal;

		typedef typename ht::size_type size_type;
		typedef typename ht::difference_type difference_type;
		typedef typename ht::const_pointer const_pointer;
		typedef typename ht::const_pointer pointer;
		typedef typename ht::const_reference reference;
		typedef typename ht::const_reference const_reference;
		typedef typename ht::const_iterator iterator;
		typedef typename ht::const_iterator const_iterator;

		hasher hash_funct() const { return rep.hash_funct(); }
		key_equal key_eq() const { return rep.key_eq(); }

	public:
		hash_set():rep(100,hasher(),key_equal()){}
		explicit hash_set(size_type n)
			:rep(n,hasher(),key_equal()){}
		hash_set(size_type n,const hasher& hf)
			:rep(n,hf,key_equal()){}
		hash_set(size_type n,const hasher& hf,const key_equal& eql)
			:rep(n,hf,eql){}

		template <typename InputIterator>
		hash_set(InputIterator first, InputIterator last)
			: rep(100, hasher(), key_equal()) {	rep.insert_unique(first, last);	}

		template <typename InputIterator>
		hash_set(InputIterator first,InputIterator last,size_type n)
			: rep(n, hasher(), key_equal()) {
			rep.insert_unique(first, last);
		}

		template <typename InputIterator>
		hash_set(InputIterator first,InputIterator last,size_type n,
			hasher hf,key_equal key_eq)
			: rep(n, hf, key_eq) {
			rep.insert_unique(first, last);
		}

	public:
		size_type size() const { return rep.size(); }
		size_type max_size() const { return rep.max_size(); }
		bool empty() const { return rep.empty(); }
		void swap(hash_set& hs) { rep.swap(hs.rep); }

		friend bool operator==(const hash_set& lhs, const hash_set& rhs);

		iterator begin() const { return rep.begin(); }
		iterator end() const { return rep.end(); }

	public:
		std::pair<iterator, bool> insert(const value_type& obj)
		{
			std::pair<iterator, bool> p = rep.insert_unique(obj);
			return std::pair<iterator, bool>(p.first, p.second);
		}
		template <typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			rep.insert_unique(first, last);
		}
		std::pair<iterator, bool> insert_noresize(const value_type& obj)
		{
			std::pair<iterator, bool> p = rep.insert_unique_noresize(obj);
			return std::pair<iterator, bool>(p.first, p.second);
		}
		iterator find(const key_type& key) const {	return rep.find(key);	}
		size_type count(const key_type& key) const { return rep.count(key);	}
		void clear() { rep.clear(); }
	};

}

#endif