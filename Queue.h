#pragma once
#ifndef _QUEUE_H
#define _QUEUE_H

#include "../MySTL/Deque.h"
#include "../MySTL/Vector.h"
#include "../MySTL/Functional.h"

namespace MySTL
{
	template <typename T,typename Sequence=MySTL::deque<T>>
	class queue
	{
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence container_type;
	private:
		container_type container_;
	public:
		queue(){}
		explicit queue(const container_type& val):container_(val){}
		bool empty() const { return container_.empty(); }
		size_type size() const { return container_.size(); }
		reference front() { return container_.front(); }
		const_reference front() const { return container_.front(); }
		reference back() { return container_.back(); }
		const_reference back() { return container_.back(); }
		void push(const value_type& val) { container_.push_back(val); }
		void pop() { container_.pop_front(); }
		void swap(queue& rhs) { container_.swap(rhs.container_); }
	public:
		template<typename T,typename Sequence>
		friend bool operator==(const queue<T, Sequence>&, 
			const queue<T, Sequence>&);
		template<typename T, typename Sequence>
		friend bool operator!=(const queue<T, Sequence>&,
			const queue<T, Sequence>&);
		template<typename T, typename Sequence>
		friend void swap(queue<T, Sequence>&,queue<T, Sequence>&);
	};
	template <typename T,typename Sequence>
	bool operator==(const queue<T,Sequence>& lhs,const queue<T,Sequence>& rhs)
	{	return lhs.container_ == rhs.container_;	}
	template <typename T, typename Sequence>
	bool operator!=(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs)
	{	return !(lhs == rhs);	}
	template<typename T, typename Sequence>
	void swap(queue<T, Sequence>& lhs, queue<T, Sequence>& rhs)
	{	lhs.swap(rhs);	}


}

#endif // !_QUEUE_H