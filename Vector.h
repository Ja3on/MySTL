
#ifndef _VECTOR_H
#define _VECTOR_H

#include <algorithm>
#include <type_traits>

#include "Allocator.h"
#include "Algorithm.h"
#include "Iterator.h"
#include "ReverseIterator.h"
#include "UninitializedFunctions.h"

namespace MySTL
{
	template <typename T,typename Alloc=allocator<T>>
	class vector
	{
	private:
		T* start_;
		T* finish_;
		T* endOfStorage_;

		typedef Alloc dataAllocator;

	public:
		typedef T			value_type;
		typedef T*			iterator;
		typedef const T*	const_iterator;
		typedef reverse_iterator_t<T*>	reverse_iterator;
		typedef reverse_iterator_t<const T*>	const_reverse_iterator;
		typedef	iterator	pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef std::ptrdiff_t	difference_type;

	public:
		vector()
			:start_(0),finish_(0),endOfStorage_(0){}
		explicit vector(const size_type);
		vector(const size_type, const value_type&);
		template <typename InputIterator>
		vector(InputIterator first, InputIterator last);

		vector(const vector&);
		vector(vector&&);
		vector& operator=(const vector&);
		vector& operator=(vector&&);//move¹¹Ôìº¯Êý

		~vector();

		//**********************************************

		iterator begin() { return start_; }
		const_iterator begin() const { return start_; }
		const_iterator cbegin() const { return start_; }

		iterator end() { return finish_; }
		const_iterator end() const { return finish_; }
		const_iterator cend() const { return finish_; }

		reverse_iterator rbegin() { return reverse_iterator(finish_); }
		const_reverse_iterator crbegin() const
		{ return reverse_iterator(finish_); }
		reverse_iterator rend() { return reverse_iterator(start_); }
		const_reverse_iterator crend() const
		{ return reverse_iterator(start_); }

		//**********************************************

		bool operator==(const vector&) const;
		bool operator!=(const vector&) const;

		//**********************************************

		difference_type size() const { return finish_ - start_; }
		difference_type capacity() const { return endOfStorage_ - start_; }
		bool empty() const { return finish_ == start_; }
		void resize(size_type n, value_type val = value_type());
		void reseve(size_type n);
		void shrink_to_fit();

		//**********************************************

		reference operator[](const difference_type index) 
		{ return *(begin() + index); }
		const_reference operator[](const difference_type index) const
		{ return *(cbegin() + index); }
		reference front() { return *begin(); }
		reference back() { return *(end()-1); }
		pointer data() { return start_; }

		//**********************************************

		void clear();
		void swap(vector&);
		void push_back(const value_type&);
		void pop_back();
		iterator insert(iterator position, const value_type& value);
		void insert(iterator position, const size_type& n, const value_type& value);
		template <typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);

		//**********************************************

		Alloc get_allocator() { return dataAllocator; }

	private:
		void destroyAndDeallocateAll();
		void allocateAndFillN(const size_type n, const value_type& value);
		template <typename InputIterator>
		void allocateAndCopy(InputIterator first, InputIterator last);

		template <typename InputIterator>
		void vector_aux(InputIterator first, InputIterator last, 
			std::false_type);
		template <typename Integer>
		void vector_aux(Integer n, const value_type&,std::true_type);
		template<typename Integer>
		void insert_aux(iterator position, Integer n,const value_type&
			, std::true_type);
		template  <typename InputIterator>
		void insert_aux(iterator position, InputIterator first,
			InputIterator last, std::false_type);
		template <typename InputIterator>
		void reallocateAndCopy(iterator position, InputIterator first,
			InputIterator last);
		void reallocateAndFilln(iterator position, const size_type&,
			const value_type&);
		size_type getNewCapacity(size_type len) const;

	public:
		template <typename T,typename Alloc>
		friend bool operator==(const vector<T, Alloc>&, 
			const vector<T, Alloc>&) ;
		template <typename T,typename Alloc>
		friend bool operator!=(const vector<T, Alloc>&, 
			const vector<T, Alloc>&);
	};
}
#include "Detail\Vector.impl.h"
#endif // !_VECTOR_H