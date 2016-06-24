#pragma once
#ifndef _UNORDERED_SET_IMPL_H
#define _UNORDERED_SET_IMPL_H
#include <exception>
namespace MySTL
{
	namespace Detail
	{
		template <typename Key, typename ListIterator, typename Hash,
			typename KeyEqual, typename Allocator>
			ust_iterator<Key, ListIterator, Hash, KeyEqual, Allocator>::
			ust_iterator(size_t index, ListIterator it, cntrPtr ptr)
			:_bucket_index(index), _iterator(it), _container(ptr) {}

		template <typename Key, typename ListIterator, typename Hash,
			typename KeyEqual, typename Allocator>
			ust_iterator<Key, ListIterator, Hash, KeyEqual, Allocator>&
			ust_iterator<Key, ListIterator, Hash, KeyEqual, Allocator>::
			operator++()
		{
			++_iterator;
			if (_bucket_index == _container->__bucket[_bucket_index].end())
			{
				for (;;)
				{
					if (_bucket_index == _container->__bucket.size() - 1)
					{
						*this = _container->end();
						break;
					}
					else
					{
						++_bucket_index;
						if (!(_container->__bucket[_bucket_index].empty()))
							_iterator = _container->__bucket[_bucket_index].begin();
						break;
					}
				}
			}
			return *this;
		}

		template <typename Key, typename ListIterator, typename Hash,
			typename KeyEqual, typename Allocator>
			ust_iterator<Key, ListIterator, Hash, KeyEqual, Allocator>
			ust_iterator<Key, ListIterator, Hash, KeyEqual, Allocator>::
			operator++(int)
		{
			auto temp = *this;
			++(*this);
			return temp;
		}

		template <typename Key, typename ListIterator, typename Hash,
			typename KeyEqual, typename Allocator>
			bool operator ==(const ust_iterator<Key, ListIterator, Hash,
				KeyEqual, Allocator>& lhs,
				const ust_iterator<Key, ListIterator, Hash, KeyEqual, Allocator>& rhs)
		{
			return (lhs._bucket_index == rhs._bucket_index &&
				lhs._iterator == rhs._iterator &&
				lhs._container == rhs._container);
		}

		template <typename Key, typename ListIterator, typename Hash,
			typename KeyEqual, typename Allocator>
			bool operator !=(const ust_iterator<Key, ListIterator, Hash,
				KeyEqual, Allocator>& lhs,
				const ust_iterator<Key, ListIterator, Hash, KeyEqual, Allocator>& rhs)
		{
			return !(lhs == rhs);
		}
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
		Unordered_set<Key, Hash, KeyEqual, Allocator>::size() const
	{
		return __size;
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	bool Unordered_set<Key, Hash, KeyEqual, Allocator>::empty() const
	{
		return size() == 0;
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
		Unordered_set<Key, Hash, KeyEqual, Allocator>::bucket_count() const
	{
		return __bucket.size();
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
		Unordered_set<Key, Hash, KeyEqual, Allocator>::
		bucket_size(size_type index) const
	{
		if (index >= bucket_count())
			throw std::_Xout_of_range("out of range");
		return __bucket[index].size();
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
		Unordered_set<Key, Hash, KeyEqual, Allocator>::bucket(const key_type& key) const
	{
		return __bucket_index(key);
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
		Unordered_set<Key, Hash, KeyEqual, Allocator>::
		__bucket_index(const key_type& key) const
	{
		return haser()(key) % __bucket.size();
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	float Unordered_set<Key, Hash, KeyEqual, Allocator>::load_factor() const
	{
		return (float)size() / (float)bucket_count();
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::haser
		Unordered_set<Key, Hash, KeyEqual, Allocator>::hash_function() const
	{
		return haser();
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::key_equal
		Unordered_set<Key, Hash, KeyEqual, Allocator>::key_eq() const
	{
		return key_equal();
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::allocator_type
		Unordered_set<Key, Hash, KeyEqual, Allocator>::get_allocator() const
	{
		return allocator_type();
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	size_t Unordered_set<Key, Hash, KeyEqual, Allocator>::
		prime_list_[PRIME_LIST_SIZE] =
	{
		53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
		786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611,
		402653189, 805306457, 1610612741, 3221225473, 4294967291
	};
	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	bool Unordered_set<Key, Hash, KeyEqual, Allocator>::has_key(const key_type& key)
	{
		auto& list = __bucket[__bucket_index(key)];
		auto pred = std::bind(KeyEqual(), key, std::placeholders::_1);//????
		return MySTL::find_if(list.begin(), list.end(), pred) != list.end();
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
		Unordered_set<Key, Hash, KeyEqual, Allocator>::
		__next_prime(size_type n) const
	{
		auto i = 0;
		for (; i != PRIME_LIST_SIZE; ++i)
		{
			if (n > prime_list_[i])
				continue;
			else break;
		}
		i = (i == PRIME_LIST_SIZE ? PRIME_LIST_SIZE - 1 : i);
		return prime_list_[i];
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	Unordered_set<Key, Hash, KeyEqual, Allocator>::
		Unordered_set(const Unordered_set& rhs)
	{
		__bucket = rhs.__bucket;
		__size = rhs.__size;
		__max_load_factor = rhs.__max_load_factor;
	}
	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	Unordered_set<Key, Hash, KeyEqual, Allocator>&
		Unordered_set<Key, Hash, KeyEqual, Allocator>::
		operator=(const Unordered_set& rhs)
	{
		if (this != &rhs)
		{
			__bucket = rhs.__bucket;
			__size = rhs.__size;
			__max_load_factor = rhs.__max_load_factor;
		}
		return *this;
	}
	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	Unordered_set<Key, Hash, KeyEqual, Allocator>::
		Unordered_set(size_t bucket_count)
	{
		bucket_count = __next_prime(bucket_count);
		__bucket.resize(bucket_count);
		__size = 0;
		__max_load_factor = 1.0;
	}
	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	template <typename InputIterator>
	Unordered_set<Key, Hash, KeyEqual, Allocator>::
		Unordered_set(InputIterator first, InputIterator last)
	{
		__size = 0;
		__max_load_factor = 1.0;
		auto len = last - first;
		__bucket.resize(__next_prime(len));
		for (; first != last; ++first)
		{
			auto index = __bucket_index(*first);
			if (!has_key(*first)) {
				__bucket[index].push_front(*first);
				++__size;
			}
		}
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::iterator
		Unordered_set<Key, Hash, KeyEqual, Allocator>::begin()
	{
		size_type index = 0;
		for (; index != __bucket.size(); ++index)
			if (!__bucket[index].empty()) break;
		if (index == __bucket.size()) return end();
		return iterator(index, __bucket[index].begin(), this);
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::iterator
		Unordered_set<Key, Hash, KeyEqual, Allocator>::end()
	{
		return iterator(__bucket.size() - 1,
			__bucket[__bucket.size() - 1].end(), this);
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::local_iterator
		Unordered_set<Key, Hash, KeyEqual, Allocator>::
		begin(size_type i)
	{
		return __bucket[i].begin();
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::local_iterator
		Unordered_set<Key, Hash, KeyEqual, Allocator>::
		end(size_type i)
	{
		return __bucket[i].end();
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::iterator
		Unordered_set<Key, Hash, KeyEqual, Allocator>::
		find(const key_type& key)
	{
		auto index = __bucket_index(key);
		for (auto it = begin(index); it != end(index); ++it)
		{
			if (key_equal()(key, *it))
				return iterator(index, it, this);
		}
		return end();
	}
	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
		Unordered_set<Key, Hash, KeyEqual, Allocator>::
		count(const key_type& key)
	{
		auto it = find(key);
		return it == end() ? 0 : 1;
	}
	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	MySTL::pair<typename Unordered_set<Key, Hash, KeyEqual, Allocator>::iterator,
		bool> Unordered_set<Key, Hash, KeyEqual, Allocator>::
		insert(const value_type& val)
	{
		if (!has_key(val))
		{
			if (load_factor() > __max_load_factor)
				rehash(__next_prime(size()));
			auto index = __bucket_index(val);
			__bucket[index].push_front(val);
			++__size;
			//return MySTL::pair<iterator(index, __bucket[index], this), true>
			return MySTL::
				pair<iterator, bool>(iterator(index, __bucket[index].begin(),
					this), true);
		}
		return MySTL::pair<iterator, bool>(end(), false);
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	template <typename InputIterator>
	void  Unordered_set<Key, Hash, KeyEqual, Allocator>::
		insert(InputIterator first, InputIterator last)
	{
		for (; first != last; ++first)
			insert((value_type)*this);
	}
	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::iterator
		Unordered_set<Key, Hash, KeyEqual, Allocator>::
		erase(iterator position)
	{
		--__size;
		iterator t = position++;
		size_t index = t._bucket_index;
		auto it = __bucket[index].erase(t._iterator);
		return position;
	}

	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	typename Unordered_set<Key, Hash, KeyEqual, Allocator>::size_type
		Unordered_set<Key, Hash, KeyEqual, Allocator>::
		erase(const key_type& key)
	{
		auto it = find(key);
		if (it == end()) return 0;
		else
		{
			erase(it);
			return 1;
		} 
	}
	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	float Unordered_set<Key, Hash, KeyEqual, Allocator>::
		max_load_factor() const
	{
		return __max_load_factor;
	}
	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	void Unordered_set<Key, Hash, KeyEqual, Allocator>::rehash(size_type n)
	{
		if (n <= __bucket.size()) return;
		Unordered_set temp(__next_prime(n));
		for (auto& val : *this)
			temp.insert((value_type)val);
		MySTL::swap(*this, temp);
	}
	template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
	void swap(Unordered_set<Key, Hash, KeyEqual, Allocator>& lhs,
		Unordered_set<Key, Hash, KeyEqual, Allocator>& rhs)
	{
		MySTL::swap(lhs.__bucket, rhs.__bucket);
		MySTL::swap(lhs.__size, rhs.__size);
		MySTL::swap(lhs.__max_load_factor, rhs.__max_load_factor);
	}
}

#endif // !_UNORDERED_SET_IMPL_H