#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	long int count = 0;

    printf("Enter the size of matrix : ");
    scanf("%ld", &count);
	
    srand(time(0));
    
    FILE *output_file = fopen("M.txt", "w");
    if (output_file == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }
    for (long int i = 1; i <= count; i++) 
    {
    	for(long int j = 1; j <= count; j++) 
    	{ 
        	int num = rand() % 10;
	        fprintf(output_file, "%s,%ld,%ld,%d\n","M",i,j, num);
        }
    }
    fclose(output_file);



    
    FILE *output_file2 = fopen("N.txt", "w");
    if (output_file2 == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }
    for (long int i = 1; i <= count; i++) 
    {
    	for(long int j = 1; j <= count; j++) 
    	{ 
        	int num = rand() % 10;
	        fprintf(output_file2, "%s,%ld,%ld,%d\n","N",i,j, num);
        }
    }
    fclose(output_file2);
    return 0;
}

