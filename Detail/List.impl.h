#pragma once
#ifndef _LIST_IMPL_H
#define _LIST_IMPL_H
// !_LIST_IMPL_H
namespace MySTL
{
	namespace Detail
	{
		template <typename T>
		listIterator<T>& listIterator<T>::operator++()
		{
			p = p->next;
			return *this;
		}
		template <typename T>
		listIterator<T> listIterator<T>::operator++(int)
		{
			auto temp = *this;
			++*this;
			return temp;
		}
		template <typename T>
		listIterator<T>& listIterator<T>::operator--()
		{
			p = p->prev;
			return *this;
		}
		template <typename T>
		listIterator<T> listIterator<T>::operator--(int)
		{
			auto temp = *this;
			--*this;
			return temp;
		}
		template <typename T>
		bool operator==(const listIterator<T>& lhs,
			const listIterator<T>& rhs)
		{
			return  lhs.p == rhs.p;
		}
		template <typename T>
		bool operator!=(const listIterator<T>& lhs,
			const listIterator<T>& rhs) {
			return !(lhs == rhs);
		}
	}
	template <typename T>
	list<T>::list()
	{
		head.p = newNode();
		tail.p = head.p;
	}
	template <typename T>
	list<T>::list(const size_type& n, const value_type& val = value_type())
	{
		ctorAux(n, val, std::is_integral<value_type>());
	}

	template <typename T>
	list<T>& list<T>::operator=(const list<T>& rhs)
	{
		if (this != &rhs)
			list(rhs).swap(*this);//list(rhs)调用拷贝构造函数生成一个局部变量
									//局部变量离开作用域后销毁;
		return *this;
	}
	template <typename T>
	list<T>::~list()
	{
		for (; head != tail;)
		{
			auto temp = head++;
			nodeAllocator::destroy(temp.p);
			nodeAllocator::deallocate(temp.p);
		}
		nodeAllocator::deallocate(tail.p);
	}
	template <typename T>
	typename list<T>::size_type list<T>::size() const
	{
		size_type length = 0;
		for (auto h = head; h != tail; ++h)
			++lengh;
		return lengh;
	}
 	template<typename T>
	typename list<T>::nodePtr list<T>::newNode(const T& val = T())
	{
		nodePtr res = nodeAllocator::allocate();
		nodeAllocator::
			construct(res, Detail::node<T>(val, nullptr, nullptr, this));
		return res;
	}
	template <typename T>
	void list<T>::deleteNode(nodePtr p)
	{
		p->prev = p->next = nullptr;
		nodeAllocator::destroy(p);
		nodeAllocator::deallocate(p);
	}
	template <typename T>
	void list<T>::push_front(const value_type& val)
	{
		Detail::node<T>* node = newNode(val);
		head.p->prev = node;
		node->next = head.p;
		head.p = node;
	}
	template <typename T>
	void list<T>::pop_front()
	{
		Detail node<T>* oldNode = head.p;
		head.p = oldNode->next;
		head.p->prev = nullptr;
		deleteNode(oldNode);
	}
	template <typename T>
	void list<T>::push_back(const value_type& val)
	{
		Detail::node<T>* node = newNode(val);
		tail.p ->data = val;
		tail.p->next = node;
		node->next = tail.p;
		tail.p = node;
	}
	template <typename T>
	void list<T>::pop_back()
	{
		typename list<T>::iterator newTail = tail.p->prev;
		newTail.p->next = nullptr;
		deleteNode(tail.p);
		tail.p = nullptr;
	}

	template <typename T>
	void list<T>::ctorAux(const size_type& n, const value_type& val,
		std::true_type)
	{
		head.p = newNode();
		tail.p = head.p;
		while (n--)
			push_back(val);
	}
	template <typename T>
	template <typename InputIterator>
	void list<T>::ctorAux(InputIterator first, InputIterator last,
		std::false_type)
	{
		head.p = newNode();
		tail.p = head.p;
		for (; first != last, ++first)
			push_back(*first);
	}
	template <typename T>
	void list<T>::insert_aux(iterator position,const size_type& n, 
		const T& val, std::true_type)
	{
		for (auto i = n; i != 0; --i)
			position = insert(position, val);
	}
	template <typename T>
	template <typename InputIterator>
	void list<T>::insert_aux(iterator position, InputIterator first,
		InputIterator last, std::false_type)
	{
		for (--last; first != last; --last)
		{
			position = insert(position, *last);
		}
		insert(position, *last);
	}
	template <typename T>
	typename list<T>::iterator list<T>::
		insert(iterator position, const value_type& val)
	{
		typedef typename list<T> ::nodePtr np;
		if (position == begin())
		{
			this->push_front(val);
			return begin();
		}
		else if (position == end())
		{
			auto ret = position;
			push_back(val);
			return ret;
		}
		np node = newNode(val);
		np prev = position.p->prev;
		np next = position.p->next;
		node->next = position.p;
		node->prev = prev;
		prev->next = node;
		position.p->prev = node;
		return iterator(node);
	}
	template <typename T>
	void list<T>::insert(iterator position, const size_type& n,
		const value_type& val)
	{
		insert_aux(position, n, val,
			typename std::is_integral<value_type>::type());
	}
	template <typename T>
	template <typename InputIterator>
	void list<T>::insert(iterator position, InputIterator first,
		InputIterator last)
	{
		insert_aux(position, first, last, 
		typename std::is_integral<InputIterator>::type());
	}
	template <typename T>
	typename list<T>::iterator list<T>::erase(iterator position)
	{
		typedef typename list<T>::nodePtr np;
		if (position == head)
		{
			pop_front();
			return head;
		}
		else
		{
			 np prev = position.p->prev;
			 prev->next = position.p->next;
			 position.p->next->prev = prev;
			 deleteNode(position.p);
			 return iterator(prev->next);
		}
	}
	template <typename T>
	typename list<T>::iterator list<T>::erase(iterator first, iterator last)
	{
		typename list<T>::iterator res;
		for (; first != last;)
		{
			auto temp = first++;
			res = erase(temp);
		}
		return res;
	}
	template <typename T>
	void list<T>::clear() { erase(begin(), end()); }
	template <typename T>
	typename list<T>::iterator list<T>::begin() { return head; }
	template <typename T>
	typename list<T>::iterator list<T>::end() { return tail; }
	template <typename T>
	typename list<T>::const_iterator list<T>::
		changeIteratorToConstIterator(iterator& t) const
	{
		using nodeP = Detail::node<const T>*;
		auto temp = (list<const T>* const)this;
		auto ptr = t.p;
		Detail::node<const T> node(ptr->data, (nodeP)(ptr->prev),
			(nodeP)(ptr->next), temp);
		return (const_iterator)(&node);
	}
	template <typename T>
	typename list<T>::const_iterator list<T>::cbegin() const
	{
		auto temp = (list* const)this;
		return changeIteratorToConstIterator(temp->head);
	}
	template <typename T>
	typename list<T>::const_iterator list<T>::cend() const
	{
		auto temp = (list* const)this;
		return changeIteratorToConstIterator(temp->tail);
	}
	template <typename T>
	 typename list<T>::reverse_iterator list<T>::rbegin() 
	 {
		 return MySTL::reverse_iterator_t(tail);
	 }
	 template <typename T>
	 typename list<T>::reverse_iterator list<T>::rend()
	 {
		 return MySTL::reverse_iterator_t(head);
	 }
	 template <typename T>
	 void list<T>::reverse()
	 {
		 if (isEmpty() || head.p->next == tail.p) return;
		 typename list<T>::nodePtr curNode = head.p;
		 head.p = tail.p->prev;
		 head.p->prev = nullptr;
		 do
		 {
			 typename list<T>::nodePtr nextNode = curNode->next;
			 curNode->next = head.p->next;
			 head.p->next->prev = curNode;
			 head.p->next = curNode;
			 curNode->prev = head.p;
			 curNode = nextNode;
		 } while(curNode!=head.p)
	 }
	 template <typename T>
	 void list<T>::remove(const value_type& val)
	 {
		 for (auto iter = begin(); iter != end();)
			 if (*iter = val)
				 iter = erase(val);
			 else ++iter;
	 }
	 template <typename T>
	 template<typename Predicate>
	 void list<T>::remove_if(Predicate pred)
	 {
		 for (auto iter = begin(); iter != end();)
			 if (pred(*iter))
				 iter = erase(iter);
			 else ++iter;
	 }
	 template <typename T>
	 void list<T>::swap(list& rhs)
	 {
		 MySTL::swap(head, rhs.head);
		 MySTL::swap(tail, rhs.tail);
	 }
	 template <typename T>
	 void swap(list<T>& lhs,list<T>& rhs)
	 {
		 lhs.swap(rhs);	
	 }
	 template <typename T>
	 void list<T>::unique()
	 {
		 nodePtr curNode = head.p;
		 while (curNode != tail.p)
		 {
			 nodePtr nextNode = curNode->next;
			 if (curNode->data == nexNode->data)
			 {
				 if (nextNode == tail.p)
				 {
					 curNode->next = nullptr;
					 tail.p = curNode;
				 }
				 else
				 {
					 curNode->next = nextNode->next;
					 nextNode->next->prev = curNode;
				 }
				 deleteNode(nextNode);
			 }
			 else
				 curNode = nextNode;
		 }
	 }
	 template <typename T>
	 template <typename BinaryPredicate>
	 void list<T>::unique(BinaryPredicate pred)
	 {
		 nodePtr curNode = head.p;
		 while (curNode != tail.p)
		 {
			 nodePtr nextNode = curNode->next;
			 if (pred(curNode->data, nextNode->data))
			 {
				 if (nextNode == tail.p)
				 {
					 curNode->next = nullptr;
					 tail.p = curNode;
				 }
				 else
				 {
					 curNode->next = nextNode->next;
					 nextNode->next->prev = curNode;
				 }
				 deleteNode(nextNode);
			 }
			 else
				 curNode = nextNode;
		 }
	 }
	 template <typename T>
	 void list<T>::splice(iterator position, list<T>& l)
	 {
		 this->insert(position, l.begin(), l.end());
		 l.head.p = l.tail.p;
	 }
	 template <typename T>
	 void list<T>::splice(iterator position, list<T>& l,
		 iterator first, iterator last)
	 {
		 if (first.p == last.p) return;
		 nodePtr tailNode = last.p->prev;
		 if (l.head.p == first.p)
		 {
			 l.head.p = last.p;
			 l.head.p->prev = nullptr;
		 }
		 else
		 {
			 first.p->next->prev = last.p;	
			 last.p-> prev = first.p->prev;
		 }
		 if (position.p == head.p)
		 {
			 first.p->prev = nullptr;
			 tailNode->next = head.p;
			 head.p->prev = tailNode;
			 head.p = first.p;
		 }
		 else
		 {
			 position.p->prev->next = first.p;
			 first.p->prev = position.p->prev;
			 tailNode->next = position.p;
			 position.p->prev = tailNode;
		 }
	 }
	 template <typename T>
	 void list<T>::splice(iterator position, list<T>& l,
		 iterator iter)
	 {
		this -> splice(position, l, iter, ++iter);
	 }
	 template <typename T>
	 void list<T>::merge(list& rhs)
	 {
		 iterator iter1 = begin(), iter2 = rhs.begin();
		 while (iter1 != end() && iter2 != rhs.end())
		 {
			 if (*iter1 <= *iter2)
				 ++iter1;
			 else
			 {
				 iterator temp = iter2++;
				 splice(iter1, rhs, temp);
			 }
		 }
		 if (iter1 == end())
			 this->splice(iter1, rhs, iter2, rhs.end());
	 }
	 template <typename T>
	 template <typename Compare>
	 void list<T>::merge(list& rhs, Compare comp)
	 {
		 iterator iter1 = begin(), iter2 = rhs.begin();
		 while (iter1 != end() && iter2 != rhs.end())
		 {
			 if(comp(*iter1,*iter2))
				 {
					 iterator temp = iter2++;
					 splice(iter1, rhs, temp);
				 }	
			 else
				 ++iter1;
		 }
		 if (iter1 == end())
			 this->splice(iter1, rhs, iter2, rhs.end());
	 }
	 template <typename T>
	 bool operator==(const list<T>& lhs, const list<T>& rhs)
	 {
		 Detail::node<T>* node1 = lhs.head.p;
		 Detail::node<T>* node2 = rhs.head.p;
		 for (; node1 != lhs.tail.p && node2 != rhs.head.p;
			 node1 = node1->next, node2 = node2->next)
		 {
			 if (node1->data != node2->data) break;
		 }
		 if (node1 == lhs.tail.p && node2 == rhs.tail.p) return true;
		 return false;
	 }
	 template <typename T>
	 void list<T>::sort()
	 {
		 sort(MySTL::less<T>());
	 }
	 template <typename T>
	 template <typename Compare>
	 void list<T>::sort(Compare comp)
	 {
		 if (isEmpty() || head.p->next == tail.p) return;
		 list<T> carry;
		 list<T> counter[64];
		 int fill = 0;
		 while (!isEmpty())
		 {
			 carry.splice(carry.begin(), *this, begin());
			 int i = 0;
			 while (i < fill && !counter[i].isEmpty())
			 {
				 counter[i].merge(carry, comp);
				 carry.swap(counter[i + 1]);
			 }
			 carry.swap(counter[i]);
			 if (i == fill)
				 ++fill;
		 }
		 for (int i = 0; i != fill; ++i)
			 counter[i].merge(counter[i - 1], comp);
		 swap(counter[fill - 1]);
	 }

}
#endif