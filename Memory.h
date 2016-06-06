#pragma once
#ifndef _MEMORY_H
#define _MEMORY_H

#include <utility>
#include "../MySTL/Detail/Ref.h"

namespace MySTL
{
	template <typename T>
	class cow_ptr;

	template <typename T>
	struct default_delete
	{
		void operator()(T* ptr) { if (ptr) delete ptr; }
	};

	template <typename T>
	struct default_delete<T[]>
	{
		void operator()(T* ptr) { if (ptr) delete[] ptr; }
	};

	template <typename T,typename D=default_delete<T>>
	class unique_ptr
	{
	public:
		typedef T element_type;
		typedef element_type* pointer;
		typedef D deleter_type;
	public:

		explicit unique_ptr(T* p=nullptr):data_(p){}
		unique_ptr(T* data,D* del):data_(data),deleter(del){}

		//¿½±´¿ØÖÆ
		unique_ptr(const unique_ptr& rhs) = delete;
		unique_ptr& operator=(const unique_ptr& rhs) = delete;

		~unique_ptr() { clean(); }

		const pointer get() const { return data_; }
		pointer get() { return data_; }
		const deleter_type get_deleter() const { return deleter; }
		deleter_type get_deleter() { return deleter; }

		operator bool() const { return get() != nullptr; }

		void swap(unique_ptr& up) { MySTL::swap(data_, up.data_); }

		pointer release() {
			T* p = nullptr;
			MySTL::swap(p, data_);
			return p;
		}
		void reset(pointer p=pointer())
		{
			clean();
			data_ = p;
		}

		const element_type operator*() const { return *get(); }
		element_type operator*() { return *get(); }
		const pointer operator->() const { return get(); }
		pointer operator->() { return get(); }

	private:
		inline void clean() {
			deleter(data_);
			data_ = nullptr;
		}
	private:
		element_type* data_;
		deleter_type deleter;
		
	};
	template <typename T>
	void swap(unique_ptr<T>& lhs, unique_ptr<T>& rhs) {
		lhs.swap(rhs);
	}

	template <typename T1,typename D1,
	typename T2,typename D2>
	bool operator==(const unique_ptr<T1,D1>& lhs,
		const unique_ptr<T2,D2>& rhs) const
	{	return lhs.get() == rhs.get();	}

	template <typename T>
	bool operator==(const unique_ptr<T>& lhs,
		nullptr_t p) const
	{return lhs.get() == p;}

	template <typename T>
	bool operator==(nullptr_t p,
		const unique_ptr<T>& rhs) const 
	{ return rhs.get() == p;}

	template <typename T1, typename D1,
		typename T2,typename D2 >
		bool operator !=(const unique_ptr<T1, D1>& lhs,
			const unique_ptr<T2, D2>& rhs) const
	{ return lhs.get() != rhs.get();}

	template <typename T>
	bool operator !=(const unique_ptr<T>& lhs,
		nullptr_t p) const
	{ return lhs.get() != p; }
	
	template <typename T>
	bool operator != (nullptr_t p,
		const unique_ptr<T>& rhs)
	{ return rhs != p; }

	template <typename T,typename...Arges>
	unique_ptr<T> make_unique(Arges...args)
	{
		return unique_ptr<T>(new T(std::forward<Arges>(args)...));
	}

	template <typename T>
	class shared_ptr
	{
	public:
		typedef T element_type;
		typedef T* pointer;
	private:
		template <typename Type>
		using ref_t = MySTL::Detail::ref_t<Type>;
	public:
		explicit shared_ptr(T* p=nullptr)
			:ref_(new ref_t<T>(p)){}
		template <typename D>
		shared_ptr(T* p=nullptr,D del)
			:ref_(new ref_t<T>(p,del)){}
		//¿½±´¿ØÖÆ
		shared_ptr(const shared_ptr& sp)
		{
			copy_ref(sp.ref_);
		}
		shared_ptr& operator=(const shared_ptr& rhs)
		{
			if (&rhs != this)
			{
				decrease_ref();
				copy_ref(rhs.ref_);
			}
			return *this;
		}
		~shared_ptr() {	decrease_ref();	}
		//²Ù×÷·ûÖØÔØ
		const pointer get() const { return ref_->get_data(); }
		pointer get() { return ref_->get_data(); }
		size_t use_count() const { return ref_->count(); }

		const element_type& operator*() const { return *(get()); }
		element_type& operator*() { return *get(); }
		const pointer operator->() const { return get(); }
		pointer operator->() { return get(); }

		operator bool() const { return get() != nullptr; }

	private:
		void decrease_ref()
		{
			if (ref_->get_data())
				--(*ref_);
			if (ref_->ncount_ == 0)
				delete ref_;
		}
		void copy_ref(ref_t<T>* r)
		{
			ref_ = r;
			++(*ref_);
		}
	private:
		//element_type data_;
		ref_t<T>* ref_;
	public:
		template <typename _T>
		friend class cow_ptr;
	};
	template <typename T1,typename T2>
	bool operator==(const shared_ptr<T1>& lhs,
		const shared_ptr<T2>& rhs) const 
	{	return lhs.get() == rhs.get();	}

	template <typename T>
	bool operator==(const shared_ptr<T>& lhs,
		nullptr_t p) const 
	{ return lhs.get() == p; }

	template <typename T>
	bool operator==(nullptr_t p,
		const shared_ptr<T>& rhs) const
	{	return rhs.get() == p;	}

	template <typename T1,typename T2>
	bool operator!=(const shared_ptr<T1>& lhs,
		const shared_ptr<T2>& rhs) const
	{	return lhs.get() != rhs.get();	}

	template <typename T>
	bool operator!=(const shared_ptr<T>& lhs,
		nullptr_t p) const 
	{	return lhs.get() != p;	}

	template <typename T>
	bool operator!=(nullptr_t p,
		const shared_ptr<T>& rhs) const
	{	return rhs.get() != p;	}

	template <typename T,typename...Arges>
	shared_ptr<T> make_shared(Arges...args)
	{
		return shared_ptr<T>(new T(std::forward<Arges>(arges)...));
	}
}

#endif // !_MEMORY_H