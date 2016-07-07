#pragma once
#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include "../MySTL/Allocator.h"
#include "../MySTL/Iterator.h"
#include "../MySTL/TypeTraits.h"
#include "../MySTL/Construct.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>

namespace MySTL
{
	template <typename Value, typename Key, typename HashFcn, typename ExtractKey,
		typename EqualKey, typename Alloc>
		class hashtable;
	namespace Detail
	{
		template <typename Value>
		struct __hashtable_node
		{
			__hashtable_node* next;
			Value val;
		};

		template <typename Value,typename Key,typename HashFcn,
		typename ExtractKey,typename EqualKey,typename Alloc>
			struct __hashtable_iterator
		{
			typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
				hashtable;
			typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey,
				Alloc> iterator;

			typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey,
				EqualKey, Alloc> const_iterator;
			typedef __hashtable_node<Value> node;

			typedef forward_iterator_tag iterator_category;

			typedef Value value_type;
			typedef ptrdiff_t difference_type;
			typedef size_t size_type;
			typedef Value& reference;
			typedef Value* pointer;

			node* cur;
			hashtable* ht;

			__hashtable_iterator(node* n,hashtable* tab):cur(n),ht(tab){}
			__hashtable_iterator() {}

			reference operator*() const { return cur->val; }
			pointer operator->() const { return &(operator*()); }
			iterator& operator++();
			iterator operator++(int);
			bool operator==(const iterator& it) const { return cur == it->cur; }
			bool operator!=(const iterator& it) const { return !(*this == it); }


		};
	}

	template <typename Value, typename Key, typename HashFcn, 
		typename ExtractKey,typename EqualKey, typename Alloc>
		class hashtable
	{
	public:
		typedef HashFcn hasher;
		typedef EqualKey key_equal;
		typedef size_t size_type;
		typedef Value value_type;
		typedef Key key_type;
		typedef Detail::__hashtable_iterator<Value, Key, HashFcn,
			ExtractKey, EqualKey, Alloc>::iterator iterator;

	private:
		hasher hash;
		key_equal equals;
		ExtractKey get_key;

		typedef Detail::__hashtable_node<Value> node;
		typedef MySTL::allocator<node> node_allocator;

		std::vector<node*, Alloc> buckets;
		size_type num_elements;




	private:
		static const int __stl_num_primes = 28;
		static const unsigned long __stl_prime_list[__st1_num_primes] =
		{
			53,		 97,	   193,		 389,	   769,
			1543,    3079,     6151,	 12289,	   24593,
			49157,   98317,    196613,   393241,   786433,
			1572869, 3145739,  6291469,  12582917, 25165843,
			50331653,100663319,201326611,402453189,805306457,
			1610612741,3221225473ul,4294967291ul
		};
		inline unsigned long __stl_next_prime(unsigned long n)
		{
			const unsigned long* first = __stl_prime_list;
			const unsigned long* last = __stl_prime_list + __stl_num_primes;
			const unsigned long* pos = std::lower_bound(first, last, n);
			return pos == last ? *(last - 1) : *pos;
		}
	public:

		size_type max_bucket_count() const
		{
			return __stl_prime_list[__stl_num_primes - 1];
		}

		//构造新节点
		node* new_node(const value_type& obj)
		{
			node* n = node_allocator::allocate();
			n->next = 0;
			construct(&n->val, obj);
		}
		//
		void delete_node(node* n)
		{
			destroy(&n->val);
			node_allocator::deallocate(n);
		}

		size_type bucket_count() const { return buckets.size(); }

		size_type next_size(size_type n) const { return __stl_next_prime(n); }

		void initialize_buckets(size_type n)
		{
			const size_type n_buckets = next_size(n);
			buckets.reserve(n_buckets);
			buckets.insert(buckets.end(), n_buckets, (node*)0);
			num_elements = 0;
		}


		void resize(size_type);
	public:
		std::pair<iterator, bool> insert_unique(const value_type& obj)
		{
			resize(num_elements + 1);//判断是否需要重建表格
			return insert_unique_noresize(obj);
		}
		std::pair<iterator, bool> insert_unique_noresize(const value_type&);
		size_type bkt_num(const value_type&, size_type);

	};



}

#include "../MySTL/Detail/HashTable.impl.h"
#endif // !_HASHTABLE_H