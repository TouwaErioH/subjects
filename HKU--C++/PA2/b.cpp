#include <stdio.h>
#include <assert.h>
#include<iostream>
#include <algorithm>
#include <algorithm>
using namespace std;
int cnt=0;
int max(int a, int b)
{
	return (a > b ? a : b);
}
struct node
{
    int key;
    int height;
    int size; //tree node 个数
    node *left, *right;
    /*
    node(int x) : key(x), height(1), size(1), left(NULL), right(NULL) {}
        node() : key(NULL), height(NULL), size(NULL), left(NULL), right(NULL){}
        */
       node(int k)
    {
        key = k;
        height = 1;
        size = 1;
        left = right = 0;
    }
};

int height(node* r)
{
    return r ? r->height : 0;
}

void update_height(node* root)
{
root->height = max(height(root->left), height(root->right)) + 1;
}

int sizeOfTree(node* root){
    return root == NULL? 0 : root->size;
}

void right_rotate(node*& ref_root)
{
        node *y = ref_root->left;
	ref_root->left = y->right;
	y->right = ref_root;
        
	ref_root->size = sizeOfTree(ref_root->left) + sizeOfTree(ref_root->right) + 1;
	update_height(ref_root);
        y->size = sizeOfTree(y->left) + sizeOfTree(y->right) + 1;
	update_height(y);
        ref_root=y;
}

void left_rotate(node*& ref_root)
{
node *y = ref_root->right;
	ref_root->right = y->left;
	y->left = ref_root;
        
        ref_root->size = sizeOfTree(ref_root->left) + sizeOfTree(ref_root->right) + 1;
	update_height(ref_root);
        y->size = sizeOfTree(y->left) + sizeOfTree(y->right) + 1;
	update_height(y);
        ref_root=y;
}

//after deletion and insertion, maintain a balanced tree.
void maintain(node*& ref_root)
{
        int balance=0;
        if (!ref_root) return ;
        update_height(ref_root);
	balance = height(ref_root->left)-height(ref_root->right);
	if (balance > 1)
	{
		if ((height(ref_root->left->left)-height(ref_root->left->right)) < 0) //LR
                        left_rotate(ref_root->left);
		right_rotate(ref_root);
	}
	else if (balance < -1)
	{
		if ((height(ref_root->right->left)-height(ref_root->right->right)) > 0) //RL
                        right_rotate(ref_root->right);
		left_rotate(ref_root);
	}
}

void insert_key(int key, node*& ref_root)
{
        cnt ++;
        node*p=new node(key);
        if(!ref_root){
         node*p=new node(key);
              ref_root=p;
        }
	if(key < ref_root->key){
		insert_key(key,ref_root->left);
                ref_root->size=ref_root->size+1;
        }
	else if(key > ref_root->key){
		insert_key(key,ref_root->right);
                ref_root->size=ref_root->size+1;
        }
	/* calculate the height after insertion */
                update_height(ref_root);
	maintain(ref_root);
}

void delete_key(int key, node*& ref_root)
{
        if(key <  ref_root->key){
                ref_root->size=ref_root->size-1;
		delete_key(key, ref_root->left);
        }
	else if(key >  ref_root->key){
                ref_root->size=ref_root->size-1;
		 delete_key(key,ref_root->right);
        }
	else {
		if(!ref_root->left) {
				struct node *temp =  ref_root->right;
				//free( ref_root);
				ref_root = temp;
                                if(ref_root!=NULL)
                                ref_root->size=ref_root->size;
		} else if(! ref_root->right) {
			struct node *temp =  ref_root->left;
			//free (ref_root);
			 ref_root = temp;
		} else {
                        struct node *temp = ref_root->right;
	                while(temp->left != NULL)
		                temp = temp->left;
			ref_root->key = temp->key;
                        temp->key=key;
                        ref_root->size=ref_root->size-1;
			delete_key(key,ref_root->right);
		}
	}
        if(ref_root!=NULL)
                update_height(ref_root);
        maintain(ref_root);
}

int KthMin(node * root, int k){  
if(root->left!=NULL){
int lSize = sizeOfTree(root->left);
if (k <= lSize) 
        return KthMin(root->left, k);
else if (lSize + 1 < k)
        return KthMin(root->right, k - lSize - 1);
return root->key;
}
else
{
        if(root->right==NULL)
                return root->key;
        else if(k==1)
                return root->key;
        else
        return KthMin(root->right, k  - 1);
        
}
}

void printtree(node* root) {
	if (root == NULL) {
		return;
	}
	cout << root->key<<'X'<<endl;
        cout<<"L"<<endl;
	printtree(root->left);
        cout<<"R"<<endl;
	printtree(root->right);
	if (root->left == NULL || root->right == NULL) {
		return;
	}
}

void preOrder(node *root)
{
    if(root != NULL)
    {
        printf("k %d s %d h %d ", root->key,root->size,root->height);
        preOrder(root->left);
        preOrder(root->right);
    }
}

int main()
{
    node *root=NULL;
    char op[10] = "";
    int k;
    while(true)
    {
        //if(cnt>1)
        // { preOrder(root);/*printtree(root);*/}
        scanf("%s", op);
        if(op[0] == 'E') break;
        switch(op[0])
        {
            case 'A': scanf("%d", &k); insert_key(k, root); break;
            case 'D': scanf("%d", &k); delete_key(k, root); break;
            case 'M': scanf("%d", &k); printf("%d\n", KthMin(root, k));break;
            default: assert(0);
        }
    }

    return 0;
}