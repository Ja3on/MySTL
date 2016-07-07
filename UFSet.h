#pragma once
#ifndef _UFSET_H
#define _UFSET_H

#include <cstring>

namespace MySTL
{
	template <size_t N>
	class uf_set
	{
	public:
		uf_set();

		int Find(int index);
		void Union(int index, int index2);
		void Clear();

	private:
		int parent[N];
	};

	template <size_t N>
	int uf_set<N>::Find(int index)
	{
		auto root = index;
		for (; parent[root] >= 0; root = parent[root]) {}
		while (root != index)
		{//Â·¾¶Ñ¹Ëõ
			auto t = parent[index];
			parent[index] = root;
			index = t;
		}
		return t;
	}
	template <size_t N>
	void uf_set<N>::Union(int index, int index2)
	{
		auto root1 = Find(index), root2 = Find(index2);
		auto total_nodes = parent[root1] + parent[root2];
		if (parent[root1] > parent[root2])
		{
			parent[root1] = root2;
			parent[root2] = total_nodes;
		}
		else
		{
			parent[root2] = root1;
			parent[root1] = total_nodes;
		}
	}
	template <size_t N>
	void uf_set<N>::Clear()
	{
		std::memset(parent, -1, sizeof(int)*N);
	}

}

#endif // !_UFSET_H