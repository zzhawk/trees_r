// Copyright 2023 watson.wang
// https://github.com/zzhawk 

#include <memory>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <iostream>

template <typename T>
class binaryTree {

protected:
	struct node {
		node(T pt, node* left = nullptr, node* right = nullptr) :
			pt(pt), left(left), right(right) {}
		T pt;
		node* left;
		node* right;
	};

public:

	explicit binaryTree() {}

	explicit binaryTree(T val) {
		_root = new node(val);
	}

	virtual ~binaryTree(){
		destoryTree(_root);
	}

	binaryTree(const binaryTree& tar) {
		copyTree(&_root, tar._root);
	}

	binaryTree(binaryTree&& tar) noexcept {
		_root = tar._root;
		tar._root = nullptr;
	};

	binaryTree& operator = (const binaryTree& tar) {
		copyTree(&_root, tar._root);
		return *this;
	};

	binaryTree& operator = (binaryTree&& tar) noexcept {
		_root = tar._root;
		tar._root = nullptr;
		return *this;
	};

	virtual void append(T val) {
		if (val < _root->pt) appendNode(val, &_root->left);
		else appendNode(val, &_root->right);
	}

	std::pair<node*, node**> search(T val) {
		return searchNode(val, &_root);
	}

	virtual bool del(T val) {
		auto node = search(val);
		if (node.first != nullptr) {
			delelteNode(&node.first, node.second);
			return true;
		}
		else {
			return false;
		}
	}

protected:

	node* _root{};

	node* mostLeft(node* root) { 
		if (root->left != nullptr) {
			auto ans = mostLeft(root->left);
			if (root->left == ans) root->left = nullptr;
			return ans;
		}

		else return root;
	}

	node* mostRight(node* root) {
		if (root->right != nullptr) {
			auto ans = mostRight(root->right);
			if (root->right == ans) root->right = nullptr;
			return ans;
		}
		else return root;
	}

	std::pair<node*, node**> searchNode(T val, node** root) {
		std::pair<node*, node**> ans;

		if ((*root) == nullptr) return { nullptr, nullptr };

		if ((*root)->pt == val) return { (*root), root };
		else {
			auto ans = searchNode(val, &(*root)->left);
			if (ans.first == nullptr) {
				return searchNode(val, &(*root)->right);
			}
			else {
				return ans; 
			}
		}
	}


	void destoryTree(node* root) {
		if (root == nullptr) return;
		destoryTree(root->left);
		destoryTree(root->right);
		delete root;
	}

	virtual void appendNode(T val, node** root) {
		if (*root == nullptr) {
			*root = new node(val);
			return;
		}
		if (val < (*root)->pt) appendNode(val, &(*root)->left);
		else appendNode(val, &(*root)->right);
	}

	node* copyTree(node** root, node* tar) {
		if (tar == nullptr) {
			return nullptr;
		}
		else {
			*root = new node(tar->pt);
			(*root)->left = copyTree(&(*root)->left, tar->left);
			(*root)->right = copyTree(&(*root)->right, tar->right);
		}

		return *root;
	}

	void delelteNode(node** nd, node** from) {

		if (((*nd)->left == nullptr) && ((*nd)->right == nullptr)) {
			(*from) = nullptr;
			delete((*nd));
		}
		else if ((*nd)->right == nullptr) {
			*from = (*nd)->left;
			delete(nd);
		}
		else if ((*nd)->left == nullptr) {
			*from = (*nd)->right;
			delete(*nd);
		}
		else {
			/* TODO: pick proper branch for balance */

			auto candi = mostRight((*nd)->left);

			candi->left = (*nd)->left;
			candi->right = (*nd)->right;

			if ((*nd) == _root) {
				_root = candi;
			}

			(*from) = candi;
			delete(*nd);
		}
	}
};

template <typename T>
class avl : public binaryTree<T> {
public:
	typedef binaryTree<T>::node bTreeNode;

	explicit avl() {}

	explicit avl(T val) : binaryTree<T>::binaryTree(val) {}

	avl(const avl& tar) {
		this->copyTree(&this->_root, tar._root);
	}

	avl(avl&& tar) noexcept {
		this->_root = tar._root;
		tar._root = nullptr;
	};

	avl& operator = (const avl& tar) {
		this->copyTree(&this->_root, tar._root);
		return *this;
	};

	avl& operator = (avl&& tar) noexcept {
		this->_root = tar._root;
		tar._root = nullptr;
		return *this;
	};

	void append(T val) override {
		start = nullptr;
		while (!operation.empty()) operation.pop();

		if (val < this->_root->pt) this->appendNode(val, &this->_root->left);
		else this->appendNode(val, &this->_root->right);

		getInstruction(false, val, this->_root);
		rebalance();
	}

	bool del(T val) override {
		auto node = this->search(val);
		if (node.first != nullptr) {
			this->delelteNode(&(node.first), node.second);
			getInstruction(false, val, this->_root);
			rebalance();
			return true;
		}
		else {
			return false;
		}
	}

	void print(void) {
		std::queue<bTreeNode*> q;
		int cntr = 1;
		int max = 1;
		q.push(this->_root);
		auto height = getHeight(this->_root);
		int len = static_cast<int>(std::pow(6, height - 1)) >> 1;

		while (!q.empty()) {
			for (int i = 0; i < len / 6; ++i) std::cout << " ";
			if (q.front() != nullptr) std::cout << q.front()->pt;
			else std::cout << " ";
			for (int i = 0; i < len / 6; ++i) std::cout << " ";
			cntr--;

			if (q.front() != nullptr) q.push(q.front()->left);
			if (q.front() != nullptr) q.push(q.front()->right);
			q.pop();

			if (cntr == 0) {
				std::cout << std::endl;
				cntr = q.size();
				max = cntr;
				len = len >> 1;
			}
		}
	}

private:
	bTreeNode** start = nullptr;
	std::stack<std::pair<bTreeNode*, char>> operation;

	int getBalanceFactor(bTreeNode* p) {
		return getHeight(p->left) - getHeight(p->right);
	}

	void getInstruction(bool rec, T val, bTreeNode* root)
	{
		if (root == nullptr) {
			return;
		}

		auto factor = getBalanceFactor(root);

		if ((rec)||(factor > 1)) {
			if (val < root->pt) {
				operation.push({ root, 'r' });
				getInstruction(true, val, root->left);
				if(start == nullptr) start = &root->left;
			}
			else if (val > root->pt) getInstruction(false, val, root->right);
			else {}
		}
		else if ((rec) || (factor < -1)) {
			if (val < root->pt) getInstruction(false, val, root->left);
			else if (val > root->pt) {
				operation.push({ root, 'l' });
				getInstruction(true, val, root->right);
				if (start == nullptr) start = &root->right;
			}
			else {}
		}
		else {
			if (val < root->pt) getInstruction(false, val, root->left);
			else if (val > root->pt) getInstruction(false, val, root->right);
			else {}
		}
	}


	void rebalance(void) {
		if (operation.size() < 2) return;
		while (operation.size() > 2) operation.pop();

		auto op2 = operation.top(); operation.pop();
		auto op1 = operation.top(); operation.pop();
		bTreeNode* ans = nullptr;
		if (op2.second == op1.second) {
			if (op1.second == 'l') ans = L_Rotate(&op1.first);
			else if (op1.second == 'r') ans = R_Rotate(&op1.first);
		}
		else {
			if (op2.second == 'l') {
				op1.first->left = L_Rotate(&op2.first);
				ans = R_Rotate(&op1.first);
			}
			else if (op2.second == 'r') {
				op1.first->right = R_Rotate(&op2.first);
				ans = L_Rotate(&op1.first);
			}
		}

		if (this->_root == op1.first) this->_root = ans;
		else *start = ans;
	}

	bTreeNode* R_Rotate(bTreeNode** root) {

		bTreeNode* grand = (*root);
		bTreeNode* parent = (*root)->left;
		bTreeNode* bro = parent->right;

		parent->right = grand;
		grand->left = bro;

		return parent;
	}

	bTreeNode* L_Rotate(bTreeNode** root) {

		bTreeNode* grand = (*root);
		bTreeNode* parent = (*root)->right;
		bTreeNode* bro = parent->left;

		parent->left = grand;
		grand->right = bro;

		return parent;
	}

	int getHeight(bTreeNode* p) {
		if (p == nullptr) return 0;

		auto l = getHeight(p->left);
		auto r = getHeight(p->right);

		auto o = std::max(l, r);
		return ++o;
	}
};
