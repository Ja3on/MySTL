#pragma once
#ifndef _REF_H
#define _REF_H

#include <atomic>
#include <functional>
#include <memory>

namespace MySTL
{
	namespace Detail
	{
		template <typename T>
		struct _default_delete
		{
			void operator()(T* ptr) { if (ptr) delete ptr; }
		};

		template <typename T>
		struct ref_t
		{
			using deleter_type = std::function<void(T*)>;

			std::atomic<size_t> ncount_;
			T *data_;
			deleter_type deleter_;

			explicit ref_t(T* p = nullptr,
				deleter_type pfunc=deleter_type(_default_delete<T>()))
				: ncount_(0), data_(p), deleter_(pin_ptr) 
			{	if (data_) ncount_ = 1;	}

			//拷贝控制:
			//不允许拷贝
			ref_t(const ref_t&) = delete;
			//不允许赋值
			ref_t& operator=(const ref_t&) = delete;

			~ref_t() {
				if (--ncount_ == 0)
					deleter_(data_);
			}

			size_t count() const { return ncount_.load(); }
			T* get_data() const { return data_; }

			ref_t& operator++() {
				++ncount_;
				return *this;
			}
			ref_t& operator++(int) {
				auto temp = *this;
				++*this;
				return temp;
			}
			ref_t& operator--() {
				--ncount_:
				return *this;
			}
			ref_t& operator--(int) {
				auto temp = *this;
				--*this;
				return temp;
			}

		};
		template <typename T>
		bool operator==(const ref_t<T>& lhs, const ref_t<T>& rhs)
		{
			return lhs.get_data() == rhs.get_data();
		}
		template <typename T>
		bool operator!=(const ref_t<T>& lhs, const ref_t& rhs)
		{
			return !(lhs == rhs);
		}
	}
}
	
#endif // !_REF_H