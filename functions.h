#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "optab.h"

//settings both hashtables to null
list *opHash = NULL;
list *symHash = NULL;

/*********************************
        OPTABLE HASH FUNCTIONS
**********************************/

//prints the linked list of a given index
void printList(int i)
{
    node *n = opHash[i].head;
    printf("\nTraversal in forward direction \n");
    while (n != NULL)
    {
        printf("%s\n", n->data);
        n = n->next;
    }
}

//prints whole OPTAB hashtable
void printHash()
{
    for(int i=0; i<29; i++){
    node *n = opHash[i].head;
    while (n != NULL)
    {
        printf("%s\n", n->data);
        n = n->next;
    }
}
}

//returns the index to insert the given string
unsigned int hash(char *s)
{
    int length = strlen(s);
    unsigned int hashVal=0;
    if (length == 0){
        return 0;
    }
    else{
            for(int i=0; i<length; i++){
                unsigned int num = s[i] - '0'; //converts current char into ASCII num
                hashVal += num; //adds that ascii into hashval
            }
            return (43*hashVal) % 29; //hashval mod the size of the hashtable
        }

}

//insert into hash table
void insert(char *d, char *v)
{
    unsigned int i = hash(d); //index where string x will be stored
    node *new_node = malloc(sizeof(node)); //allocating space for node
       
    strcpy(new_node->data, d);
    strcpy(new_node->value, v);
      
    new_node->next = NULL;

    if (opHash[i].head == NULL){
        new_node->prev = NULL;
        opHash[i].tail = new_node;
        opHash[i].head = opHash[i].tail;
    }
    else{
        opHash[i].tail->next = new_node;
        new_node->prev = opHash[i].tail;
        opHash[i].tail = new_node;
    }

       opHash->s++; //number of nodes in total (all linked lists)
}

//insert the optable struct into the hashtable
void OPTAB(arr *opt){
    for(int i=0; i<25; i++){
        insert(opt[i].data, opt[i].value);
    }
}

//search for a mnemonic in the optable
bool search(char *x){
    int i = hash(x);
    node *n = opHash[i].head;
    while (n != NULL){
        if(strcmp(n->data, x) == 0){
            return true;
        }
        else{
            n = n->next;
        }
    }
    return false;
}

/*********************************
        SYMTAB HASH FUNCTIONS
**********************************/

unsigned int hash2(char *s)
{
    int length = strlen(s);
    unsigned int hashVal=0;
    if (length == 0){
        return 0;
    }
    else{
            for(int i=0; i<length; i++){
                unsigned int num = s[i] - '0'; //converts current char into ASCII num
                hashVal += num; //adds that ascii into hashval
            }
            return (43*hashVal) % 501; //hashval mod the size of the hashtable
        }

}
//insert into hash table
void insertSy(char *d, int v)
{
    unsigned int i = hash2(d); //index where string x will be stored; using locctr to index
    node *new_node = malloc(sizeof(node));
       
    strcpy(new_node->lab, d); //copying label 
    new_node->locctr = v; //copying locctr
      
    new_node->next = NULL;

    if (symHash[i].head == NULL){//adding node if list is empty or full
        new_node->prev = NULL;
        symHash[i].tail = new_node;
        symHash[i].head = symHash[i].tail;
    }
    else{
        symHash[i].tail->next = new_node;
        new_node->prev = symHash[i].tail;
        symHash[i].tail = new_node;
    }

       symHash->s++; //number of nodes in total (all linked lists)
}

bool search2(char *x){
    int i = hash2(x);
    node *n = symHash[i].head;
    while (n != NULL){
        if(strcmp(n->lab, x) == 0){
            return true;
        }
        else{
            n = n->next;
        }
    }
    return false;
}

void printHash2(FILE*inter)
{
    fprintf(inter,"%s\t %s\n", "SYMBOL", "LOCCTR");
    fprintf(inter,"-----------------\n");
    for(int i=0; i<501; i++){
    node *n = symHash[i].head;
        while (n != NULL)
        {
            fprintf(inter,"%s\t %d\n", n->lab, n->locctr);
            n = n->next;
        }
    }
}

/*********************************
        PASS1 FUNCTIONS
**********************************/

void split(char*str, char*lab, char*opc, char*oper){
	if(str[0] == ' ' || str[0] == '\t'){//checks if there's no label
        sscanf(str, "%s %s", opc, oper);
        lab[0]= '\0';
        if(strcmp(opc, "RSUB") == 0){ //RSUB no label nor operand
            oper[0] = '\0';
        }    
    }
    else if(str[0] == '.'){//checks if the line is a comment
        lab[0] = '\0';
        opc[0] = '\0';
        oper[0] = '\0';
    }
    else{
        sscanf(str, "%s %s %s", lab, opc, oper);//else grabs the first 3 words
    }

}

void validLabel(char*lab, char*err){
    int len = strlen(lab);
    //checking if label has 6 chars or less
    if(len < 7){
        //checking if the 1st char is a letter
        if(lab[0] <= 'Z' && lab[0] >= 'A' || lab[0] <='z' && lab[0] >= 'a'){
            //checking that the string only has letters and numbers
            for(int i=1; i<len-1; i++){
                if(lab[i] > 'Z' && lab[i] < 'A' || lab[i] >'z' && lab[i] < 'a' || lab[i] >'9' && lab[i] < '0'){
                    strcat(err, "INVLAB");
                }
            }

        }
        else{
            strcat(err, "#INVLAB");
        }
    }
    else{
        strcat(err, "#INVLAB");
    } 
}

void validOpr(char*oper, char *err){
    int len = strlen(oper);
    if(len > 2){
        if(oper[(len-2)] == ','){
            if(oper[len-1] != 'X'){
                strcat(err, "#MIOPR");
            }
        }
    }
}