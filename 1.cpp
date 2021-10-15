#include <bits/stdc++.h>
using namespace std;


struct Node
{
    int d;
    Node* left, * right;
};

Node* newNode(int d)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->d = d;
    node->left = node->right = NULL;
    return (node);
}

Node* iLO(int arr[], Node* r,
                       int i, int n)
{
    if (i < n)
    {
        Node* t = newNode(arr[i]);
        r = t;

        r->left = iLO(arr,
                                      r->left, 2 * i + 1, n);

        r->right = iLO(arr,
                                       r->right, 2 * i + 2, n);
    }
    return r;
}

void inOrder(Node* r)
{
    if (r != NULL)
    {
        inOrder(r->left);
        cout << r->d <<" ";
        inOrder(r->right);
    }
}

{
int arr[] = { 1, 2, 3, 4, 5, 6, 6, 6, 6 };
int n = sizeof(arr)/sizeof(arr[0]);
Node* r = iLO(arr, r, 0, n);
inOrder(r);
}
