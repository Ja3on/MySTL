#pragma once
#ifndef _COWPTR_H
#define _COWPTR_H

#include "../MySTL/Memory.h"

namespace MySTL
{
	template <typename T>
	class cow_ptr;
	namespace Detail
	{
		template <typename T>
		class proxy
		{
		public:
			explicit proxy(cow_ptr<T>* const cp):cp_(cp){}
			proxy(const proxy& rhs) = default;
			proxy& operator = (const proxy&) = default;

			const T& operator*() const;
			T& operator*();
			const T* operator->() const;
			T* operator->();
			operator T() const;
		private:
			cow_ptr<T> *cp_;
		};
	}
	template <typename T>
	class cow_ptr
	{
	public:
		typedef T element_type;
		template <typename Type> 
		using shared_ptr = MySTL::shared_ptr<Type>;
		template <typename T_>
		using proxy = MySTL::Detail::proxy<T_>;
		template <typename T_>
		using make_shared = MySTL::make_shared<T_>;
	public:
		explicit cow_ptr(T* p = nullptr);
			//:ptr_(MySTL::make_shared<T>(p)){}
		template <typename D>
		cow_ptr(T* p, D del);
			//: ptr_(MySTL::make_shared<T,D>(p, del)){}
		//¿½±´¿ØÖÆ
		cow_ptr(const cow_ptr&);
		cow_ptr& operaotr = (const cow_ptr&);
		//~cow_ptr();
		const proxy<element_type> operator*() const;
		proxy<element_type> operator*();
		const proxy<element_type> operator->() const;
		proxy<element_type> operator->();

		const element_type* get() const;
		element_type* get();

		operator bool() const;
	private:
		shared_ptr<element_type> ptr_;

	public:
		template <typename T1,typename T2>
		friend bool operator==(const cow_ptr<T1>&, const cow_ptr<T2>&) const;
		template <typename T>
		friend bool operator==(const cow_ptr<T>&, nullptr_t) const;
		template <typename T>
		friend bool operator==(nullptr_t, const cow_ptr<T>&) const;

		template <typename T1, typename T2>
		friend bool operator!=(const cow_ptr<T1>&, const cow_ptr<T2>&) const;
		template <typename T>
		friend bool operator!=(const cow_ptr<T>&, nullptr_t) const;
		template <typename T>
		friend bool operator!=(nullptr_t, const cow_ptr<T>&) const;
	};
	template <typename T,typename...Arges>
	cow_ptr<T> make_cow(Arges...args)
	{
		return cow_ptr<T>(new T(std::forward<Arges>(args)...));
	}
}
#include "../MySTL/Detail/COWPtr.impl.h"
#endif // !_COWPTR_H