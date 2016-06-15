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
		if (word.empty())
		{
			++__size;
			up->is_a_word = true;
			return true;
		}
		char ch = word[0];
		auto res = up->map_childs.find(ch);
		if (res != up->map_childs.end())
			return _insert(word.substr(1), res->second);
		else
		{
			auto is_a_word = (word.size() == 1 ? true : false);
			auto node = make_node(ch, is_a_word);
			up->map_childs[ch] = std::move(node);
			return _insert(word.substr(1), up->map_childs[ch]);
		}
	}
	void trie_tree::print_tree(std::ostream& os) const
	{
		auto root = get_root();
		if (root == nullptr)
			os << "the trie_tree is empty!" << std::endl;
		for (auto cit = root->map_childs.cbegin(); 
			cit != root->map_childs.cend();	++cit)
			_print_tree(os, cit->second, string());
	}
	void trie_tree::_print_tree(std::ostream& os, const node_ptr& up,
		string word) const
	{
		word += up->data;
		if (up->is_a_word)
			os << word << std::endl;
		for (auto cit = up->map_childs.cbegin();
			cit != up->map_childs.cend(); ++cit)
			_print_tree(os, cit->second, word);
	}
	vector<string> trie_tree::get_word_by_prefix(const string& prefix) const
	{
		vector<string> words;
		auto root = get_root();
		if (root == nullptr || prefix.size() == 0)
			return words;
		char ch = prefix[0];
		auto result = root->map_childs.find(ch);
		if (result != root->map_childs.end())
			_get_word_by_prefix(prefix, result->second, prefix, words);
		return words;
	}
	void trie_tree::_get_word_by_prefix(const string& prefix, const node_ptr& up,
		const string& real_prefix, vector<string>& words) const
	{
		if (prefix.size() == 1)
		{
			if (up->is_a_word)
				words.push_back(real_prefix);
			for (auto cit = up->map_childs.cbegin(); cit != up->map_childs.cend();
				++cit)
				__get_word_by_prefix(cit->second, string(), real_prefix, words);
		}
		else
		{
			char ch = prefix[1];
			auto res = up->map_childs.find(ch);
			if (res != up->map_childs.end())
				_get_word_by_prefix(prefix.substr(1), res->second, 
					real_prefix, words);
		}
	}

	void trie_tree::__get_word_by_prefix(const node_ptr& up, string& word,
		const string& prefix, vector<string>& words) const
	{
		word += up->data;
		if (up->is_a_word)
			words.push_back(prefix + word);
		for (auto cit = up->map_childs.cbegin(); cit != up->map_childs.cend();
			++cit)
			__get_word_by_prefix(cit->second, string(word), prefix, words);
	}
}