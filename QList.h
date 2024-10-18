#ifndef QLIST_H
#define QLIST_H

struct Node {
    int val;
    Node* next;

    Node(int _val)
    {
        val = _val;
        next = nullptr;
    }
};
struct List
{
    Node* head = nullptr;
    int lenght = 0;

    Node* append(int _val)
    {
        Node* p = new Node(_val);
        p->val = _val;
        p->next = nullptr;
        if (lenght == 0)
        {
            head = p;
            lenght++;
            return p;
        }
        Node* curr = head;
        for (int i = 1; i < lenght; ++i)
            curr = curr->next;
        curr->next = p;
        lenght++;
        return p;
    }


}; List lst;

#endif // QLIST_H
