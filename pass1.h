#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "functions.h"

void pass1(char*sour);
//function to assemble, takes in the file name
void pass1(char*sour){
	char line[100]; //line to be split
	char lab[10]; //label
	char opc[10]; //opcode
	char oper[10]; //operand
	char starting_address[10] = "0"; //starting address
	int startaddr = 0;
	char loc_hex[10] = "0"; //location counter
	char loc[10] = "0";
	int loc_dec = 0;
	int op; //used to convert operands into ints
	char err[100] = "\0"; //error array
	FILE *src = fopen(sour, "r");
	FILE *inter = fopen("intermediate.txt", "w");

	OPTAB(optable);
	memset(err, '\0', sizeof(err));

	if(src == NULL){
		printf("Cannot open file\n");
        exit(1);
	}
	else{
		//gets first line
		fgets(line, sizeof(line), src);
		split(line, lab, opc, oper);
		//check if operand in start is valid
		if(strcmp(opc, "START") == 0){
			strcpy(loc_hex, oper);
			strcpy(starting_address, oper);
		}
		fprintf(inter, "%s\t%s\t%s\t%s\n", loc_hex, lab, opc, oper);
		while(!feof(src) && strcmp(opc, "END") != 0){
			strcpy(loc, loc_hex);
			fgets(line, sizeof(line), src);
			split(line, lab, opc, oper);
			memset(err, '\0', sizeof(err));

			if(line[0] != '.'){
				if(lab[0] != '\0'){//if no label, do not insert
        			if(search2(lab) == 0){ //if label not found in symtab
            			insertSy(lab, loc_hex);
        			}
        			else{
            			strcat(err, "#DUP");
        			}//END OF SYMTAB
        			validLabel(lab, err);
    			}//END IF NO LABEL
    			
    			if(search(opc) == 1){//if opc found in OPTAB
            		loc_dec = (int)strtol(loc_hex, NULL, 16); //converting hex string to a decimal
            		loc_dec+=3; //adding 3 to locctr in decimal
            		sprintf(loc_hex, "%X", loc_dec); //converting loc decimal to hex
            		
        		}
		        else if(strcmp(opc, "WORD") == 0){
		            loc_dec = (int)strtol(loc_hex, NULL, 16); //converting hex string to a decimal
            		loc_dec+=3; //adding 3 to locctr in decimal
            		sprintf(loc_hex, "%X", loc_dec);
		        }
		        else if(strcmp(opc, "RESW") == 0){
		            int val;
		            char*c;
		            val = strtol(oper, &c, 10);
		            if(oper[0] == '\0' || *c != '\0' || c == oper){ //check for illegal operand
		                strcat(err, "#MIOPR");
		            }
		            else{
		                op = atoi(oper);
		                loc_dec += (3*op);
		                sprintf(loc_hex, "%X", loc_dec);
		            }
		        }
		        else if(strcmp(opc, "RESB") == 0){
		            int val;
		            char*c;
		            val = strtol(oper, &c, 10);
		            if(*c != '\0' || c == oper){ //check for illegal operand
		                strcat(err, "#MIOPR");
		            }
		            else{
		                op = atoi(oper);
		                loc_dec += op;
		                sprintf(loc_hex, "%X", loc_dec);
		            }
		        }
		        else if(strcmp(opc, "BYTE") == 0){
		        	int len = strlen(oper);
		        	int max_c = 30; 
		        	int byt;
    				if(oper[0] == 'C' && oper[1] == '\'' && oper[len-1]== '\''){
        				loc_dec += len-3; 
	            		sprintf(loc_hex, "%X", loc_dec);
   					}
    				else if(oper[0] == 'X' && oper[1] == '\'' && oper[len-1]== '\''){
    					byt = len - 3; 
    					if(byt % 2 == 0){//checking if bytes is even
    						loc_dec += (byt/2);
    						sprintf(loc_hex, "%X", loc_dec);
    					}
    					else{
    						strcat(err, "#INVCONS");
    					}
    				}
    				else{
    					strcat(err, "#MIOPR");
    				}
    			}		        
		        else{
		        	if(strcmp(opc, "END") == 0){
		        		loc_dec = (int)strtol(loc_hex, NULL, 16); //converting hex string to a decimal
            			loc_dec+=3; //adding 3 to locctr in decimal
            			sprintf(loc_hex, "%X", loc_dec);
		        		int val;
		            	char*c;
		            	val = strtol(oper, &c, 10);
		            	if(oper[0] == '\0' /*|| *c != '\0' || c == oper*/){
		        			strcat(err, "#MIOPR");
		        		}
		        	}
		        	else{
		        		strcat(err,"#ILOPC");
		        	}
				}//END IF OPC FOUND
				//check if operand is in a valid format
				validOpr(oper, err);
				//print to intermediate file
				fprintf(inter, "%s\t%s\t%s\t%s\t%s\n", loc, lab, opc, oper, err);
			}//END IF LABEL IS COMMENT
			else{
				fprintf(inter, "%s", line);
			}
		}//END OF WHILE LOOP
		memset(err, '\0', sizeof(err));
		
		//if too many symbols in the program
		if(symHash->s > 500){
			strcat(err, "#500+SYM");
			fprintf(inter, "Symbols: %d\t %s\n", symHash->s, err);
		}
		//printing the length of the program to file in HEX
		loc_dec = (int)strtol(loc, NULL, 16); //converting string hex to decimal
		startaddr = (int)strtol(starting_address, NULL, 16); //setting starting address
		int program_len = loc_dec - startaddr; //calculating length in dec
		sprintf(prog_len, "%06X", program_len);//converting length from dec to hex string + saving in global var
		fprintf(inter, "program length: %s\n\n", prog_len);

		//printing the SYMTAB to the file
		printHash2(inter);

		fclose(src);
		fclose(inter);
	}//END OF ELSE OPENING FILE
}
