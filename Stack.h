#pragma once
#ifndef _STACK_H
#define _STACK_H

#include "../MySTL/Vector.h"

namespace MySTL
{
	template <typename T,typename Container=MySTL::vector<T>>
	class stack
	{
	public:
		typedef typename Container::value_type value_type;
		typedef typename Container::reference reference;
		typedef typename Container::size_type size_type;
		typedef Container container_type;
	private:
		container_type container_;
	public:
		explicit stack(const container_type& ctnr=container_type())
			:container_(ctnr){}
		bool empty() const { return container_::empty(); }
		size_type size() const { return container_::size(); }
		value_type& top() { return container_::back(); }
		const value_type& top() const { return container_::back(); }
		void push(const value_type& val) { container_::push_back(val); }
		void pop() { container_::pop_back(); }
		void swap(stack& rhs) { MySTL::swap(container_, rhs.container_); }
	public:
		template <typename T,typename Container>
		friend bool operator==(const stack<T, Container>&, 
			const stack<T, Container>&);
		template <typename T,typename Container>
		friend bool operator!=(const stack<T, Container>&,
			const stack<T, Container>&);
		template <typename T,typename Container>
		friend void swap(stack<T, Container>&, stack<T, Container>&);
	};
	template <typename T,typename Container>
	bool operator==(const stack<T, Container>& lhs, 
		const stack<T, Container>& rhs)
	{	return lhs.container_ == rhs.container_; }
	template <typename T, typename Container>
	bool operator!=(const stack<T, Container>& lhs,
		const stack<T, Container>& rhs)	{	return !(lhs==rhs);	}
	template <typename T, typename Container>
	void swap(stack<T, Container>& lhs, stack<T, Container>& rhs)
	{	lhs.swap(rhs);	}
}

#endif // !_STACK_H