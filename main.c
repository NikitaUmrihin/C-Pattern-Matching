#include "pattern_matching.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    
    
    // cabab tree
    
    // /* 
    printf("\n__________________________ TEST I __________________________\n");
    
    
    pm_t* tree = (pm_t*) malloc(sizeof(pm_t));
    pm_init(tree);
    
    pm_addstring(tree, (unsigned char*)"abc", 3);
    pm_addstring(tree, (unsigned char*)"ace", 3);
    pm_addstring(tree, (unsigned char*)"abb", 3);
    pm_addstring(tree, (unsigned char*)"acab", 4);
    pm_addstring(tree, (unsigned char*)"bace", 4);
    pm_addstring(tree, (unsigned char*)"cabab", 5);
    pm_addstring(tree, (unsigned char*)"aba", 3);
    pm_addstring(tree, (unsigned char*)"baba", 4);
    pm_addstring(tree, (unsigned char*)"ab", 2);
    pm_addstring(tree, (unsigned char*)"af", 2);
    pm_addstring(tree, (unsigned char*)"cab", 3);
    pm_addstring(tree, (unsigned char*)"caba", 4);
    
    pm_makeFSM(tree);
    
    
    dbllist_t* answers = (dbllist_t*)malloc(sizeof(dbllist_t));
    dbllist_init(answers);
    
    char* txt = "abbabccabbabacbacababbabacababbbaceaf78901234ab";
    
    answers = pm_fsm_search(tree->zerostate, (unsigned char*)txt, strlen(txt));
    
    printf("\nSearching patterns :\n");
    printf("\t\tabc, ace, abb, acab, bace, cabab, aba, baba, ab, af, cab, caba\n");
    printf("in text : \n\t\t%s\n", txt);
    
    printMatchList(answers);
    
    dbllist_destroy(answers, DBLLIST_FREE_DATA);
    free(answers);
    
    pm_destroy(tree);
    free(tree);



    /////////////////////////

    // sababa tree
    
    // */
    
    
    printf("\n__________________________ TEST II __________________________\n");
    printf("\nlets try another tasty test ....\n\n");

    tree = (pm_t*) malloc(sizeof(pm_t));
    pm_init(tree);
    
    pm_addstring(tree, (unsigned char*)"aba", 3);
    pm_addstring(tree, (unsigned char*)"ba", 2);
    pm_addstring(tree, (unsigned char*)"baba", 4);
    pm_addstring(tree, (unsigned char*)"sababa", 6);
    pm_addstring(tree, (unsigned char*)"ab", 2);
    pm_addstring(tree, (unsigned char*)"a", 1);
    pm_addstring(tree, (unsigned char*)"saba", 4);
    pm_addstring(tree, (unsigned char*)"basa", 4);

    
    txt = "ababasababa";
    
    pm_makeFSM(tree);
    
    answers = pm_fsm_search(tree->zerostate, (unsigned char*)txt, strlen(txt));
    
    printf("\nSearching patterns :\n");
    printf("\t\taba, ba, baba, sababa, ab, a, saba, basa\n");
    printf("in text : \n\t\t%s\n", txt);
    
    printMatchList(answers);
    
    dbllist_destroy(answers, DBLLIST_FREE_DATA);
    free(answers);
    
    pm_destroy(tree);
    free(tree);






    /////////////////////////
    
    // shimrit
    
    // /*
    
    printf("\n__________________________ TEST III __________________________\n");
    printf("\nnice !!! now lets try shimrit's tree ....\n\n");
    tree = (pm_t*) malloc(sizeof(pm_t));
    pm_init(tree);
    
    pm_addstring(tree, (unsigned char*)"e", 1);
    pm_addstring(tree, (unsigned char*)"be", 2);
    pm_addstring(tree, (unsigned char*)"bd", 2);
    pm_addstring(tree, (unsigned char*)"bcd", 3);
    pm_addstring(tree, (unsigned char*)"cdbcab", 6);
    pm_addstring(tree, (unsigned char*)"bcaa", 4);
    
    pm_makeFSM(tree);
    
    answers = (dbllist_t*)malloc(sizeof(dbllist_t));
    dbllist_init(answers);
    
    txt = "beeebdbcdbdbcaaabcebcda";
    
    answers = pm_fsm_search(tree->zerostate, (unsigned char*)txt, strlen(txt));
    
    
    printf("\nSearching patterns :\n");
    printf("\t\te, be, bd, bcd, cdbcab, bcaa\n");
    printf("in text : \n\t\t%s\n", txt);
    
    printMatchList(answers);
    
    dbllist_destroy(answers, DBLLIST_FREE_DATA);
    free(answers);
    
    pm_destroy(tree);
    free(tree);

    
    // */
    
    
    /////////////////////////
    
    // the A tree 
    
    printf("\n__________________________ TEST IV __________________________\n");
    
    tree = (pm_t*) malloc(sizeof(pm_t));
    pm_init(tree);
    
    pm_addstring(tree, (unsigned char*)"a", 1);
    pm_addstring(tree, (unsigned char*)"aa", 2);
    pm_addstring(tree, (unsigned char*)"aaa", 3);
    pm_addstring(tree, (unsigned char*)"aaaa", 4);
    pm_addstring(tree, (unsigned char*)"aaaaa", 5);
    
    
    
    
    pm_makeFSM(tree);
    
    answers = (dbllist_t*)malloc(sizeof(dbllist_t));
    dbllist_init(answers);
    
    txt = "aaaaa";
    
    answers = pm_fsm_search(tree->zerostate, (unsigned char*)txt, strlen(txt));
    
    printf("\nSearching patterns :\n");
    printf("\t\ta, aa, aaa, aaaa, aaaaa\n");
    printf("in text : \n\t\t%s\n", txt);
    
    printMatchList(answers);
    
    dbllist_destroy(answers, DBLLIST_FREE_DATA);
    free(answers);
    
    pm_destroy(tree);
    free(tree);
    
    
    
    
    printf("\n__________________________ (: GOODBYE :) __________________________\n");

    return 0;
}
