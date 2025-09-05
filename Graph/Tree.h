#pragma once

#include <iostream>

class TreeNode {
public:
	TreeNode() {}
	TreeNode(int _val) : val(_val) {}
	TreeNode(int _val, TreeNode* _left, TreeNode* _right) : val(_val), left(_left), right(_right) {}
	~TreeNode() = default;

	void inorder(TreeNode* root) {
		if (root == nullptr) {
			return;
		}
		inorder(root->left);
		std::cout << root->val;
		inorder(root->right);

		return;
	}

private:
	int val;
	TreeNode* left;
	TreeNode* right;
};