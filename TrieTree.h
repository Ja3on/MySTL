#pragma once
#ifndef _TRIETREE_H
#define _TRIETREE_H

#include "../MySTL/Iterator.h"
#include "../MySTL/Vector.h"
#include "../MySTL/String.h"

#include <utility>
#include <memory>
#include <map>
#include <iostream>

namespace MySTL
{
	class trie_tree
	{
	private:
		struct trie_node
		{
			char data;
			bool is_a_word;
			std::map<char, std::unique_ptr<trie_node>> map_childs;
			trie_node():data('\0'),is_a_word(false){}
			trie_node(char ch,bool is):data(ch),is_a_word(is){}
		};
		typedef std::unique_ptr<trie_node> node_ptr;
	public:
		typedef string value_type;
		typedef size_t size_type;
	private:
		trie_node* __root;
		size_type __size;
	public:
		trie_tree();
		~trie_tree();
		trie_tree(const trie_tree&) = delete;
		trie_tree& operator=(const trie_tree&) = delete;

		bool empty() const;
		size_type size() const;

		vector<string> get_word_by_prefix(const string& prefix) const;
		void print_tree(std::ostream& os = std::cout) const;
		bool insert(const string&);
		bool is_existed(const string&) const;
	private:
		node_ptr make_node(char ch, bool is_a_word);
		inline trie_node* get_root() const { return __root; }
		void _get_word_by_prefix(const string& prefix, const node_ptr& up,
			const string& real_prefix, vector<string>& words) const;
		void __get_word_by_prefix(const node_ptr& up, string& word,
			const string& prefix, vector<string>& words) const;
		void _print_tree(std::ostream& os, const node_ptr& up, string) const;
		bool _insert(const string& word, const node_ptr&);
		bool _is_existed(const string& word, const node_ptr&) const;
	};
}

#endif // !_TRIETREE_H