#include "../../MySTL/TrieTree.h"

namespace MySTL
{
	typedef trie_tree::size_type size_type;
	trie_tree::trie_tree() :__root(new trie_node), __size(0) {}
	trie_tree::~trie_tree()
	{
		if (__root)
		{
			__root->map_childs.clear();
			delete __root;
		}
	}
	bool trie_tree::empty() const { return __size == 0; }
	size_type trie_tree::size() const { return __size; }
	bool trie_tree::is_existed(const string& word) const
	{
		if (word.empty()) return false;
		auto root = get_root();
		auto result = root->map_childs.find(word[0]);
		if (result == root->map_childs.end()) return false;
		else return _is_existed(word, result->second);
	}
	bool trie_tree::_is_existed(const string& word, const node_ptr& up) const
	{
		if (word.size() == 1) return up->is_a_word;
		char ch = word[1];
		auto result = up->map_childs.find(ch);
		if (result == up->map_childs.end()) return false;
		return _is_existed(word.substr(1), result->second);
	}
	trie_tree::node_ptr trie_tree::make_node(char ch, bool is)
	{
		return std::make_unique<trie_node>(ch, is);
	}
	bool trie_tree::insert(const string& word)
	{
		if (is_existed(word)) return true;
		if (word.empty()) return false;
		char ch = word[0];
		auto root = get_root();
		auto result = root->map_childs.find(ch);
		if (result != root->map_childs.end())
			return _insert(word.substr(1), result->second);
		else
		{
			auto is_a_word = (word.size() == 1 ? true : false);
			auto node = make_node(ch, is_a_word);
			__root->map_childs[ch] = std::move(node);
			return _insert(word.substr(1), __root->map_childs[ch]);
		}
	}
	bool trie_tree::_insert(const string& word, const node_ptr& up)
	{

	}
}