#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "functions.h"

void pass2(char*inter);

void pass2(char*inter){
	char line[100]; //line to be split
	char lab[10]; //label
	char opc[10]; //opcode
	char oper[10]; //operand
	char loc[10]; //storing locctr in hex
	char prog_name[10]; //program name
	char star_addr[10]; //stores starting address
	char error[100]; //for errors
	char mach_code[4]; //storing the machine value of the opcode
	char oper_addr[15]; //storing addr of foward referenced labels
	char obj_code[33]; //object code
	char byte_hex[33]; 
	bool indx = false; 
	int counter = 0;//used for text records

	strcpy(obj_code, "\0");
	memset(error, '\0', sizeof(error));

	FILE *src = fopen(inter, "r"); //reading intermediate file
	FILE *list = fopen("listing.txt", "w"); //writing to the listing file
	FILE *obj = fopen("object_file.txt", "w");//writing to the object file
	
	if(src == NULL){
		printf("Cannot open file\n");
        exit(1);
	}
	else{
		fgets(line, sizeof(line), src);
		split_inter(line, loc, lab, opc, oper, error);

		//saving the name of program
		strcpy(prog_name, lab);

		if(strcmp(opc, "START") == 0){
			//write to listing
			fprintf(list, "LOCCTR\tOBJ_CODE LABEL\tOPCODE\tOPERAND\tERRORS\n");
			fprintf(list, "%s\t %s\t %s\t %s\t %s\t %s\n", loc, obj_code, lab, opc, oper, error);
			strcpy(star_addr, oper);
		}
		//intializing header + writing to obj file
		init_header(obj, oper, prog_name, star_addr, prog_len);

		//intialize text record

		//while opcode !=end
		while(strcmp(opc, "END") != 0){
			fgets(line, sizeof(line), src);
			split_inter(line, loc, lab, opc, oper, error);


			//if this is not a comment
			if(line[0] != '.'){
				if(error[0] == '\0'){
					if(search(opc) == 1){ //if opcode found in OPTAB
						get_val(opc, mach_code); //get the hex val of opcode

						//if there's a symbol in the operand field
						if(oper[0] != '\0'){
							//if indexing is required (",X")
							if(index_oper(oper) == 1){
								indx = true;  //variable to hold if indexing is required
								//remove ,X to find the opcode
								char chop[10];
								int len=strlen(oper)-1;
								for(int i=0; i < len-1; i++){
									chop[i] = oper[i];
								}
								strcpy(oper, chop);
							}
							else{
								indx = false; 
							}
							//search SYMTAB for operand
							if(search2(oper) == 1){ //if oper found in symbtab
								//store symbol value as operand address
								get_addr(oper, oper_addr);

								//if indexed, convert correct obj code
								if(indx == true){
									int loc_hex = (int)strtol(oper_addr, NULL, 16); //converting hex string to a decimal
	            					loc_hex+=32768; //adding 8000 because we're adding (1000)2 in binary *32768
	            					sprintf(oper_addr, "%X", loc_hex); //converting loc decimal to hex
								}
							}
							else{
								//store 0 as operand address
								strcpy(oper_addr, "0000");
								//error: undefined symbol
								strcpy(error, "#undefSYMB");
							}//END IF found in SYMTAB

							//creating objcode
							strcpy(obj_code, mach_code);
							strcat(obj_code, oper_addr);
						}
	        			else if(strcmp(opc, "RSUB") == 0){
	        				//creating object code with 0000
	        				char hex[10];
	        				int opr = 0;
	        				strcpy(obj_code, mach_code);
	        				sprintf(hex, "%04X", opr);
	        				strcat(obj_code, hex);
	        			}//END IF there's an operand

					}
	        		else{
	        			//if opc is BYTE or WORD
	        			if(strcmp(opc, "BYTE") == 0){
	        				//convert constant to obj code
	        				char cons[10];
	        				if(oper[0] == 'C'){
	        					for(int i=2; i<strlen(oper)-1; i++){
	        						sprintf(cons, "%02X", oper[i]);
	        						strcat(byte_hex, cons);
	        					}
	        					strcpy(obj_code, byte_hex);
	        				}
	        				else if(oper[0] == 'X'){
	        					
	        					for(int i=2; i<strlen(oper)-1; i++){
	        						sprintf(cons, "%c", oper[i]);
	        						strcat(byte_hex, cons);
	        					}
	        					strcpy(obj_code, byte_hex);
	        				}
	        			}
	        			else if(strcmp(opc, "WORD") == 0){
	        				int opr = (int)strtol(oper, NULL, 10); //turning oper string into int
	        				sprintf(obj_code, "%06X", opr);//converting decimal into hex string
	        			}//END IF opc byt || word
	        		}//END IF opc found in symtab

	        		//adding the ,X removed previously for printing purposes
	        		if(indx == true){
	        			strcat(oper, ",X");
	        		}
	        		//writing to listing file
	        		fprintf(list, "%s\t %s\t %s\t %s\t %s\t %s\n", loc, obj_code, lab, opc, oper, error);

	        		//writing text records into object file
	        		if(counter == 0){
						begin_textR(obj, loc,obj_code);
					}
					//if obj code does not fit into current text{
	        		//write current T row into file
	        		//intialize new T row
					else if(counter > 0 && counter  < 10){
						if(obj_code[0] != '\0'){
							fprintf(obj, "%s", obj_code);
						}
					}
					else if(counter == 10){
						begin_textR(obj, loc,obj_code);
						counter = 0;
					}

					counter++;
					//add obj code to text record
	        		//setting strings to '0'
	        		memset(error, '\0', sizeof(error));
	        		memset(byte_hex, '\0', sizeof(byte_hex));
	        		memset(obj_code, '\0', sizeof(obj_code));
	        	}
	        	else{
	        		fprintf(list, "%s\t %s\t %s\t %s\t %s\t %s\n", loc, obj_code, lab, opc, oper, error);
	        		fprintf(list, "****cannot generate object code because of error\n");

	        		//to not generate an object file if there are any errors on intermediate file
	        		int ret = remove("object_file.txt");
	        		memset(error, '\0', sizeof(error));
	        	}
	        	memset(error, '\0', sizeof(error));
			}//END IF line is not comment*/
	        else{
	        	fprintf(list, "%s", line);
	        }
	        memset(error, '\0', sizeof(error));
		}//END WHILE opc != end
		//memset(error, '\0', sizeof(error));
		//write last text record to object file
		//write end record to object file
		fprintf(obj, "\nE%s", star_addr);

		//closing files
		fclose(src);
		fclose(list);
		fclose(obj);
	}
}