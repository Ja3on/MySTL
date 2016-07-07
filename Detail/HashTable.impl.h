#include "..\HashTable.h"
namespace MySTL
{
	namespace Detail
	{
		template <typename Value, typename Key, typename HashFcn,
			typename ExtractKey, typename EqualKey, typename Alloc>
			__hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>&
			__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
			operator++()
		{
			const node* old = cur;
			cur = cur->next;
			if (!cur)
			{
				
			}		
			return *this;
		}
		template <typename Value, typename Key, typename HashFcn,
			typename ExtractKey, typename EqualKey, typename Alloc>
			__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
			__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
			operator++(int)
		{
			auto temp = *this;
			++(*this);
			return temp;
		}
	}
	template<typename Value, typename Key, typename HashFcn,
		typename ExtractKey, typename EqualKey, typename Alloc>
	void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		resize(size_type num_elements_hint)
	{
		//拿元素个数和bucket的大小对比,如果前者大于后者,重建表格;
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n)
		{
			const size_type n = next_size(num_elements_hint);//找出下一个质数
			if (n > old_n)
			{
				std::vector<node*, Alloc> tmp(n, (node*)0);//新的buckets
				for (size_type bucket = 0; bucket < old_n; ++bucket)
				{
					//处理每一个旧的bucket
					node* first = buckets[bucket];//每个串行的起始节点
					while (first)//处理每个串行的节点
					{	//找出节点在哪个新的bucket里
						size_type new_bucket = bkt_num(first->val, n);
						////令旧的bucket指向其所对应串行的下一个节点
						buckets[bucket] = first->next;
						//将当前节点插入到新的bucket内,成为其对应串行的第一个节点
						first->next = tmp[new_bucket];
						tmp[new_bucket] = first;
						//回到旧的bucket所指串行,准备处理下一个节点
						first = bucket[bucket];
					}
				}
				//新旧对调
				buckets.swap(tmp);
			}
		}
	}

	template<typename Value, typename Key, typename HashFcn,
		typename ExtractKey, typename EqualKey, typename Alloc>
	inline void hashtable<Value, Key, HashFcn, ExtractKey, 
		EqualKey, Alloc>::clear()
	{
		for (size_type i = 0; i < buckets.size(); ++i)
		{
			node* cur = buckets[i];
			while (cur != 0)
			{
				node* next = cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = 0;
		}
		num_elements = 0;
	}

	template<typename Value, typename Key, typename HashFcn, 
		typename ExtractKey, typename EqualKey, typename Alloc>
	inline void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		copy_from(const hashtable &rhs)
	{
		buckets.clear();
		buckets.reserve(rhs.buckets.size());
		buckets.insert(buckets.end(), rhs.buckets.size(), (node*)0);
		for (size_type i = 0; i < rhs.buckets.size(); ++i)
		{
			if (const node* cur = rhs.buckets[i])
			{
				node* copy = new_node(cur->val);
				buckets[i] = copy;
				for (node* next = cur->next; next; cur = next, next = cur->next)
				{
					copy->next = new_node(next->val);
					copy = copy->next;
				}
			}
		}
		num_elements = rhs.num_elements;
	}

	template<typename Value, typename Key, typename HashFcn, 
		typename ExtractKey, typename EqualKey, typename Alloc>
	std::pair<typename hashtable<Value, Key, HashFcn, 
		ExtractKey, EqualKey, Alloc>::iterator, bool>
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		insert_unique_noresize(const value_type& obj)
	{
		const size_type n = bkt_num(obj);//obj位于#n bucket
		node* first = buckets[n];//first指向bucket对应串行的头部

		for (node* cur = first; cur; cur = cur->next)
			if (equals(get_key(cur->val), get_key(obj)))//如果链表中已经存在,立刻返回
				return std::pair<iterator, bool>(iterator(cur, this), false);
		node* tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;//新节点为链表的第一个节点
		++num_elements;//节点个数
		return std::pair<iterator, bool>(iterator(tmp, this), true);
	}

	template<typename Value, typename Key, typename HashFcn, 
		typename ExtractKey, typename EqualKey, typename Alloc>
	inline typename hashtable<Value, Key, HashFcn,
		ExtractKey, EqualKey, Alloc>::iterator
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
		insert_equal_noresize(const value_type &obj)
	{
		const size_type n = bkt_num(obj);
		node* first = buckets[n];
		for(node* cur=first;cur;cur=cur->next)
			if (equals(get_key(cur->val), get_key(obj)))
			{
				node* tmp = new_node(obj);
				tmp->next = cur->next;
				cur->next = tmp;
				++num_elements;
				return iterator(tmp, this);
			}

		//没有重复出现
		node* tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return iterator(tmp,this);
	}


}