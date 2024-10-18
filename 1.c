#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 1000001

struct Node {
    struct Node *next;
    int data; 
};

struct Node* createNode(int n) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = n;
    newNode->next = NULL;
    return newNode; 
}

void AddToBeggining(struct Node** list, int n) {
    struct Node* newNode = createNode(n);
    newNode->next = *list;
    *list = newNode;
}

void AddToEnd(struct Node** list, int n) {
    struct Node* newNode = createNode(n);
    if(*list == NULL) {
        *list = newNode;
        return;
    }

    struct Node* temp = *list;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

void deleteNode(struct Node** head, int key) {
    struct Node* temp = *head;
    struct Node* prev = NULL;
    if(temp == NULL) {
        printf("List is empty, nothing to delete.\n");
        return;
    }

    if(temp != NULL && temp->data == key) {
        *head = temp->next;
        free(temp);
        return;
    }

    while(temp->data != key && temp!= NULL) {
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL) {
        printf("Key %d was not found in the list.\n",key);
        return;
    }

    prev->next = temp->next;
    free(temp);

}

void traverse(struct Node* head) {
    struct Node* temp = head;
    while(temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

void findKey(struct Node* head,int n) {
    struct Node* temp = head;
    while(temp!= NULL) {
        if(temp->data == n) {
            printf("The number %d exist in the List!\n",n);
            return;
        }
        temp = temp->next;
    }
    printf("The number %d doesnt exist in the list!\n",n);
}

void freeList(struct Node* head) {
    struct Node* temp;
    while(head != NULL) {   
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    srand(time(0));
    clock_t startTime = clock();    

    struct Node* head = NULL;
    for(int i = 0; i < 100000; i++) {
        AddToBeggining(&head, rand() % MAX);
    }
    /*
    AddToBeggining(&head, 5);
    printf("%d\n", head->data);
    AddToEnd(&head,10);
    AddToEnd(&head,8);
    AddToBeggining(&head, 2);
    traverse(head);
    findKey(head,10);
    deleteNode(&head,10);
    traverse(head);
    findKey(head,10); 
    */
    
    clock_t endTime = clock();
    double time_taken = ((double)(endTime - startTime))/CLOCKS_PER_SEC; //Converts from clock ticks to seconds...
    printf("Time of operations: %f seconds!\n",time_taken);
    freeList(head);
    return 0;
}