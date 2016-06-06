
#ifndef _VECTOR_IMPL_H
#define _VECTOR_IMPL_H

namespace MySTL
{
	template <typename T,typename Alloc>
	vector<T, Alloc>::~vector()
	{
		destroyAndDeallocateAll();
	}
	template <typename T,typename Alloc>
	vector<T, Alloc>::vector(const size_type n)
	{
		allocateAndFillN(n, value_type());
	}
	template <typename T,typename Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type& value)
	{
		allocateAndFillN(n, value);
	}
	template <typename T,typename Alloc>
	template <typename InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last)
	{
		vector_aux(first,last,
			typename std::is_integral<InputIterator>::type())
	}
	template <typename T,typename Alloc>
	vector<T, Alloc>::vector(const vector& rhs)
	{ allocateAndCopy(rhs.start_, rhs.finish_); }

	template <typename T,typename Alloc>
	vector<T,Alloc>::vector(vector&& rhs) 
	{
		start_ = rhs.start_;
		finish_ = rhs.finishi_;
		endOfStorage_ = rhs.endOfStorage_;
		rhs.start_ = rhs.finish_ = rhs.endOfStorage_ = 0;
	}
	template <typename T,typename Alloc>
	vector<T,Alloc>& vector<T,Alloc>::
		operator=(const vector& rhs)
	{
		if (this != &rhs)
			allocateAndCopy(rhs.start_, rhs.finish_);
		return *this;
	}
	template <typename T,typename Alloc>
	vector<T,Alloc>& vector<T,Alloc>::
		operator=(vector&& rhs)
	{
		if (this != rhs)
		{
			destroyAndDeallocateAll();
			start_ = rhs.start_;
			finish_ = rhs.finish_;
			endOfStorage_ = rhs.endOfStorage_;
			rhs.start_ = rhs.finish_ = rhs.endOfStorage_ = 0;
		}
		return *this;
	}
	template <typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::
		operator=(const vector& rhs)
	{
		if (this != &rhs)
			allocateAndCopy(rhs.start_,rhs.finish_);
		return *this;
	}
	//*****************************************************
	template <typename T,typename Alloc>
	void vector<T,Alloc>::
		resize(size_type n,value_type val=value_type())
	{
		if (n < size())
		{
			dataAllocator::destroy(start_ + n, finish_);
			finish_ = start_ + n;
		}
		else if (n > size() && n <= capacity())
		{
			auto lengthOfInsert = n - size();
			finish_ = MySTL::uninitialized_fill_n(finish_,
				lengthOfInsert, val);
		}
		else if (n > capacity())
		{
			auto lengthOfInsert = n - size();
			T* newStart = dataAllocator::
				allocate(getNewCapacity(lengthOfInsert));
			T* newFinish = MySTL::
				unintialized_copy(begin(), end(), newStart);
			newFinish = MySTL::
				uninitialized_fill_n(newFinish, lengthOfInsert,val);

			destroyAndDeallocateAll();
			start_ = newStart;
			finish_ = newFinish;
			endOfStorage_ = start_ + n;
		}
	}
	template <typename T,typename Alloc>
	void vector<T, Alloc>::reseve(size_type n)
	{
		if (n < capacity())
			return;
		T* newStart = dataAllocator::allocate(n);
		T* newFinish = MySTL::
			unintialized_copy(begin(), end(), newStart);
		destroyAndDeallocateAll();
		start_ = newStart;
		finish_ = newFinish;
		endOfStorage_ = start_ + n;
	}
	//*********************************************************************
	template <typename T,typename Alloc>
	typename vector<T,Alloc>::iterator
		vector<T, Alloc>::erase(iterator first, iterator last)
	{
		difference_type lenOfTail = end() - last;//尾部残留长度
		difference_type lenOfRemoved = last - first;//要删除的长度
		finish_ = finish_ - lenOfRemoved;
		for(;lenOfTail!=0;--lenOfTail)
		{
			auto temp = (last - lenOfRemoved);
			*temp = *(last++);
		}
		return first;
	}
	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator
		vector<T, Alloc>::erase(iterator position)
	{
		return erase(position, position + 1);
	}
	template <typename T,typename Alloc>
	template <typename InputIterator>
	void vector<T, Alloc>::reallocateAndCopy(iterator position,
		InputIterator first, InputIterator last)
	{
		//newCapacity>=oldCapacity*2;
		difference_type newCapacity = 
			getNewCapacity(last - first);
		
		T* newStart = dataAllocator::allocate(newCapacity);
		T* newEndOfStore = newStart + newCapacity;
		//复制begin()到position的数据到新的vector,
		T* newFinish = MySTL::unintialized_copy(begin(), position, newStart);
		//复制first到last的数据到新的vector,接在position的前一个位置
		newFinish = MySTL::unintialized_copy(first, last, newFinish);
		//复制position到end()的数据到新的vector,接在last的前一个位置,
		//并更新finish_
		newFinish = MySTL::unintialized_copy(position, end(), newFinish);
		//销毁原vector
		destroyAndDeallocateAll();
		start_ = newStart;
		finish_ = newFinish;
		endOfStorage_ = newEndOfStore;
	}
	template <typename T,typename Alloc>
	void vector<T, Alloc>::reallocateAndFilln(iterator position,
		const size_type& n, const value_type& val)
	{
		difference_type newCapacity = getNewCapacity(n);
		T* newStart = dataAllocator::allocate(newCapacity);
		T* newEndOfStorage = newStart + newCapacity;
		T* newFinish = MySTL::
			unintialized_copy(begin(), position, newStart);
		newFinish = MySTL::uninitialized_fill_n(newFinish, n, val);
		newFinish = MySTL::unintialized_copy(position, end(), newFinish);

		destroyAndDeallocateAll();
		start_ = newStart;
		finish_ = newFinish;
		endOfStorage_ = newEndOfStorage;
	}
	template <typename T,typename Alloc>
	template <typename InputIterator>
	void vector<T, Alloc>::insert_aux(iterator position,
		InputIterator first, InputIterator last, std::false_type)
	{
		difference_type locationLeft = endOfStorage_ - finish_;
		difference_type locationNeed = distance(last - first);

		if (locationLeft >= locationNeed)
		{
			if (finish_ - position > locationNeed)
			{
				MySTL::unintialized_copy(finish_ - locationNeed,
					locationNeed, finish_);
				std::copy_backward(position, finish_ - locationNeed,
					finish_);
				std::copy(first, last, position);
			}
			else
			{
				finish_ += locationNeed;
			}
		}
		else reallocateAndCopy(position, first, last);
	}
	template <typename T,typename Alloc>
	template <typename Integer>
	void vector<T, Alloc>::insert_aux(iterator position, Integer n,
		const value_type& val,std::true_type)
	{
		assert(n != 0);
		difference_type locationLeft = endOfStorage_ - finish_;
		difference_type locationNeed = n;
		if (locationLeft >= locationNeed)
		{
			auto tempPtr = end() - 1;
			for (; tempPtr - position >= 0; --tempPtr)
				construct(tempPtr + locationNeed, *tempPtr);
		
		MySTL::uninitialized_fill_n(position, n, val);
		finish_ += locationNeed;
		}
		else reallocateAndCopy(position, n, val);
	}
	template <typename T,typename Alloc>
	template <typename InputIterator>
	void vector<T, Alloc>::insert(iterator position,
		InputIterator first, InputIterator last)
	{
		insert_aux(position, first, last,
		typename std::is_integral<InputIterator>::type());
	}
	template <typename T,typename Alloc>
	void vector<T, Alloc>::insert(iterator position,
		const size_type& n, const value_type& val)
	{
		insert_aux(position, n, val,
			typename std::is_integral<size_type>::type());
	}
	template <typename T,typename Alloc>
	typename vector<T,Alloc>::iterator
		vector<T, Alloc>::insert(iterator position,
			const value_type& val)
	{
		const auto index = position - begin();
		insert(position, 1, val);
		return begin() + index;
	}
	template <typename T,typename Alloc>
	void vector<T, Alloc>::push_back(const value_type& val)
	{
		insert(end(), val);
	}

	//*********************************************************************

	template <typename T,typename Alloc>
	bool vector<T, Alloc>::operator==(const vector& rhs) const
	{
		if (size() != rhs.size()) return false;
		else
		{
			auto ptr1 = start_;
			auto ptr2 = rhs.start_;
			for (; ptr1 != finish_ && ptr2 != rhs.finish_;
				++ptr1, ++ptr2)
				if (*ptr1 != *ptr2) return false;
		}
		return true;
	}
	template <typename T,typename Alloc>
	bool vector<T,Alloc>::operator!=(const vector& rhs) const
	{	return !(*this == rhs);	}
	template<typename T,typename Alloc>
	bool operator==(const vector<T,Alloc>& lhs,
		const vector<T,Alloc>& rhs) const
	{	return lhs.operator==(rhs);	}
	template <typename T,typename Alloc>
	bool operator!=(const vector<T,Alloc>& lhs,
		const vector<T, Alloc>& rhs) const 
	{	return !(lhs == rhs);	}

	template <typename T,typename Alloc>
	void vector<T, Alloc>::shrink_to_fit()
	{
		T* t = (T*)dataAllocator::allocate(size());
		finish_ = MySTL::unintialized_copy(start_, finish_, t);
		dataAllocator::deallocate(start_, capacity());
		start_ = t;
		endOfStorage_ = finish_;
	}
	template <typename T,typename Alloc>
	void vector<T, Alloc>::clear()
	{
		dataAllocator::destroy(start_, finish_);
		finish_ = start_;
	}
	template <typename T,typename Alloc>
	void vector<T, Alloc>::swap(vector& rhs)
	{
		if (this != &rhs)
		{
			MySTL::swap(start_, rhs.start_);
			MySTL::swap(finish_, rhs.finish_);
			MySTL::swap(endOfStorage_, rhs.endOfStorage_);
		}
	}
	template <typename T,typename Alloc>
	void vector<T, Alloc>::pop_back()
	{
		--finish_;
		dataAllocator::destroy(finish_);
	}
	template <typename T,typename Alloc>
	void vector<T, Alloc>::destroyAndDeallocateAll()
	{
		if (capacity() != 0)
		{
			dataAllocator::destroy(start_, finish_);
			dataAllocator::deallocate(start_, capacity());
		}
	}
	template <typename T,typename Alloc>
	void vector<T, Alloc>::allocateAndFillN(const size_type n,
		const value_type& value)
	{
		start_ = dataAllocator::allocate(n);
		MySTL::uninitialized_fill_n(start_,n,value);
		finish_ = endOfStorage_ = start_ + n;
	}
	template <typename T,typename Alloc>
	template <typename InputIterator>
	void vector<T, Alloc>::allocateAndCopy(InputIterator first,
		InputIterator last)
	{
		start_ = dataAllocator::allocate(last - first);
		finish_ = MySTL::unintialized_copy(first, last, start_);
		endOfStorage_ = finish_;
	}
	template <typename T,typename Alloc>
	template <typename InputIterator>
	void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last,
		std::false_type)
	{
		allocateAndCopy(first, last);
	}
	template <typename T, typename Alloc>
	template <typename Integer>
	void vector<T, Alloc>::vector_aux(Integer n,const value_type& val,
		std::true_type)
	{
		allocateAndFillN(n, val);
	}
	template <typename T,typename Alloc>
	typename vector<T,Alloc>::size_type vector<T,Alloc>::
		getNewCapacity(size_type len) const
	{
		size_type oldCapacity = endOfStorage_ - start_;
		auto res = std::max(oldCapacity, len);
		size_type newCapacity = 
			(oldCapacity !=0 ? (oldCapacity + res) : len);
		return newCapacity;
	}
}

#endif // !_VECTOR_IMPL_H