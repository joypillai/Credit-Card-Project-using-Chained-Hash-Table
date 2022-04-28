#include<stdio.h>	
#include"oldcard.h"	
int main(int argc, char *argv[]){ 
	
	struct Card C;
	struct Address A;
	struct Full_Name F;
	printf("Size of Card: %lu \n",sizeof(C));
	printf("Size of addr: %lu \n",sizeof(A));
	printf("Size of name: %lu \n",sizeof(F));		
	return 0; 
}	
