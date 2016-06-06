#pragma once
#ifndef _DEQUE_IMPL_H
#define _DEQUE_IMPL_H

namespace MySTL
{
	namespace Detail
	{
		template <typename T>
		dq_iter<T>& dq_iter<T>::operator++()
		{
			if (cur_ != getBuckTail(mapIndex_)) ++cur_;//+1后还在同一个桶里
			else if ((mapIndex_ + 1) < container_->mapSize_) {//+1后还在一个map里
				++mapIndex_;
				cur_ = getBuckHead(mapIndex_);
			}
			else//+1后跳出了map
			{
				mapIndex_ = container_->mapSize_;
				cur_ = container_->map_[mapIndex_];//指向map_[mapSize-1]的尾后位置;
			}
			return *this;
		}
		template <typename T>
		dq_iter<T> dq_iter<T>::operator++(int)
		{
			auto temp = *this;
			++(*this);
			return temp;
		}
		template <typename T>
		dq_iter<T>& dq_iter<T>::operator--()
		{
			if (cur_ != getBuckHead(mapIndex_)) --cur_;//当前不指向桶头
			else if (mapIndex_ - 1 >= 0) {//-1后还在map里面
				--mapIndex_;
				cur_ = getBuckTail(mapIndex_);
			}
			else
			{
				mapIndex_ = 0;
				cur_ = container->map_[mapIndex_];//指向map_[0]的头
			}
		}
		template <typename T>
		dq_iter<T> dq_iter<T>::operator--(int)
		{
			auto temp = *this;
			--(*this);
			return temp;
		}
		template <typename T>
		bool dq_iter<T>::operator==(const dq_iter<T>& it) const
		{
			return ((mapIndex_ == it.mapIndex_) &&
				(cur_ == it.cur_) && (container_ == it.container_));
		}
		template <typename T>
		bool dq_iter<T>::operator!=(const dq_iter<T>& it) const
		{
			return !(*this == it);
		}
		template <typename T>
		dq_iter<T>& dq_iter<T>::operator=(const dq_iter<T>& rhs)
		{
			if (this != &rhs)
			{
				mapIndex_ = rhs.mapIndex_;
				cur_ = rhs.cur_;
				container_ = rhs.container_;
			}
			return *this;
		}
		template <typename T>
		void dq_iter<T>::swap(dq_iter<T>& rhs)
		{
			MySTL::swap(mapIndex_, rhs.mapIndex_);
			MySTL::swap(cur_, rhs.cur_);
		}

		template <typename T>
		dq_iter<T> operator+(const dq_iter<T>& lhs, typename dq_iter<T>::difference_type n)
		{
			dq_iter<T> res(lhs);
			auto m = res.getBuckTail(res.mapIndex_) - res.cur_;
			if (n <= m)//还在缓冲区内
			{
				res.cur_ += n;
			}
			else//不在缓冲区
			{
				n = n - m;
				res.mapIndex_ += (n / lhs.getBuckSize() + 1);
				auto p = res.getBuckHead(res.mapIndex_);
				auto x = n%lhs.getBuckSize() - 1;
				res.cur_ = p + x;
			}
			return res;
		}

		template <typename T>
		dq_iter<T> operator+(typename dq_iter<T>::difference_type n,
			const dq_iter<T>& rhs)
		{
			return (rhs + n);
		}
		template <typename T>
		dq_iter<T> operator-(const dq_iter<T>& lhs,
			typename dq_iter<T>::difference_type n)
		{
			dq_iter<T> res(lhs);
			auto m = res.cur_ - res.getBuckHead(res.mapIndex_);
			if (n <= m) res.cur_ -= n;
			else
			{
				n = n - m;
				res.mapIndex_ -= (n / res.getBuckSize() + 1);
				res.cur_ = res.getBuckTail(res.mapIndex_) -
					(n%res.getBuckSize() - 1);
			}
			return res;
		}
		template <typename T>
		dq_iter<T> operator-(typename dq_iter<T>::difference_type n,
			const dq_iter<T>& rhs)
		{
			return (rhs - n);
		}

		template <typename T>
		typename dq_iter<T>::difference_type operator-(const dq_iter<T>& lhs,
			const dq_iter<T>& rhs)
		{
			if (lhs.container_ == rhs.container_ && lhs.container_ == 0)
				return 0;
			return typename dq_iter<T>::difference_type(lhs.getBuckSize())*
				(lhs.mapIndex_ - rhs.mapIndex_ - 1) +
				(lhs.cur_ - lhs.getBuckHead(lhs.mapIndex_)) +
				(lhs.getBuckTail(lhs.mapIndex_) - lhs.cur_) + 1;
		}
		template <typename T>
		void swap(dq_iter<T>& lhs, dq_iter<T>& rhs)
		{
			return lhs.swap(rhs);
		}
		template <typename T>
		T* dq_iter<T>::getBuckTail(size_t mapIndex) const
		{
			return container_->map_[mapIndex] +
				(container_->getBuckSize() - 1);
		}

		template <typename T>
		T* dq_iter<T>::getBuckHead(size_t mapIndex)const
		{
			return container_->map_[mapIndex];
		}

		template <typename T>
		size_t dq_iter<T>::getBuckSize() const
		{
			return container_->getBuckSize();
		}
	}
	template <typename T, typename Alloc>
	bool deque<T, Alloc>::back_full() const
	{
		return map_[mapSize_ - 1] &&
			(map_[mapSize_]) == end().cur_;
	}
	template <typename T, typename Alloc>
	bool deque<T, Alloc>::front_full() const
	{
		return map_[0] && map_[0] == begin().cur_;
	}
	template <typename T, typename Alloc>
	void deque<T, Alloc>::deque_aux(size_t n, const value_type& val,
		std::true_type)
	{
		int i = 0;
		for (; i != n / 2; ++i) (*this).push_front(val);
		for (; i != n; ++i) (*this).push_back(val);
	}
	template <typename T, typename Alloc>
	template <typename Iterator>
	void deque<T, Alloc>::deque_aux(Iterator first, Iterator last,
		std::false_type)
	{
		difference_type mid = (last - first) / 2;
		for (auto it = first + mid; it != first - 1; --it)
			(*this).push_front(*it);
		for (auto it = first + mid + 1; it != last; ++it)
			(*this).push_back(*it);
	}
	template <typename T, typename Alloc>
	void deque<T, Alloc>::init()
	{
		mapSize_ = 2;
		map_ = getANewMap(mapSize_);
		beg_.container_ = end_.container_ = this;
		beg_.mapIndex_ = end_.mapIndex_ = mapSize_ - 1;
		beg_.cur_ = end_.cur_ = map_[mapSize_ - 1];
	}
	template <typename T, typename Alloc>
	T* deque<T, Alloc>::getANewBuck()
	{
		return typename dataAllocator::allocate(getBuckSize());
	}
	template <typename T, typename Alloc>
	T** deque<T, Alloc>::getANewMap(const size_t& size)
	{
		T** map = new T*[size];
		for (int i = 0; i != size; ++i)
			map[i] = getANewBuck();
		return map;
	}
	template <typename T, typename Alloc>
	size_t deque<T, Alloc>::getBuckSize() const
	{
		return (size_t)EBuckSize::BUCKSIZE;
	}
	template <typename T, typename Alloc>
	void deque<T, Alloc>::clear()
	{
		for (auto i = 0; i != mapSize_; ++i)
			for (auto p = map_[i] + 0; p != map[i] + getBuckSize();
				++p)
				dataAllocator::destroy(p);
		mapSize_ = 0;
		beg_.mapIndex_ = end_.mapIndex_ = mapSize_ / 2;
		beg_.cur_ = end_.cur_ = map_[mapSize_ / 2];
	}
	template <typename T, typename Alloc>
	typename deque<T, Alloc>::reference deque<T, Alloc>::
		operator[](size_type n)	{	return*(begin() + n);	}
	template <typename T,typename Alloc>
	typename deque<T,Alloc>::const_reference deque<T,Alloc>::
		operator[](size_type n)	const {		return *(begin() + n);	}
	template <typename T, typename Alloc>
	typename deque<T, Alloc>::reference deque<T, Alloc>::front()
	{	return *begin();	}
	template <typename T, typename Alloc>
	typename deque<T, Alloc>::const_reference deque<T, Alloc>::
		front() const { return *begin(); }
	template <typename T, typename Alloc>
	typename deque<T, Alloc>::reference deque<T, Alloc>::back()
	{	return *(end() - 1); }
	template <typename T,typename Alloc>
	typename deque<T,Alloc>::const_reference deque<T,Alloc>::back()
		const {	return *(end() - 1);}

	template <typename T,typename Alloc>
	typename deque<T,Alloc>::iterator deque<T,Alloc>::begin()
	{	return beg_; }
	template <typename T,typename Alloc>
	typename deque<T,Alloc>::iterator deque<T,Alloc>::begin()
		const {	return beg_; }
	template <typename T,typename Alloc>
	typename deque<T,Alloc>::iterator deque<T,Alloc>::end()
	{	return end_; }
	template <typename T,typename Alloc>
	typename deque<T,Alloc>::iterator deque<T,Alloc>::end()
		const {	 return end_; }

	template <typename T,typename Alloc>
	deque<T, Alloc>::~deque()
	{
		for (int i = 0; i < mapSize_; ++i)
		{
			for (auto p = map_[i] + 0; p != map_[i] + getBuckSize(); ++p)
				dataAllocator::destroy(p);
			if (!map_[i])
				dataAllocator::deallocate(map_[i], getBuckSize());
		}
		delete[] map_;
	}

	template <typename T,typename Alloc>
	deque<T,Alloc>::deque():mapSize_(0),map_(0){}

	template <typename T,typename Alloc>
	deque<T, Alloc>::deque(size_type n, const value_type& val = value_type())
	{
		deque();
		deque_aux(n, val, typename std::is_integral<size_type>::type()); 
	}
	template <typename T,typename Alloc>
	template <typename Iterator>
	deque<T, Alloc>::deque(Iterator first, Iterator last)
	{
		deque();
		deque_aux(first, last, typename std::is_integral<Iterator>::type());
	}
	template<typename T,typename Alloc>
	deque<T, Alloc>::deque(const deque<T, Alloc>& rhs)
	{
		mapSize_ = rhs.mapSize_;
		map_ = getANewMap(mapSize_);
		for (int i = 0; i + rhs.begin().mapIndex_ != rhs.mapSize_; ++i)
			for (int j = 0; j != getBuckSize(); ++j)
				map_[rhs.beg_.mapIndex_ + i][j] =
				rhs.map_[rhs.beg_.mapIndex_ + i][j];
		beg_.mapIndex_ = rhs.beg_.mapIndex_;
		end_.mapIndex_ = rhs.end_.mapIndex_;
		beg_.cur_ = map_[beg_.mapIndex_] +
			(rhs.beg_.cur_ - rhs.map_[rhs.beg_.mapIndex_]);
		end_.cur_ = map_[end_.mapIndex_] +
			(rhs.end_.cur_ - rhs.map_[rhs.end_.mapIndex_]);
		beg_.container_ = end_.container_ = this;
	}
	template <typename T,typename Alloc>
	void deque<T, Alloc>::reallocateAndCopy()
	{
		auto newMapSize = getNewMapSize(mapSize_);
		T** newMap = getANewMap(newMapSize);
		size_t startIndex = newMapSize / 4;
		for (int i = 0; i + beg_.mapIndex_ != mapSize_; ++i)
			for (int j = 0; j != getBuckSize(); ++j)
				newMap[startIndex + i][j] = map_[beg_.mapIndex_ + i][j];

		size_t n = beg_.cur_ - map_[beg_.mapIndex_];
		auto size = this->size();
		auto b = beg_, e = end_;
		clear();
		mapSize_ = newMapSize;
		map_ = newMap;
		beg_ = iterator(startIndex, newMap[startIndex] + n, this);
		end_ = beg_ + size;
	}

	template <typename T,typename Alloc>
	void deque<T, Alloc>::push_back(const value_type& val)
	{
		if (empty()) init();
		else if (back_full())
			reallocateAndCopy();
		MySTL::construct(end_, cur_, val);
		++end_;
	}
	template <typename T,typename Alloc>
	void deque<T, Alloc>::push_front(const value_type& val)
	{
		if (empty()) init();
		else if (front_full()) reallocateAndCopy();
		--beg_;
		MySTL::construct(beg_, cur_, val);
	}
	template <typename T,typename Alloc>
	void deque<T, Alloc>::pop_front()
	{
		dataAllocator::destroy(beg_.cur_);
		++beg_;
	}
	template <typename T,typename Alloc>
	void deque<T, Alloc>::pop_back()
	{
		--end_;
		dataAllocator::destroy(end_, cur_);
	}
	template <typename T,typename Alloc>
	void deque<T, Alloc> ::swap(deque<T, Alloc>& rhs)
	{
		MySTL::swap(mapSize_, rhs.mapSize_);
		MySTL::swap(map_, rhs.map_);
		beg_.swap(rhs.beg_);
		end_.swap(rhs.end_);
	}
	template <typename T,typename Alloc>
	bool operator==(const deque<T, Alloc>& lhs,
		const deque<T, Alloc>& rhs)
	{
		auto cit1 = lhs.begin(), cit2 = rhs.begin();
		for (; cit1 != lhs.end(), cit2 != rhs.end(); ++cit1, ++cit2)
			if (*cit1 != *cit2) return false;
		if (cit1 == lhs.end() && cit2 == rhs.end()) return true;
		return false;
	}
	template <typename T, typename Alloc>
	bool operator!=(const deque<T, Alloc>& lhs,
		const deque<T, Alloc>& rhs)	{	return !(lhs == rhs); }

	template <typename T,typename Alloc>
	void swap(deque<T,Alloc>& lhs,deque<T,Alloc>& rhs)
	{	lhs.swap(rhs);	}

	
}


#endif // !_DEQUE_IMPL_H