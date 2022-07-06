#include <stdio.h>
#include <stdlib.h>

#define SIZE 10    //Considering initially our memory has a capacity of -SIZE- blocks

//Keeping track of requests through a linked list...

typedef enum status_code {FAILURE, SUCCESS} sc;
typedef enum occupancy_status {UNOCCUPIED, OCCUPIED} occ;

typedef struct node_tag
{
    struct node_tag* prev;
    struct node_tag* next;
    occ state;
}Node;
typedef struct node_ip_tag
{
    struct node_ip_tag* prev;
    struct node_ip_tag* next;
    Node *id;
    int size;

}Node_IP;

Node* initialize_list()                     //Initialize Heap linked list
{
    Node *nptr,*lptr;
    nptr=NULL;
    lptr=NULL;

    for(int i=0;i<SIZE;i++)
    {
        nptr=(Node *)malloc(sizeof(Node));
        nptr->prev=NULL;
        nptr->next=lptr;
        nptr->state=UNOCCUPIED;
        lptr=nptr;
    }
    return lptr;
}

sc allocate(int size_req, Node *head, Node **ptr_to_addr)   //Allocate memory according to request
{
    Node *nptr,*start_node;
    sc   allocate_status;
    int count=0;
    nptr=head;
    start_node=head;

    while(count!=size_req && nptr!=NULL)
    {
        if(nptr->state==OCCUPIED)
        {
            nptr=nptr->next;
            start_node=nptr;
            count=0;
        }
        else
        {
            nptr=nptr->next;
            count++;
        }
    }
    if(count==size_req)            //Required number of contiguous blocks found
    {
        *ptr_to_addr=start_node;
        while(start_node!=nptr)
        {
            start_node->state=OCCUPIED;
            start_node=start_node->next;
        }
        allocate_status=SUCCESS;
    }
    else
    {
        allocate_status=FAILURE;
    }
    return allocate_status;

}

void make_block(Node_IP **ptr,Node *ptr_to_addr, int num_blocks)     //Making a block which will store info about the requests
{
    Node_IP *nptr,*temp;
    temp=*ptr;
    nptr=(Node_IP *)malloc(sizeof(Node_IP));

    nptr->size=num_blocks;
    nptr->id=ptr_to_addr;

    if(temp==NULL)
    {
        nptr->next=NULL;
        nptr->prev=NULL;
        *ptr=nptr;
    }
    else
    {
        while(temp->next!=NULL)
            temp=temp->next;
        temp->next=nptr;
        nptr->prev=temp;
        nptr->next=NULL;
    }
}


void free_memory(int p_num, Node *head_of_heap, Node_IP **ptr)          //Free operation on heap
{
    Node_IP *nptr,*temp;
    Node* mem_ptr;
    int count=1;
    nptr=*ptr;
    while(count!=p_num)             //searching for the Node which contains required info about the amount of memory to be removed
    {
        nptr=nptr->next;
        count++;
    }
    mem_ptr=nptr->id;
    count=nptr->size;


    while(count!=0)
    {
        mem_ptr->state=UNOCCUPIED;
        mem_ptr=mem_ptr->next;
        count--;
    }
    //deleting the block containing info about the allocated memory nptr
    temp=*ptr;
    if(nptr->next==NULL && nptr->prev==NULL)
    {
        free(nptr);
        nptr=NULL;
        *ptr=nptr;
    }
    else if(nptr->prev==NULL)
    {
        nptr=nptr->next;
        nptr->prev=NULL;
        free(temp);
        temp=NULL;
        *ptr=nptr;
    }
    else if(nptr->next==NULL)
    {
        nptr->prev->next=NULL;
        free(nptr);
        nptr=NULL;
    }
    else
    {
        nptr->prev->next=nptr->next;
        nptr->next->prev=nptr->prev;
        free(nptr);
	nptr=NULL;
    }

}


int main()
{
    Node *head_of_heap, *ptr_to_addr;
    Node_IP *head_of_inputs, *display;
    head_of_inputs=NULL;
    int choice,num_blocks,count=0;
    head_of_heap=initialize_list();
    sc func_status;
    int  process_number;
    do
    {
        printf("1.Allocate memory\n2.Free memory\n3.Quit Program\n");
        printf("Enter your choice : ");
        scanf("%d",&choice);
        switch (choice)
        {
        case 1:
            printf("Enter number of blocks of memory required : ");
            scanf("%d",&num_blocks);
            if(num_blocks<1)
                printf("Please enter valid number of blocks.\n");
            else
            {
                func_status=allocate(num_blocks,head_of_heap, &ptr_to_addr);
                if(func_status==FAILURE)
                {
                    printf("Not Enough Memory. Request Denied.\n");
                }
                else
                {
                    printf("Operation Successful.\n");
                    make_block(&head_of_inputs, ptr_to_addr, num_blocks);
                    count++;
                }
            }
            break;
        case 2:
            printf("Enter the ID number of the memory to be freed : ");
            scanf("%d",&process_number);
            if(process_number>count || process_number<1)
            {
                printf("Please Enter Valid ID number.\n\n");
            }
            else
            {
                free_memory(process_number, head_of_heap, &head_of_inputs);
                count--;
                printf("Operation Successful.\n");

            }

            break;
        default:
            break;
        }
        //PRINTING.........
        display=head_of_inputs;
        printf("ID:\t\tAllocated Size:\n");
        for(int j=1;j<=count;j++)
        {
                printf("%d\t\t",j);
                printf("%d\n",display->size);
                display=display->next;

        }
        printf("\n\n");

    }while(choice!=3);

}
