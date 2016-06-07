#pragma once
#ifndef _PRIORITY_QUEUE_H
#define _PRIORITY_QUEUE_H
#include "../MySTL/Heap.h"

#include "../MySTL/Vector.h"

namespace MySTL
{
	template <typename T,typename Sequence=vector<T>,
	typename Compare=less<typename Sequence::value_type>>
	class priority_queue
	{
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;
		typedef typename Sequence::const_iterator const_iterator;
	protected:
		Sequence container_;
		Compare comp;
	public:
		priority_queue():container_(){}
		explicit priority_queue(const Compare& x):container_(),comp(x){}

		template <typename InputIterator>
		priority_queue(InputIterator first,InputIterator last,const Compare& x)
			:container_(first,last),comp(x)
		{
			Heap::make_heap(container_.begin(), container_.end(), comp);
		}
		template <typename InputIterator>
		priority_queue(InputIterator first,InputIterator last)
			:container_(first,last)
		{
			Heap::make_heap(container_.begin(), container_.end(), comp);
		}
		
		bool empty() const { return container_.emtpy(); }
		size_type size() const { return container_.size(); }
		const_reference top() const { return container_.top(); }
		void push(const value_type& va)
		{
			container_.push_back(val);
			Heap::push_heap(constainer_.begin(), container_.end(), comp);
		}
		void pop()
		{
			Heap::pop_heap(container_.begin(), container_.end(), comp);
			container_.pop_back();
		}

	};


}

#endif // 