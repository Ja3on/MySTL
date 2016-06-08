#pragma once
#ifndef _BITMAP_H
#define _BITMAP_H

#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "Allocator.h"
#include "String.h"
#include "UninitializedFunctions.h"

namespace MySTL
{
	template <size_t N>
	class bitmap
	{
	public:
		typedef allocator<uint8_t> dataAllocator;
	private:
		uint8_t *start_;
		uint8_t *finish_;
		const size_t size_;
		const size_t sizeOfUINT8_;
		enum EAlign { ALIGN = 8 };
	public:
		bitmap();
		size_t count() const;
		size_t size() const { return size_; }
		bool test(size_t) const;
		bool any() const;
		bool none() const;
		bool all() const;

		bitmap& set();
		bitmap& set(size_t, bool val = true);
		bitmap& reset();
		bitmap& reset(size_t);
		bitmap& flip();
		bitmap& flip(size_t);

		string to_string() const;

		template <size_t N>
		friend std::ostream& operator<<(std::ostream&, const bitmap<N>&);
	private:
		size_t roundUp8(size_t);
		//将i的第n位置为val
		void setNthInInt8(uint8_t&, size_t, bool val);
		//取出i的第n位
		uint8_t getMask(uint8_t i, size_t nth) const { return (i&(1 << nth)); }
		//
		size_t getNth(size_t n) const { return (n / 8); }
		//
		size_t getMth(size_t n) const { return (n%EAlign::ALIGN); }
		void allocateAndFillN(size_t, uint8_t);
		void THROW(size_t) const;
	};
}
//#include "../MySTL/Detail/Bitmap.impl.h"
#endif // !_BITMAP_H