#include "dbl_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "pattern_matching.h"


////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////



//   Initialize a double linked list

void dbllist_init(dbllist_t * list)
{
    assert(list!=NULL);
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////



//   Append data to list (add as last node of the list)

int dbllist_append(dbllist_t* l, void* d)
{
    assert(l!=NULL);
    
    dbllist_node_t *node = (dbllist_node_t*)malloc(sizeof(dbllist_node_t));
    
    assert(node!=NULL);
    
    node->next=NULL;
    node->prev=NULL;
    node->data=d;
    
    
    // adding first node    
    if(l->head==NULL || l->size==0)
    {
        l->head=node;
        l->tail=node;
        l->size++;
        return 0;
    }
    
    else
    {
        
        dbllist_node_t *p = l->head;
        
        // go to last node
        while(p->next!=NULL)
        {
            p=p->next;
        }
        
        // add node
        node->prev=p;
        p->next=node;
        l->tail=node;
        l->size++;
        return 0;
    }
    
    return -1;
    
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////



//   Prepend data to list (add as first node of the list)

int dbllist_prepend(dbllist_t * list , void * d)
{
    
    assert(list!=NULL);
    
    dbllist_node_t *node = (dbllist_node_t*)malloc(sizeof(dbllist_node_t));
    
    assert(node!=NULL);

    node->data=d;
    
    // adding first node
    if(list->head==NULL || list->size==0)
    {
        node->next = NULL;
        node->prev = NULL;
        list->size++;
        list->head = node;
        list->tail = node;
        return 0;
    }
    
    else
    {
        node->next = list->head;
        node->prev=NULL;
        list->size++;
        list->head->prev=node;
        list->head=node;
        
        return 0;
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////



//   Remove the specific node from the list. 

int dbllist_remove(dbllist_t * list , dbllist_node_t* node ,dbllist_destroy_t flag)
{
    if(list->head==NULL)
        return -1;

    // only 1 node
    if(list->size==1)
    {
        if (flag==DBLLIST_FREE_DATA)
            free (node->data);
        free (node);
        list->size--;
        list->head=NULL;
        list->tail=NULL;
        return 0;
    }

    
    // first node
    else if(node->prev==NULL && node->next!=NULL)
    {
        // dbllist_node_t* temp = list->head;
        list->head = node->next;
        list->head->prev = NULL;
        if (flag==DBLLIST_FREE_DATA)
            free (node->data);        
        free (node);
        list->size--;
        return 0;
    }
    
    // last node
    else if(node->next==NULL)
    {
        // dbllist_node_t* temp;
        // temp = list->tail;
        list->tail = node->prev;
        list->tail->next = NULL;
        if (flag==DBLLIST_FREE_DATA)
            free (node->data);        
        free (node);
        list->size--;
        return 0;
    }
    
    // node somwhere in the middle
    else
    {
        dbllist_node_t* n = node->next;
        dbllist_node_t* p = node->prev;
        
        p->next=node->next;
        n->prev=node->prev;
        if (flag==DBLLIST_FREE_DATA)
            free (node->data);
        free (node);
        list->size--;
        return 0;
    }
    
    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////



//   Destroy and de-allocate the memory hold by a list

void dbllist_destroy(dbllist_t * list , dbllist_destroy_t flag)
{
    assert(list!=NULL);
    if(list->head==NULL  || list->size==0)
        return;
    
    dbllist_node_t* p = list->head;
    assert(p!=NULL);
    
    // go through all nodes
    while(p!=NULL)
    {
        // remove node
        dbllist_remove(list,p,flag);
        p=list->head;
    }
    
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////






