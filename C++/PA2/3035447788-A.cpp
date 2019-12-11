#include <iostream>
#include <math.h>
#include <algorithm>
using namespace std;

// Definition for a binary tree node.
struct TreeNode {
    int val;
    int max; // maximum value of the subtree from this node
    int min; // minimum value of the subtree from this node
    int num; // number of the node
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), max(NULL), min(NULL), left(NULL), right(NULL), num(NULL) {}
    TreeNode() : val(NULL), max(NULL), min(NULL), left(NULL), right(NULL), num(NULL){}
};

bool is_BST(TreeNode* root){
    if (root->left == NULL&&root->right == NULL) {
        return true;
    }
    if (is_BST(root->left) && is_BST(root->right) ) {
                if(root->left->max==0)
                {
                        if(root->right->min==0)
                                return true;
                        else if(root->right->min> root->val)
                                return true;
                        else return false;
                }
                else{
                        if(root->left->max < root->val)
                        {
                        if(root->right->min==0)
                                return true;
                        else if(root->right->min> root->val)
                                return true;
                        else return false;
                        }
                        else
                                return false;
                        
                }
                        
    }
    else {
        return false;
    }
    
}

void buildnode(TreeNode* root, int A[], int n) {
    if (2 * root->num <= n) {
        root->left = new TreeNode;
        root->left->val = A[2 * root->num - 1];
        root->left->num = 2 * root->num;

        root->right = new TreeNode;
        root->right->val = A[2 * root->num];
        root->right->num = 2 * root->num + 1;
    }
    else {
        return;
    }

    buildnode(root->left, A, n);
    buildnode(root->right, A, n);
}

int fillmax(TreeNode* root) {
    if (root->right == NULL) {
        root->max = root->val;
        return root->max;
    }
    else {
        root->max = max(max(fillmax(root->right), root->val), fillmax(root->left));
        return root->max;
    }

}
int fillmin(TreeNode* root) {
    if (root->left == NULL) {
        root->min = root->val;
        return root->min;
    }
    else {
        if (root->left->val == 0 && root->right->val != 0) {
            root->min = min(root->val, fillmin(root->right));
        }
        if (root->left->val != 0 && root->right -> val == 0) {
            root->min = min(root->val, fillmin(root->left));
        }
        if (root->left->val == 0 && root->right -> val == 0) {
            root->min = root->val;
        }


　　　　　　if (root->left->val != 0 && root->right -> val != 0) {
            root->min = min(min(root->val, fillmin(root->left)), fillmin(root->right));
        }

return root->min;
    }

}

void printtree(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    cout << root->min<<endl;
    printtree(root->left);
    printtree(root->right);
    if (root->left == NULL || root->right == NULL) {
        return;
    }
}


void SolveD() {
    TreeNode* root = new TreeNode;
    int n;
    cin >> n;
    int* valarray = new int[n];
    int value;
    for (int i = 0; i <= (n - 1); i++) {
        cin >> value;
        valarray[i] = value;
    }
    root->val = valarray[0];
    root->num = 1;
    buildnode(root, valarray, n);
    fillmin(root);
    fillmax(root);
    //printtree(root);
    if (is_BST(root)) {
        cout << "true";
        return;
    }
    else {
        cout << "false";
        return;    
    }
    
    return;
}

int main() {
    SolveD();
        system("pause");
    return 0;
}