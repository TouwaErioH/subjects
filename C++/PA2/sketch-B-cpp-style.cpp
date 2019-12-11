#include <stdio.h>
#include <assert.h>
#include <algorithm>

struct node
{
    int key;
    int height;
    int size;
    node *left, *right;
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

}

int sizeOfTree(node* root){
    return root == NULL? 0 : root->size;
}

void right_rotate(node*& ref_root)
{

}

void left_rotate(node*& ref_root)
{

}

//after deletion and insertion, maintain a balanced tree.
void maintain(node*& ref_root)
{

}

void insert_key(int key, node*& ref_root)
{

}

void delete_key(int key, node*& ref_root)
{

}

int KthMin(node * root, int k){

}

int main()
{
    node *root = 0;
    char op[10] = "";
    int k;
    while(true)
    {
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
