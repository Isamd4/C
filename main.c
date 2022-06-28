#include <stdio.h>
#include <stdlib.h>
typedef struct Node{
    int data;
   struct Node* next;
   struct Node* prev;

}Node;
Node* head;
Node* tail;
 Node* newNode(int element){
   Node* node=(Node*)malloc(sizeof(Node));
   node->data=element;
   node->next=NULL;
   node->prev=NULL;
   return node;
}
void append(int element){
    Node *node=newNode(element);
    if(head==NULL){
        head=node;
        tail=node;
        return;
    }
    node->prev=tail;
    tail->next=node;
    tail=node;
}
void prepend(int element){
   Node *node=newNode(element);
    if(head==NULL){
        head=node;
        tail=node;
        return;
    }
    node->next=head;
    head->prev=node;
    head=node;
}
void insert(int element,int position){//inserting at position still not working
    if(position==1){
        prepend(element);
        return;
    }
    if(position>countList()){
        append(element);
        return;
    }
    Node *node=newNode(element);
    Node *temp;
    temp=head;
    for(int i=1;i<position;i++){
        temp=temp->next;
    }
    node->next=temp;
    node->prev=temp->prev;
    temp->prev=node;
}
int countList(){
    int count_list=0;
    Node *node=head;
    while(node!=NULL){
        count_list++;
        node=node->next;
    }
    return count_list;
}
void deletFirst(){
    if(head==NULL){
        printf("There is nothing to delete \n");
        return;
    }
    Node *node=head;
    head=node->next;
    free(node);
}
void deleteLast(){
    if(head==NULL){
        printf("There is nothing to delete\n");
        return;
    }
    Node *node=tail;
    tail=node->prev;
    tail->next=NULL;
    free(node);
}
void deleteAtPos(int position){
    if(head==NULL){
        printf("There is nothing to delete\n");
        return;
    }
    Node *temp=head;
    if(position==1){
        temp=temp->next;
        free(temp);
        return;
    }
    for(int i=1;i<position;i++){
        temp=temp->next;
    }
    Node *node=temp->prev;
    node->next=temp->next;
    free(temp);

}
void print(){
    Node *node;
    node=head;
    while(node!=NULL){
        printf("The element %d \n",node->data);
        node=node->next;
    }
    printf("count: %d\n",countList());
}
int main()
{
    head=NULL;
    tail=NULL;
    int n;
    printf("Enter the number of integeres\n");
    scanf("%d",&n);
    printf("\nAppending\n");
    for(int i=1; i<=n; i++)
    {
        append(i);
    }
    print();
    printf("\nPrepending\n");
    for(int i=1;i<=8;i++){
        prepend(i);
    }
    print();
    printf("\nInsert at pos 3\n");
    insert(17,3);
    print();
    printf("\nDelete First\n");
    deletFirst();
    print();
    printf("\nDelete Last\n");
    deleteLast();
    print();
    printf("\nDelete at pos 3\n");
    deleteAtPos(3);
    print();

}
