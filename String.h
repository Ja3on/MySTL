#pragma once
#ifndef _STRING_H
#define _STRING_H

#include "../MySTL/Allocator.h"
#include "../MySTL/Iterator.h"
#include "../MySTL/ReverseIterator.h"
#include "../MySTL/UninitializedFunctions.h"
#include "../MySTL/Utility.h"

namespace MySTL
{
	class string
	{
	public:
		typedef char value_type;
		typedef char* iterator;
		typedef const char* const_iterator;
		typedef MySTL::reverse_iterator_t<char*>  reverse_iterator;
		typedef MySTL::reverse_iterator_t<const char*> const_reverse_iterator;
		typedef char& reference;
		typedef const char& const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;
		static const size_t npos = -1;
	public:
		string():start_(0),finish_(0),endOfStorage_(0){}
		string(const string&);
		string(string&&);
		string(const string&, size_type, size_t len = npos);
		string(const char*);
		string(const char*, size_type);
		string(size_type, char);
		template <typename InputIterator>
		string(InputIterator first, InputIterator last);

		string& operator=(const string&);
		string& operator=(string&&);
		string& operator=(const char*);
		string& operator=(char);

		~string();

		iterator begin() { return start_; }
		iterator end() { return finish_; }
		const_iterator cbegin() const { return start_; }
		const_iterator cend() const { return finish_; }
		reverse_iterator rbegin() { return reverse_iterator(finish_); }
		reverse_iterator cend() { return reverse_iterator(start_); }
		const_reverse_iterator rcbegin() const
		{	return const_reverse_iterator(finish_);	}
		const_reverse_iterator rcend() const 
		{	return const_reverse_iterator(start_);	}
		size_type size() const { return finish_ - start_; }
		size_type lenth() const { return size(); }
		size_type capacity() const { return endOfStorage_ - start_; }
		void clear()
		{
			dataAllocator::destroy(start_, finish_);
			start_ = finish_;
		}
		bool empty() const { return start_ == finish_; }
		void resize(size_t n);
		void resize(size_t n, char);
		void reserve(size_t n = 0);
		void shrink_to_fit()
		{
			//dataAllocator::destroy(finish_, endOfStorage_);
			dataAllocator::deallocate(finish_, endOfStorage_ - finish_);
			endOfStorage_ = finish_;
		}
		char& operator[](const size_type& index) { return *(start_ + index); }
		const char& operator[](const size_type& index) const
		{	return *(start_ + index);	}
		char& back() { return *(finish_ - 1); }
		const char& back() const { return *(finish_ - 1); }
		char& front() { return *start_; }
		const char& front() const { return *start_; }

		void push_back(const char& c);

	private:
		iterator start_;
		iterator finish_;
		iterator endOfStorage_;
		typedef MySTL::allocator<char> dataAllocator;

	};
}


#endif // !_STRING_H