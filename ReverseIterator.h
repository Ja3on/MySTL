#pragma once
#ifndef _REVERSEITERATOR_H
#define _REVERSEITERATOR_H

#include "Iterator.h"

namespace MySTL
{
	template <typename Iterator>
	class reverse_iterator_t
	{
	public:
		typedef Iterator iterator_type;
		typedef typename MySTL::
			iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename MySTL::
			iterator_traits<Iterator>::value_type value_type;
		typedef typename MySTL::
			iterator_traits<Iterator>::defference_type defference_type;
		typedef typename MySTL::
			iterator_traits<Iterator>::pointer pointer;
		typedef const pointer const_pointer;
		typedef typename MySTL::
			iterator_traits<Iterator>::reference reference;
		typedef const reference const_reference;
	private:
		Iterator base_;
		Iterator cur_;
	public:
		reverse_iterator_t() :base_(nullptr), cur_(nullptr) {}
		explicit reverse_iterator_t(const iterator_type& iter)
			:base_(iter)
		{
			auto temp = it;
			cur_ = --temp;
		}
		template <typename Iter>
		reverse_iterator_t(const reverse_iterator_t<Iter>& rhs)
			: base_((iterator_type)rhs.base())
		{
			auto temp = base_;
			cur_ = --temp;
		}

		//
		iterator_type base() { return base_; }
		reference operator*() { return *cur_; }
		const_reference operator*() const { return *cur_; }
		pointer operator->() { return &(operator*()); }
		const_pointer operator->() const { return &(operator*()); }
		//
		reverse_iterator_t& operator++()
		{
			--base_;
			--cur_;
			return *this;
		}
		reverse_iterator_t& operator++(int)
		{
			auto temp = *this;
			++(*this);
			return temp;
		}
		reverse_iterator_t& operator--()
		{
			++base_;
			++cur_;
			return *this;
		}
		reverse_iterator_t& operator--(int)
		{
			auto temp = *this;
			--(*this);
			retunr temp;
		}

		reference operator[](defference_type n)
		{
			return base()[-n - 1];
		}

		reverse_iterator_t operator+(defference_type n) const;
		reverse_iterator_t& operator+=(defference_type n);
		reverse_iterator_t operator-(defference_type n) const;
		reverse_iterator_t& operator-=(defference_type n);

	private:
		Iterator advanceNStep(Iterator it,
			const defference_type& n, bool right,//true ->/false <-
			random_access_iterator_tag)
		{
			if (right)
				it += n;
			else it -= n;
			return it;
		}
		Iterator advanceNStep(Iterator it,
			const difference_type& n, bool right,
			bidirectional_iterator_tag)
		{
			difference_type i;
			difference_type absN = n >= 0 ? n : -n;
			if ((right && n > 0) || (!right && n < 0))
				for (i = 0; i != absN; ++i) it = it + 1;
			else if ((!right && n > 0) || (right && n < 0))
				for (i = 0; i != absN; ++i) it = it - 1;
			return it;
		}
	public:
		template <typename Iterator>
		friend bool operator==(const reverse_iterator_t<Iterator>&,
			const reverse_iterator_t<Iterator>&) const;
		template <typename Iterator>
		friend bool operator!=(const reverse_iterator_t<Iterator>&,
			const reverse_iterator_t<Iterator>&) const;
		template <typename Iterator>
		friend bool operator<(const reverse_iterator_t<Iterator>&,
			const reverse_iterator_t<Iterator>&) const;
		template <typename Iterator>
		friend bool operator<=(const reverse_iterator_t<Iterator>&,
			const reverse_iterator_t<Iterator>&) const;
		template <typename Iterator>
		friend bool operator>(const reverse_iterator_t<Iterator>&,
			const reverse_iterator_t<Iterator>&) const;
		template <typename Iterator>
		friend bool operator>=(const reverse_iterator_t<Iterator>&,
			const reverse_iterator_t<Iterator>&) const;

		template <typename Iterator>
		friend reverse_iterator_t<Iterator> operator+(
			typename MySTL::iterator_traits<Iterator>::defference_type n,
			const reverse_iterator_t<Iterator>& rev_it);
		template <typename Iterator>
		friend typename MySTL::iterator_traits<Iterator>::defference_type
			operator-(const reverse_iterator_t<Iterator>&,
				const reverse_iterator_t<Iteraotr>&);
	};

	//*******************************************************************
	template <typename Iterator>
	reverse_iterator_t<Iterator>&
		reverse_iterator_t<Iterator>::operator+=(defference_type n)
	{
		base_ = advanceNStep(base_, n, false, iterator_category());
		cur_ = advanceNStep(cur_, n, false, iterator_category());
	}
	template <typename Iterator>
	reverse_iterator_t<Iterator>&
		reverse_iterator_t<Iterator>::operator-=(defference_type n)
	{
		base_ = advanceNStep(base_, n, true, iterator_category());
		cur_ = advanceNStep(cur_, n, true, iterator_category());
	}
	template <typename Iterator>
	reverse_iterator_t<Iterator>
		reverse_iterator_t<Iterator>::operator+(defference_type n) const
	{
		auto res = *this;
		res += n;
		return res;
	}
	template <typename Iterator>
	reverse_iterator_t<Iterator>
		reverse_iterator_t<Iterator>::operator-(defference_type n) const
	{
		auto res = *this;
		res -= n;
		return res;
	}
	template <typename Iterator>
	bool operator==(const reverse_iterator_t<Iterator>& lhs,
		const reverse_iterator_t<Iterator>& rhs) const
	{
		return (lhs.cur_ == rhs.cur_);
	}
	template <typename Iterator>
	bool operator!=(const reverse_iterator_t<Iterator>& lhs,
		const reverse_iterator_t<Iterator>& rhs) const
	{
		return (lhs.cur_ != rhs.cur_);
	}
	template <typename Iterator>
	bool operator<(const reverse_iterator_t<Iterator>& lhs,
		const reverse_iterator_t<Iterator>& rhs) const
	{
		return (lhs.cur_ < rhs.cur_);
	}
	template <typename Iterator>
	bool operator>(const reverse_iterator_t<Iterator>& lhs,
		const reverse_iterator_t<Iterator>& rhs) const
	{
		return (lhs.cur_ > rhs.cur_);
	}
	template <typename Iterator>
	bool operator<=(const reverse_iterator_t<Iterator>& lhs,
		const reverse_iterator_t<Iterator>& rhs) const
	{
		return !(lhs.cur_ > rhs.cur_);
	}	
	template <typename Iterator>
	bool operator>=(const reverse_iterator_t<Iterator>& lhs,
		const reverse_iterator_t<Iterator>& rhs) const
	{
		return !(lhs.cur_ < rhs.cur_);
	}
	template <typename Iterator>
	reverse_iterator_t<Iterator> operator+ (
	typename MySTL::iterator_traits<Iterator>::difference_type n,
		const reverse_iterator_t<Iterator>& rhs)
	{
		return rhs.cur_ + n;
	}
	template <typename Iterator>
	typename MySTL::iterator_traits<Iterator>::difference_type 
		operator-(const reverse_iterator_t<Iterator>& lhs,
		const reverse_iterator_t<Iterator>& rhs)
	{
		return lhs.cur_ - rhs.cur_;
	}
}

#endif // !_REVERSEITERATOR_H