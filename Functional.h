#pragma once
#ifndef _FUNCTIONAL_H
#define _FUNCTIONAL_H

namespace MySTL
{
	template <typename T>
	struct less
	{
		typedef T first_argument_type;
		typedef T second_argument_type;
		typedef bool result_type;
		result_type operator()(const first_argument_type& lhs,
			const second_argument_type& rhs)
		{
			return lhs < rhs;
		}
	};
	template <typename T>
	struct equal_to
	{
		typedef T first_argument_type;
		typedef T second_argument_type;
		typedef bool result_type;
		result_type operator()(const first_argument_type& lhs,
			const second_argument_type& rhs)
		{
			return lhs == rhs;
		}
	};
}

#endif // !_FUNCTIONAL_H