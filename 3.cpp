#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int d;
    Node *left, *right;
};

struct Node* getNode(int d)
{
    struct Node *newNode = new Node;
    newNode->d = d;
    newNode->left = newNode->right = NULL;
    return newNode;
}

bool hasPath(Node *origin, vector<int>& arr, int x)
{
    if (!origin)
        return false;

    arr.push_back(origin->d);

    if (origin->d == x)
        return true;

    if (hasPath(origin->left, arr, x) ||
        hasPath(origin->right, arr, x))
        return true;

    arr.pop_back();
    return false;
}


void printPath(Node *origin, int x)
{
    vector<int> arr;


    if (hasPath(origin, arr, x))
    {
        for (int i=0; i<arr.size()-1; i++)
            cout << arr[i] << "->";
        cout << arr[arr.size() - 1];
    }

    else
        cout << "No Path";
}

int main()
{
    struct Node *origin = getNode(1);
    origin->left = getNode(2);
    origin->right = getNode(3);
    origin->left->left = getNode(4);
    origin->left->right = getNode(5);
    origin->right->left = getNode(6);
    origin->right->right = getNode(7);

    int x = 5;
    printPath(origin, x);
    return 0;
}