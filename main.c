#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;
Node* head;
Node *tail;
void append(int element)
{
    Node *node=(Node*)malloc(sizeof(Node));
    node->data=element;
    node->next=NULL;
    if(head==NULL)
    {
        head=node;
    }
    else
    {
        tail->next=node;
    }
    tail=node;
}
void prepend(int element)
{
    Node *node=(Node*)malloc(sizeof(Node));
    node->data=element;
    node->next=head;
    head=node;
    if(node->next==NULL)
    {
        tail=node;
    }
}
int countList()
{
    int count_list=0;
    Node* node;
    node=head;
    while(node!=NULL)
    {
        count_list+=1;
        node=node->next;
    }
    return count_list;
}
void insert(int element,int position)
{
    Node *node=(Node*)malloc(sizeof(Node));
    node->data=element;
    Node *temp;
    temp=head;
    int count=1;
    int count_list=countList();
    if(position>count_list)
    {
        append(element);
        printf("Index out of bound but will be appended \n");
        return;
    }
    if(position==1)
    {
        prepend(element);
        return;
    }
    for(int i=1; i<position-1; i++)
    {
        temp=temp->next;
    }
    node->next=temp->next;
    temp->next=node;

}
void deleteFirst()
{
    if(head==NULL)
    {
        printf("Nothing to delete");
        return;
    }
    Node *temp;
    temp=head;
    head=temp->next;
    free(temp);
}
void deleteLast()
{
    if(head==NULL)
    {
        printf("Nothing to delete\n");
        return;
    }
    if(head->next==NULL)
    {
        head=NULL;
        tail=NULL;
        return;
    }
    Node* node;
    Node* pnode;
    pnode=head;
    node=head->next;
    while(node->next!=NULL)
    {
        pnode=node;
        node=node->next;
    }
    pnode->next=NULL;
    tail=pnode;
    free(node);
}
void deleteAtPos(int position)
{
    if(head==NULL)
    {
        printf("Nothing to delete\n");
        return;
    }
    if(position>countList())
    {
        printf("No such position\n");
        return;
    }
    if(position==1)
    {
        deleteFirst();
        return;
    }
    if(position==countList())
    {
        deleteLast();
        return;
    }
    Node *node;
    Node* pnode;
    node=head;
    for(int i=1; i<position-1; i++)
    {
        node=node->next;
    }
    pnode=node->next;
    node->next=pnode->next;
    free(pnode);
}
void reverseList()
{
    Node *node;
    node=head;
    head=NULL;
    while(node!=NULL)
    {
        prepend(node->data);
        node=node->next;
    }
}
void print()
{
    Node *node;
    node=head;
    while(node!=NULL)
    {
        printf("The element %d\n",node->data);
        node=node->next;
    }
}

int main()
{
    head=NULL;
    tail=NULL;
    int n;
    printf("Enter the number of integeres\n");
    scanf("%d",&n);
    for(int i=1; i<=n; i++)
    {
        append(i);
    }
    print();
    printf("Inserting at position five\n");
    insert(17,5);
    printf("\n");
    print();

    printf("\nDeleting at first");
    deleteFirst();
    print();
    printf("\nDeleting last\n");
    deleteLast();
    print();
    printf("\deleting position 3\n");
    deleteAtPos(1);
    print();
    printf("\n Reversing \n");
    reverseList();
    print();

}
