#include "pattern_matching.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////


//   Add ouput from prev_state & fail_state to this_state
//   Function is used in pm_makeFSM

void pm_collect_previous_output(pm_state_t* this_state, pm_state_t* fail_state)
{
    
    // check if there's output to collect
    if(fail_state->output==NULL)
        return;
    
    
    // if this_state has no output list , initiliaze it
    if(this_state->output==NULL)
    { 
        this_state->output = (dbllist_t*)malloc(sizeof(dbllist_t));
        dbllist_init(this_state->output);
    }
    
    assert(this_state->output!=NULL);
    
    
    // index node to go through lists
    // dbllist_node_t* index_node = NULL;
    
    // if fail state has an output list
    if(fail_state->output!=NULL && fail_state->output->head!=NULL)
    {
        
        pm_state_t* index_state = fail_state;
        // index_node = fail_state->output->head;
        char* check = " check next output :) ";
        
        
        // go through all fail states
        while(check!=NULL && index_state!=NULL && index_state->id!=0)
        {
            // collect output
            if(index_state->output!=NULL && index_state->output->head!=NULL && index_state->output->head->data!=NULL )
            {
                dbllist_append(this_state->output, (char*)index_state->output->head->data);
                check = (char*)index_state->output->head->data;
            }
            
            index_state = index_state->fail;
        }
    }
    
    
    
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////



//   initiliaze our automaton

int pm_init(pm_t * sm)
{
    assert(sm!=NULL);
    
    sm->newstate=1;
    
    // initializing our zero state
    pm_state_t* start = (pm_state_t*) malloc(sizeof(pm_state_t));
    assert(start!=NULL);
    start->id=0;
    start->depth=0;
    start->output = NULL;
	start->fail = start;
	start->_transitions = NULL;
	sm->zerostate = start;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////



//   Adds a new string to the fsm, given that the string is of length n. 
//   Returns 0 on success, -1 on failure.

int pm_addstring(pm_t * tree ,unsigned char * str, size_t n)
{
    assert(n!=0);
    assert(tree!=NULL);
    
    // if(sizeof(&str)!=n)
    // {
    //     printf("Illegal parameters for pm_addstring function....\n");
    //     return -1;
    // }

    // initialize output list
    dbllist_t* output_list = (dbllist_t*)malloc(sizeof(dbllist_t));
    dbllist_init(output_list);
    assert(output_list!=NULL);
    // dbllist_node_t* output_node;
    // char* s;
 
    // pointer to go through fsm states
    pm_state_t* current_state = tree->zerostate;
    
    // initialize new_state
    pm_state_t* new_state = NULL;
    
    // variable to help us track the id of new_state
    int new_id = tree->newstate;
        
        
    // go through string    
    for(int i=0; i<n; i++)
    {
        // initialize new_state
        new_state = (pm_state_t*) malloc(sizeof(pm_state_t));
        assert(new_state!=NULL);
        new_state->depth = i+1;
        new_state->id = new_id+i;

        assert(current_state!=NULL);
        
        // if there is an edge from current state
        if(pm_goto_get(current_state, str[i])!=NULL)
        {
            // ?????????
            new_state->id--;
            new_state->depth++;
            new_id--;
            
            // keep going
            current_state = pm_goto_get(current_state, str[i]);
            free(new_state);
        }
        else // no edge
        {
            // set edge to new state and update fsm
            tree->newstate++;
            pm_goto_set(current_state, str[i], new_state);
            printf("Allocating state: %d\n", new_state->id);
            printf("%d -> %c -> %d\n", current_state->id, str[i], new_state->id);
            // keep going
            current_state = pm_goto_get(current_state, str[i]);
        }
        
        // last char of string , we need to add output to state
        if(current_state->depth == n)
        {
            if(current_state->output==NULL)
            {
                dbllist_append(output_list, (void*)str);
                current_state->output = output_list;
            }
            
            // /*
            else
                free(output_list);
            // */
        }
        
    }
    

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////


//   Returns the transition state.  If no such state exists, returns NULL. 
//   Function is used in pm_addstring, pm_makeFSM, pm_fsm_search, pm_destroy functions. 

pm_state_t* pm_goto_get(pm_state_t *state,unsigned char symbol)
{
    assert(state!=NULL);
    
    // check if there is no transitions
    if(state->_transitions==NULL)
        return NULL;
    
    // initialize a pointer node to go through transitions list 
    dbllist_node_t* node = state->_transitions->head;
    assert(node!=NULL);

    // go through transitions list
    while(node!=NULL)
    {
        // cast void* to pm_labeled_edge_t
        pm_labeled_edge_t* edj = (pm_labeled_edge_t*) node->data;
        assert(edj!=NULL);
        
        // if the transition label is the same as wanted symbol
        // return the state the edge is pointing to
        if((char) edj->label == symbol)
            return (edj->state);
        
        // keep going through list
        node = node->next;    
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////



//   Set a transition arrow from this from_state, via a symbol, to a to_state.
//   Function is used in the pm_addstring and pm_makeFSM functions.
//   Returns 0 on success, -1 on failure.   

int pm_goto_set(pm_state_t *from_state,unsigned char symbol,pm_state_t *to_state)
{
    assert(from_state!=NULL);
    assert(to_state!=NULL);
    
    // if transition list isn't already initialized, do it now 
    if(from_state->_transitions==NULL)
    {
        from_state->_transitions = (dbllist_t*) malloc(sizeof(dbllist_t*));
        dbllist_init(from_state->_transitions);
    }
    assert(from_state->_transitions!=NULL);
    
    // initialize new edge
    pm_labeled_edge_t* edj = (pm_labeled_edge_t*) malloc(sizeof(pm_labeled_edge_t));
    assert(edj!=NULL);
    edj->label = symbol;
    edj->state = to_state;
    
    // add edge to transitions list
    dbllist_append(from_state->_transitions, (void*)edj);
    return 0;
    
    
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////



//   Finalizes construction by setting up the failrue transitions.
//   Returns 0 on success, -1 on failure.

int pm_makeFSM(pm_t * sm)
{
    assert(sm!=NULL);
    
    // initiliaze empty queue
    dbllist_t* state_list = (dbllist_t*) malloc(sizeof(dbllist_t)) ;
    assert(state_list!=NULL);
    dbllist_init(state_list);    
    
    // pointer to go through fsm states
    pm_state_t* current_state = sm->zerostate;
    assert(current_state->_transitions!=NULL);
    
    // pointer to go through transitions list
    dbllist_node_t* tnode = current_state->_transitions->head;
    
    pm_state_t* fail_state;
    pm_labeled_edge_t* edj;
    
    // check all neighbors of zerostate
    while(tnode!=NULL)
    {
        // cast from void* to pm_labeled_edge_t
        edj = (pm_labeled_edge_t*)tnode->data;
        // set failure transitions of states in depth 1 to the root
        edj->state->fail = sm->zerostate;
        // insert to the queue all the states with depth 1
        dbllist_append(state_list, (void*)edj->state);
        
        // keep going
        tnode = tnode->next;
    }
    
    // pointer to states in queue
    pm_state_t* r ;
    
    // while queue isn't empty
    while(state_list->size>0)
    {
        // Let r be the next state in queue
        r = (pm_state_t*) state_list->head->data;
        
        // check if r has neighbors, make a pointer node to go through them
        if(r->_transitions!=NULL)
            tnode = r->_transitions->head;
        dbllist_remove(state_list, state_list->head, DBLLIST_LEAVE_DATA);
        
        // while there is a transition from r ( g(r,b)=s!=fail )
        while(tnode!=NULL)
        {
            // Queue.enqueue(s)
            edj = tnode->data;
            dbllist_append(state_list, (void*)(edj->state));
            
            // pointer state to go through previous fail states of r
            fail_state = r->fail;
            
            // while fail_state has no transitions
            while(pm_goto_get(fail_state, edj->label)==NULL && fail_state->id != fail_state->fail->id)
            {
                // go to previous fail
                fail_state = fail_state->fail;
            }
            
            // set fail state of r
            
            if(pm_goto_get(fail_state, edj->label)!=NULL)
                edj->state->fail = pm_goto_get(fail_state, edj->label);
            else
                edj->state->fail=sm->zerostate;
            
            printf("Setting f(%d) = %d \n",edj->state->id ,edj->state->fail->id);
            
            
            // collect previous ouputs
            pm_collect_previous_output(edj->state, edj->state->fail);
            
            // keep going
            tnode = tnode->next;
        }
    }
    free(state_list);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////



//   Search for matches in a string of size n in the FSM. 
//   if there are no matches return empty list

dbllist_t* pm_fsm_search(pm_state_t * current_state ,unsigned char * str ,size_t n)
{
    
    // initialize match_list
    dbllist_t* match_list = (dbllist_t*) malloc(sizeof(dbllist_t));
    dbllist_init(match_list);
    assert(match_list);
    
    // pointer node to run through lists
    dbllist_node_t* index_node ;
    
    // 
    pm_match_t* match;
    
    // go through string
    for(int i=0; i<n; i++)
    {
        // int end = i-1 ;
        
        // while there is no transition, go to fail state
        while(pm_goto_get(current_state, str[i])==NULL)
        {
            current_state = current_state->fail;
            if(current_state->id==0)
                break;
        }
        
        // if theres a transition , go to next state
        if(pm_goto_get(current_state, str[i])!=NULL)
            current_state = pm_goto_get(current_state, str[i]);
        
        // if theres output, add to list
        if(current_state->output!=NULL && current_state->output->size>0)
        {
            // start from head of output_list
            index_node = current_state->output->head;
            
            // will help me check duplicate matches
            pm_match_t* check_duplicate = NULL;
            
            // while there's still output to add
            while(index_node!=NULL)
            {
                // initialize match
                match = (pm_match_t*)malloc(sizeof(pm_match_t));
                match->fstate = current_state;
                // update indexes, and pattern
                match->pattern = (char*) index_node->data;
                if(strlen(match->pattern)==1)
                {
                    match->start_pos = i; 
                    match->end_pos = i;
                }
                else
                {
                    match->end_pos = i;
                    match->start_pos = i-strlen(match->pattern)+1;
                }
                
                // check tail of match list to prevent duplicate nodes
                if(match_list->tail!=NULL)
                {
                    check_duplicate = (pm_match_t*) match_list->tail->data;
                    
                    // no duplicate, add match to list
                    if(check_duplicate->start_pos != match->start_pos || check_duplicate->end_pos != match->end_pos)
                    {
                        dbllist_append(match_list, (void*)match);
                        // keep going
                        index_node = index_node->next;
                    }
                    // duplicate match found, keep going...
                    else
                    {
                        index_node = index_node->next;
                        free(match);
                    }
                }
                // first node, no duplicates :)
                else
                {
                    match_list->size=0;
                    // add match to list
                    dbllist_append(match_list, (void*)match);
                    // keep going
                    index_node = index_node->next;
                }
                
            }
            
            
        }
    
        
    }
    
    return match_list;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////



//   Destroys the fsm, deallocating memory.

void pm_destroy(pm_t *sm)
{
    assert(sm!=NULL);

    // initialize a queue to help us run through fsm
    dbllist_t* q = (dbllist_t*) malloc(sizeof(dbllist_t)) ;
    dbllist_init(q);    // Queue <- empty
    
    // pointer state to go through fsm
    pm_state_t* current_state = sm->zerostate;
    assert(current_state->_transitions!=NULL);
    
    // pointer to transition from current_state
    dbllist_node_t* tnode = current_state->_transitions->head;
        
    pm_labeled_edge_t* edj = NULL;
    
    // go through all neighbors of zerostate
    while(tnode!=NULL)
    {
        // add state to queue
        edj = (pm_labeled_edge_t*)tnode->data;
        edj->state->fail = sm->zerostate;
        dbllist_append(q, (void*)edj->state);
        
        // keep going
        tnode = tnode->next;
    }
    
    // deleting zero state from fsm
    dbllist_destroy(current_state->_transitions, DBLLIST_FREE_DATA);
    free(current_state->_transitions);
    current_state->_transitions=NULL;
    free(current_state);

    pm_state_t* r = NULL;
    
    // while queue isn't empty
    while(q->size>0)
    {
        // Let r be the next state in queue
        r = (pm_state_t*) q->head->data;
        if(r->_transitions!=NULL)
            tnode = r->_transitions->head;
        
        // r = Q.dequeue
        dbllist_remove(q, q->head, DBLLIST_LEAVE_DATA);
        
        // while there r has neighbors ( g(r,b)=s!=fail )
        while(tnode!=NULL)
        {
            // add r neighbor to queue
            edj = tnode->data;
            dbllist_append(q, (void*)(edj->state));
            
            // check next transition
            tnode = tnode->next;
        }
        
        // if r has transitions list , deallocate list
        if(r->_transitions!=NULL && r->_transitions->head!=NULL)
        {
            dbllist_destroy(r->_transitions, DBLLIST_FREE_DATA);
            free(r->_transitions);
            r->_transitions=NULL;
        }
        // if r has ouput list , deallocate list
        if(r->output!=NULL && r->output->head!=NULL)
        {
            dbllist_destroy(r->output, DBLLIST_LEAVE_DATA);
            free(r->output);
            r->output=NULL;
        }
        
        free(r);
    }
    free(q);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//..............................................................................................
////////////////////////////////////////////////////////////////////////////////////////////////



void printMatchList(dbllist_t* l)
{
    if(l->size==0)
    {
        printf("no list to print !!!!\n");
        return;
    }
    printf("\n\n|||||||||||||||||||||| printing match list ||||||||||||||||||||||\n\n");
    
    dbllist_node_t *p = l->head;
    int i = 0;

    pm_match_t* match = NULL;
    
    while(i<l->size)
    {
        match = (pm_match_t*)(p->data);
        // printf("i==%d\n",i);
        printf("%d)  %s @ (%d,%d) \n",i+1,  match->pattern,match->start_pos, match->end_pos);
        // if(i!=l->size)
            p=p->next;
        i++;
    }

    printf("\n");
}

