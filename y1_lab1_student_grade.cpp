#include <stdio.h>

int main(){
	int b,c;
	printf("How many students are you going to enter into the system? \n");
	scanf("%d", &b);
	printf("Enter students' grades\n");
	int array[100];
	for(int i=0;i < b;i++){
		scanf("%d",&array[i]);
	}
	printf("What's the passing grade?\n");
	scanf("%d",&c);
	for(int i=0;i < b;i++){
		if(array[i] >= c){
			printf("Student %d PASSED\n",i);
		}
		
		else{printf("Student %d FAILED\n",i);
		}
		
	}
	
}
