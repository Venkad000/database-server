#include <vector>   
#include <bits/stdc++.h>
#include <string>

using namespace std;

template <typename T>
class Node{
    public:
        T data;
        Node<T> * next;
};

template <typename T>
class LinkedList{
    private:
        Node<T> * head;
    public:
        LinkedList();
        LinkedList(int n);
        LinkedList(T a, T b, T c, T d, T e);
        void push(T data);
        void deleteNode(int index);
        string printList();
        T GetNth(int index);
};

template <typename T>
class TreeNode {
    public:
        string name;
        vector<TreeNode*> child;
        vector<LinkedList<T>*>child2;
    public:
        TreeNode(string data)
        {
            name = data;
        }
};

template <typename T>
LinkedList<T>::LinkedList(){
    head = NULL;
}

template <typename T>
LinkedList<T>::LinkedList(int n){
    head = NULL;
    for(int i=0;i<n;i++){
        T ctemp;
        cout<<"Enter the data:";
        cin>>ctemp;
        this->push(ctemp);
    }
}

template <typename T>
LinkedList<T>::LinkedList(T a, T b, T c, T d, T e){
    head = NULL;
    this->push(a);
    this->push(b);
    this->push(c);
    this->push(d);
    this->push(e);
}

template <typename T>
void LinkedList<T>::push(T new_data){
    Node<T>* new_node = new Node<T>();
    new_node->data = new_data;
    new_node->next = head;
    head = new_node;
}

template <typename T>
void LinkedList<T>::deleteNode(int key){
    Node<T>* temp = head;
    Node<T>* prev = NULL;

    if (temp != NULL && temp->data == key)
    {
        head = temp->next; 
        delete temp;          
        return;
    }
      else
    {
    while (temp != NULL && temp->data != key)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
    prev->next = temp->next;
    delete temp;
    }
}

template <typename T>
string LinkedList<T>::printList()
{
    string a("");
    Node<T>*node = head;
    while (node != NULL)
    {
        // cout << node->data << " ";
        a.append(node->data).append(" ");
        node = node->next;
    }
    a.append("\n");
    // cout<<endl;
    return a;
}

template <typename T>
T LinkedList<T>::GetNth(int index)
{
 
    Node<T>* current = head;
    int count = 0;
    while (current != NULL) {
        if (count == index)
            return (current->data);
        count++;
        current = current->next;
    }
    assert(0);
}
