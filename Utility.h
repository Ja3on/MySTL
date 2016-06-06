#pragma once
#ifndef _UTILITY_H
#define _UTILITY_H

namespace MySTL
{
	//[swap]
	template <typename T>
	void swap(T& a, T& b)
	{
		auto temp = a;
		a = b;
		b = temp;
	}
	//[pair]
	template <typename T1,typename T2>
	class pair
	{
	public:
		T1 first;
		T2 second;
	public:
		typedef T1 first_type;
		typedef T2 second_type;
	public:
		//构造函数
		pair(){}
		pair(const first_type& a, const second_type& b);
		//copy构造函数
		template <typename U,typename V>
		pair(const pair<U, V>&);
		//asignment operator
		pair& operator=(const pair&);
		void swap(pair&);
	public:
		//operator reload
		template <typename T1,typename T2>
		friend bool operator==(const pair<T1, T2>&,const pair<T1,T2>&);

		template <typename T1,typename T2>
		friend bool operator!=(const pair<T1, T2>&, const pair<T1, T2>&);

		template <typename T1,typename T2>
		friend bool operator<(const pair<T1, T2>&, const pair<T1, T2>&);

		template <typename T1,typename T2>
		friend bool operator<=(const pair<T1, T2>&, const pair<T1, T2>&);

		template <typename T1,typename T2>
		friend bool operator>(const pair<T1, T2>&, const pair<T1, T2>&);

		template <typename T1,typename T2>
		friend bool operator>=(const pair<T1, T2>&, const pair<T1, T2>&);

		template <typename T1,typename T2>
		friend void swap(pair<T1, T2>&, pair<T1, T2>&);
	};
	template <typename T1,typename T2>
	pair<T1,T2>::pair(const first_type& a,const second_type& b)
		:first(a),second(b){}

	template <typename T1,typename T2>
	template <typename U,typename V>
	pair<T1, T2>::pair(const pair<U, V>& rhs)
		:first(rhs.first),second(rhs.second){}

	template <typename T1,typename T2>
	pair<T1, T2>& pair<T1, T2>::operator=(const pair<T1, T2>& rhs)
	{
		if (this != rhs)
		{
			first = rhs.first;
			second = rhs.second;
		}
		return *this;
	}

	template<typename T1, typename T2>
	bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return ((lhs.first == rhs.first) &&
			(lhs.second == rhs.second));
	}

	template<typename T1, typename T2>
	bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T1, typename T2>
	bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return ((lhs.first < rhs.first) ||
			(!(lhs.first < rhs.first) && (lhs.second < rhs.second)));
	}

	template<typename T1, typename T2>
	bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(rhs < lhs);
	}

	template<typename T1, typename T2>
	bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs<=rhs);
	}

	template<typename T1, typename T2>
	bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs < rhs);
	}

	template<typename T1, typename T2>
	void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs)
	{
		MySTL::swap(lhs.first, rhs.first);
		MySTL::swap(lhs.second, rhs.second);
	}
	////********** [make_pair] ******************************
	template<typename U,typename V>
	pair<U, V> make_pair(const U& u, const V& v)
	{
		return pair<U, V>(u, v);
	}

}

#endif // !_UTILITY_H