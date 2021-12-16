#include <stdio.h>
#include <malloc.h>
#define COUNT 4

typedef struct cel {
  int info;
  struct cel * esq;
  struct cel * dir;
} no;

typedef struct Node
{
    int data;
    struct Node* left, *right;
} bruh;
 
struct Node* newNode(int data)
{
    struct Node* node = malloc(sizeof(struct Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}
 
void print2DUtil(bruh *root, int profundidade)
{
    int i;

    if(root == NULL)
        return;
 
    
 
    print2DUtil(root->right, ++profundidade);
    printf("\n");

    for(i = 0; i < profundidade; ++i)
        putchar('\t');

    printf("%d\n", root->data);
 
    print2DUtil(root->left, profundidade);
}
 
void print2D(bruh *root)
{
   print2DUtil(root, 0);
}

int main()
{
    struct Node *root   = newNode(1);
    root->left   = newNode(2);
    root->right  = newNode(3);
 
    root->left->left  = newNode(4);
    root->left->right = newNode(5);
    root->right->left  = newNode(6);
    root->right->right = newNode(7);
 
    root->left->left->left  = newNode(8);
    root->left->left->right  = newNode(9);
    root->left->right->left  = newNode(10);
    root->left->right->right  = newNode(11);
    root->right->left->left  = newNode(12);
    root->right->left->right  = newNode(13);
    root->right->right->left  = newNode(14);
    root->right->right->right  = newNode(15);
     
/*     print2D(root); */
    print2DUtil(root, 0);
 
    return 0;
}