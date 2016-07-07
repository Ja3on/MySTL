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


}