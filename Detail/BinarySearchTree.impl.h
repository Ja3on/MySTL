#pragma once
#ifndef _BINARYSEARCHTREE_IMPL_H
#define _BINARYSEARCHTREE_IMPL_H

namespace MySTL
{
	namespace Detail
	{
		template <typename T>
		bst_iter<T>::bst_iter(const T* ptr, cntrPtr container)
			:ptr_(ptr), container_(container)
		{
			if (!ptr_) return;
			auto temp = container_->root_;
			while (temp && temp != ptr_ &&temp->data_ != ptr_->data_)
			{
				parent_.push(temp);
				if (temp->data_ < ptr_->data_)
				{
					visited_.insert(temp);
					temp = temp->right;
				}
				else if (temp->data_ > ptr_->data_)
					temp = temp->left_;
			}
		}

		template <typename T>
		bst_iter<T>& bst_iter<T>::operator++()
		{
			visited_.insert(ptr_);
			if (ptr_->right_)//节点有右子树
			{
				parent_.push(ptr_);
				ptr_ = ptr_->right;
				while (ptr_&&ptr_->left)//ptr_右子节点的左叶子节点
										//
				{
					parent_.push(ptr_);
					ptr_ = ptr_->left;
				}
			}
			else
			{
				ptr_ = 0;
				while (!parent_.empty())
				{
					ptr_ = parent_.top();
					parent_.pop();
					if (visited_.count(ptr_) == 0)
					{
						visited_.insert(ptr_);
						break;
					}
					ptr_ = 0;
				}
			}
			return *this;
		}
		template <typename T>
		bst_iter<T> bst_iter<T>::operator++(int)
		{
			auto temp = *this;
			++(*this);
			return temp;
		}

		template <typename T>
		bool operator ==(const bst_iter<T>& lhs, const bst_iter<T>& rhs)
		{
			return lhs.ptr_ == rhs.ptr_;
		}
		template <typename T>
		bool operator!=(const bst_iter<T>& lhs, const bst_iter<T>& rhs)
		{
			return !(lhs == rhs);
		}
	}

	template <typename T>
	binary_search_tree<T>::~binary_search_tree()
	{
		deallocateAllNodes(root_);
	}

	template <typename T>
	void binary_search_tree<T>::deallocateAllNodes(node* ptr)
	{
		if (ptr)
		{
			deallocateAllNodes(ptr->left_);
			deallocateAllNodes(ptr->right_);
			nodeAllocator::destroy(ptr);
			nodeAllocator::deallocate(ptr);
		}
	}
	template <typename T>
	size_t binary_search_tree<T>::height_aux(node* p) const
	{
		MySTL::queue<node*> q/*存放下一层node**/, level/*存放当前层node**/;
		size_t nlevel = 0;
		if (p != 0)
		{
			level.push(p);
			++nlevel;
			while (!(q.empty() && level.empty()))
			{
				if (level.empty())
				{
					++nlevel;
					while (!q.empty())
					{
						level.push(q.front());
						q.pop();
					}
				}
				auto temp = level.front();
				level.pop();
				if (temp->left_ != 0) q.push(temp->left_);
				if (temp->right_ != 0) q.push(temp->righ_);
			}
			return nlevel;
		}
	}

	template <typename T>
	void binary_search_tree<T>::erase_elem(const T& val, node*& ptr)
	{
		if (ptr == nullptr) return;
		if (val != ptr->data_)
		{
			if (val < ptr->data_) return erase_elem(val, ptr->left_);
			if (val > ptr->data_) return erase_elem(val, ptr->right_);
		}
		else
		{
			if (ptr->left_ != nullptr && ptr->right_ != nullptr)
			{
				size_t choose = size_ % 2;
				auto pos = (choose == 0 ?
					const_cast<node*>(find_min_aux(ptr_->right_).ptr_) :
					const_cast<node*>(find_max_aux(ptr_->left_).ptr_));
				ptr_->data_ = pos->data_;
				return (choose == 0 ? erase_elem(pos->data_, ptr->right_) :
					erase_elem(pos->data_, ptr->left));
			}
			else
			{
				auto temp = ptr;
				if (ptr->left == 0) ptr = ptr->right_;
				else ptr = ptr->left_;
				nodeAllocator::destroy(temp);
				nodeAllocator::deallocate(temp);
				--size_;
			}

		}
	}

	template <typename T>
	void binary_search_tree<T>::erase(const T& val)
	{
		erase_elem(val, root_);
	}

	template <typename T>
	void binary_search_tree<T>::insert_elem(const T& val, node*& ptr)
	{
		if (ptr == 0)
		{
			ptr = nodeAllocator::allocate();
			nodeAllocator::construct(ptr);
			ptr->data_ = val;
			ptr->left_ = ptr->right_ = nullptr;
			++size_;
		}
		else
		{
			if (val < ptr->data_)
				return insert_elem(val, ptr->left_);
			else if (val > ptr->data_)
				return insert_elem(val, ptr->right_);
		}
	}
	template <typename T>
	void binary_search_tree<T>::insert(const T& val)
	{
		insert_elem(val, root_);
	}
	template <typename T>
	template <typename Iterator>
	void binary_search_tree<T>::insert(Iterator first, Iterator last)
	{
		for (; first != last; ++first)
			insert(*first);
	}
	template <typename T>
	void binary_search_tree<T>::print_levelorder(const string& delim,
		std::ostream& os) const
	{
		auto temp = root_;
		if (temp != 0)
		{
			std::deque<node*> q;
			q.push_back(temp);
			while (!q.empty())
			{
				temp = q.front();
				q.pop_front();
				if (temp->left_ != 0) q.push_back(temp->left_);
				if (temp->right_ != 0) q.push_back(temp->right_);
				os << temp->data_ << delim;
			}
		}
	}
	template <typename T>
	void binary_search_tree<T>::print_preorder_aux(const string& delim,
		std::ostream& os, const node* ptr) const
	{
		if (ptr)
		{
			os << ptr->data_ << delim;
			print_preorder_aux(delim, os, ptr->left_);
			print_preorder_aux(delim, os, ptr->right_);
		}
	}
	template <typename T>
	void binary_search_tree<T>::print_inorder_aux(const string& delim,
		std::ostream& os, const node* ptr) const
	{
		if (ptr)
		{
			print_inorder_aux(delim, os, ptr->left_);
			os << ptr->data_ << delim;
			print_inorder_aux(delim, os, ptr->right_);
		}
	}
	template <typename T>
	void binary_search_tree<T>::print_postorder_aux(const string& delim,
		std::ostream& os, const node* ptr) const
	{
		if (ptr)
		{
			print_postorder_aux(delim, os, ptr->left_);
			print_postorder_aux(delim, os, ptr->right_);
			os << ptr->data_ << delim;
		}
	}
	template <typename T>
	void binary_search_tree<T>::print_preorder(const string& delim,
		std::ostream& os) const
	{
		print_preorder_aux(delim, os, root_);
	}
	template <typename T>
	void binary_search_tree<T>::print_inorder(const string& delim,
		std::ostream& os) const
	{
		print_inorder_aux(delim, os, root_);
	}
	template <typename T>
	void binary_search_tree<T>::print_postorder(const string& delim,
		std::ostream& os) const
	{
		print_postorder_aux(delim, os, root_);
	}
	template <typename T>
	typename binary_search_tree<T>::const_iterator binary_search_tree<T>::
		find_min_aux(const node* ptr) const
	{
		while (ptr&&ptr->left_ != 0)
		{
			ptr = ptr->left_;
		}
		return const_iterator(ptr,this);
	}
	template <typename T>
	typename binary_search_tree<T>::const_iterator binary_search_tree<T>::
		find_min() const
	{
		return find_min_aux(root_);
	}
	template <typename T>
	typename binary_search_tree<T>::const_iterator binary_search_tree<T>::
		find_max_aux(const node* ptr) const
	{
		while (ptr && ptr->right_ != 0)
			ptr = ptr->right_;
		return const_iterator(ptr, this);
	}
	template <typename T>
	typename binary_search_tree<T>::const_iterator binary_search_tree<T>::
		find_max() const
	{
		return find_max_aux(root_);
	}
	template <typename T>
	typename binary_search_tree<T>::const_iterator binary_search_tree<T>::
		find_aux(const T& val, const node* ptr) const
	{
		while (ptr)
		{
			if (val == ptr->data_) break;
			else if (val < ptr->data_) ptr = ptr->left_;
			else ptr = ptr->right_;
		}
		return const_iterator(ptr, this);
	}
	template <typename T>
	typename binary_search_tree<T>::const_iterator binary_search_tree<T>::
		find(const T& val) const
	{
		return find_aux(val, root_);
	}

}

#endif // !_BINARYSEARCHTREE_IMPL_H