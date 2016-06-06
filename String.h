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
		string& insert(size_t pos, const string&);
		string& insert(size_t, const string&, size_t, size_t sublen = npos);
		string& insert(size_t, const char*);
		string& insert(size_t, const char*, size_t);
		string& insert(size_t, size_t, char);
		
		iterator insert(iterator, size_t, char);
		iterator insert(iterator, char);

		template <typename InputIterator>
		iterator insert(iterator, InputIterator, InputIterator);

		string& append(const string&);
		string& append(const string&, size_t, size_t sublen = npos);
		string& append(const char*);
		string& append(const char*, size_t);
		string& append(size_t, char);

		template <typename InputIterator>
		string& append(InputIterator, InputIterator);

		string& operator+=(const string&);
		string& operator += (const char*);
		string& operator+=(char);

		string& erase(size_t, size_t len = npos);
		iterator erase(iterator);
		iterator erase(iterator, iterator);

		void pop_back() { erase(end() - 1, end()); }

		string& replace(size_t, size_t, const string&);
		string& replace(iterator, iterator, const string&);
		string& replace(size_t, size_t, const string&, size_t, size_t sublen = npos);
		string& replace(size_t, size_t, const char*);
		string& replace(size_t, size_t, const char*, size_t);
		string& replace(iterator, iterator, const char*);
		string& replace(iterator, iterator, const char*, size_t);
		string& replace(size_t, size_t, size_t, char);
		string& replace(iterator, iterator, size_t, const char);

		template <typename InputIterator>
		string& replace(iterator, iterator,InputIterator, InputIterator);

		void swap(string& str)
		{
			MySTL::swap(start_, str.start_);
			MySTL::swap(finish_, str.finish_);
			MySTL::swap(endOfStorage_, str.endOfStorage_);
		}

		size_t copy(char* s, size_t len, size_t pos = 0) const
		{
			auto ptr = MySTL::unintialized_copy(cbegin() + pos, cbegin() + pos + len, s);
			return (size_t)(ptr - s);
		}

		size_t find(const string& str, size_t pos = 0) const;
		size_t find(const char*, size_t pos = 0) const;
		size_t find(const char*, size_t, size_t) const;
		size_t find(char c, size_t pos = 0) const;

		size_t rfind(const string&, size_t pos = npos) const;
		size_t rfind(const char*, size_t pos = npos) const;
		size_t rfind(const char*, size_t, size_t) const;
		size_t rfind(char, size_t pos = npos) const;

		size_t find_first_of(const string&, size_t pos = 0) const;
		size_t find_first_of(const char*, size_t pos = 0) const;
		size_t find_first_of(const char*, size_t, size_t) const;
		size_t find_first_of(char, size_t pos = 0) const;

		size_t find_last_of(const string&, size_t pos = npos) const;
		size_t find_last_of(const char*, size_t pos = npos) const;
		size_t find_last_of(const char*, size_t, size_t) const;
		size_t find_last_of(char, size_t pos = npos)const;

		size_t find_first_not_of(const string&, size_t pos = 0) const;
		size_t find_first_not_of(const char*, size_t pos=0) const;
		size_t find_first_not_of(const char*, size_t, size_t) const;
		size_t find_first_not_of(char, size_t pos = npos) const;

		size_t find_last_not_of(const string&, size_t pos = npos) const;
		size_t find_last_not_of(const char*, size_t pos = npos) const;
		size_t find_last_not_of(const char*, size_t, size_t) const;
		size_t find_last_not_of(char, size_t pop = npos) const;

		string substr(size_t pos = 0, size_t len = npos) const
		{
			len = changeVarWhenEqualNPOS(len, size(), pos);
			return string(cbegin() + pos, cbegin() + pos + len);
		}

		int compare(const string&) const;
		int compare(size_t, size_t, const string&) const;
		int compare(size_t, size_t, const string&, size_t, size_t)const;
		int compare(const char*) const;
		int compare(size_t, size_t, const char*) const;
		int compare(size_t, size_t, const char*, size_t) const;

	private:
		size_t changeVarWhenEqualNPOS(size_t, size_t, size_t) const;
		void moveData(string&);
		template <typename InputIterator>
		iterator insert_aux_copy(iterator, InputIterator, InputIterator);
		iterator insert_aux_filln(iterator, size_t, value_type);
		size_type getNewCapacity(size_type) const;
		void allocateFillN(size_t, char);
		template <typename InputIterator>
		void allocateFillN(InputIterator, InputIterator);
		template <typename InputIterator>
		void allocateAndCopy(InputIterator, InputIterator);
		void string_aux(size_t, char, std::true_type);
		template <typename InputIterator>
		void string_aux(InputIterator, InputIterator,std::false_type);
		void destroyAndDeallocate();
		size_t rfind_aux(const_iterator, size_t, size_t, int) const;
		size_t find_aux(const_iterator, size_t, size_t, size_t) const;
		int compare_aux(size_t, size_t, const_iterator, size_t, size_t) const;
		bool isContained(char, const_iterator, const_iterator) const;
	private:
		iterator start_;
		iterator finish_;
		iterator endOfStorage_;
		typedef MySTL::allocator<char> dataAllocator;
	public:
		friend std::ostream& operator<<(std::ostream&, const string&);
		friend std::istream& operator >> (std::istream&, const string&);

		friend string operator+(const string&, const string&);
		friend string operator+(const string&, const char*);
		friend string operator+(const char*, const string&);
		friend string operator+(const string&, const char);
		friend string operator+(const char, const string&);

		friend bool operator==(const string&, const string&);
		friend bool operator==(const char*, const string&);
		friend bool operator==(const string&, const char*);

		friend bool operator!=(const string&, const string&);
		friend bool operator!=(const char*, const string&);
		friend bool operator!=(const string&, const char*);

		friend bool operator<(const string&, const string&);
		friend bool operator<(const char*, const string&);
		friend bool operator<(const string&, const char*);

		friend bool operator<=(const string&, const string&);
		friend bool operator<=(const char*, const string&);
		friend bool operator<=(const string&, const char*);

		friend bool operator>(const string&, const string&);
		friend bool operator>(const char*, const string&);
		friend bool operator>(const string&, const char*);

		friend bool operator>=(const string&, const string&);
		friend bool operator>=(const char*, const string&);
		friend bool operator>=(const string&, const char*);

		friend void swap(string&, string&);
		friend std::istream& getline(std::istream&, string&, char);
		friend std::istream& getline(std::istream&, string&);
	};
	template <typename InputIterator>
	string::string(InputIterator first, InputIterator last)
	{
		string_aux(first, last,
			typename std::is_integral<InputIterator>::type());
	}
	template <typename InputIterator>
	string::iterator string::insert_aux_copy(iterator p,
		InputIterator first, InputIterator last)
	{
		size_t lengthOfInset = last - first;
		auto newCapacity = getNewCapacity(lengthOfInset);
		iterator newStart = dataAllocator::allocate(newCapacity);
		iterator newFinish = MySTL::unintialized_copy(start_, p, newStart);
		newFinish = MySTL::unintialized_copy(first, last, newFinish);
		auto res = newFinish;
		newFinish = MySTL::unintialized_copy(p, finish_, newFinish);

		destroyAndDeallocate();
		start_ = newStart;
		finish_ = newFinish;
		endOfStorage_ = start_ + newCapacity;
		return res;
	}
	template <typename InputIterator>
	string::iterator string::insert(iterator p, InputIterator first,
		InputIterator last)
	{
		size_t lengthOfLeft = capacity() - size();
		size_t lengthOfInsert = distance(first, last);
		if (lengthOfInsert <= lengthOfLeft)
		{
			for (iterator iter = finish_ - 1; iter >= p; --iter)
				*(iter + lengthOfInsert) = *(it);
			MySTL::unintialized_copy(first, last, p);
			finish_ += lengthOfInsert;
			return (p + lengthOfInsert);
		}
		else
			return insert_aux_copy(p, first, last);
	}
	template <typename InputIterator>
	string& string::append(InputIterator first, InputIterator last)
	{
		insert(end(), first, last);
		return *this;
	}
	template <typename InputIterator>
	string& string::replace(string::iterator iter1, string::iterator iter2,
		InputIterator first, InputIterator last)
	{
		string::iterator ptr = erase(iter1, iter2);
		insert(ptr, first, last);
		return *this;
	}
	template <typename InputIterator>
	void string::allocateAndCopy(InputIterator first, InputIterator last)
	{
		start_ = dataAllocator::allocate(last - first);
		finish_ = MySTL::unintialized_copy(first, last, start_);
		endOfStorage_ = finish_;
	}
	template <typename InputIterator>
	void string::string_aux(InputIterator first, InputIterator last, std::false_type)
	{
		allocateAndCopy(first, last);
	}
}


#endif // !_STRING_H