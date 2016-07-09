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
		//��Ԫ�ظ�����bucket�Ĵ�С�Ա�,���ǰ�ߴ��ں���,�ؽ����;
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n)
		{
			const size_type n = next_size(num_elements_hint);//�ҳ���һ������
			if (n > old_n)
			{
				std::vector<node*, Alloc> tmp(n, (node*)0);//�µ�buckets
				for (size_type bucket = 0; bucket < old_n; ++bucket)
				{
					//����ÿһ���ɵ�bucket
					node* first = buckets[bucket];//ÿ�����е���ʼ�ڵ�
					while (first)//����ÿ�����еĽڵ�
					{	//�ҳ��ڵ����ĸ��µ�bucket��
						size_type new_bucket = bkt_num(first->val, n);
						////��ɵ�bucketָ��������Ӧ���е���һ���ڵ�
						buckets[bucket] = first->next;
						//����ǰ�ڵ���뵽�µ�bucket��,��Ϊ���Ӧ���еĵ�һ���ڵ�
						first->next = tmp[new_bucket];
						tmp[new_bucket] = first;
						//�ص��ɵ�bucket��ָ����,׼��������һ���ڵ�
						first = bucket[bucket];
					}
				}
				//�¾ɶԵ�
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
		const size_type n = bkt_num(obj);//objλ��#n bucket
		node* first = buckets[n];//firstָ��bucket��Ӧ���е�ͷ��

		for (node* cur = first; cur; cur = cur->next)
			if (equals(get_key(cur->val), get_key(obj)))//����������Ѿ�����,���̷���
				return std::pair<iterator, bool>(iterator(cur, this), false);
		node* tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;//�½ڵ�Ϊ����ĵ�һ���ڵ�
		++num_elements;//�ڵ����
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

		//û���ظ�����
		node* tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return iterator(tmp,this);
	}


}