#include <bits/stdc++.h>
using namespace std;

struct Node {
    int d;
    struct Node* left;
    struct Node* right;
};

struct Node* newNode(int d)
{
    struct Node* temp = new Node;
    temp->d = d;
    temp->left = temp->right = NULL;
    return temp;
}

struct Node* insert(struct Node* origin, int d)
{
    if (origin == NULL)
        return newNode(d);
    if (d < origin->d)
        origin->left = insert(origin->left, d);
    else if (d > origin->d)
        origin->right = insert(origin->right, d);
    return origin;
}

void inOrder(struct Node* origin)
{
    if (origin != NULL) {
        inOrder(origin->left);
        cout << origin->d << " ";
        inOrder(origin->right);
    }
}

struct Node* leafDelete(struct Node* origin)
{
    if (origin == NULL)
        return NULL;
    if(origin->left == NULL && origin->right != NULL){
        free(origin);
        origin = origin->right;
        return origin;
    }
    if (origin->left != NULL && origin->right == NULL) {
        free(origin);
        origin = origin->left;
        return origin;
    }

    origin->left = leafDelete(origin->left);
    origin->right = leafDelete(origin->right);

    return origin;
}

int main()
{
    struct Node* origin = NULL;
    origin = insert(origin, 20);
    insert(origin, 10);
    insert(origin, 15);
    insert(origin, 30);
    insert(origin, 25);
    cout << "Дерево до." << endl;
    inOrder(origin);
    cout << endl;
    leafDelete(origin);
    cout << "Дерево после." << endl;
    inOrder(origin);
    return 0;
}