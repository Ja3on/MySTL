#pragma once
#ifndef _BINARYSEARCHTREE_IMPL_H
#define _BINARYSEARCHTREE_IMPL_H

namespace MySTL
{
	namespace Detail
	{
		template <typename T>
		bst_iter<T>::bst_iter(const T* ptr,cntrPtr container)
			:ptr_(ptr),container_(container)
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
		bool operator!=(const bst_iter<T>& lhs,const bst_iter<T>& rhs)
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

}

#endif // !_BINARYSEARCHTREE_IMPL_H