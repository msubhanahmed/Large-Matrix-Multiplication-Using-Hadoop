#include <stdio.h>
#include <string.h>

void map (char data[] , int lines )
{
    char line[20];
    while (fgets(line, sizeof(line), input_file) != NULL) 
    {
        line[strcspn(line, "\n")] = 0;
    
        char* row;
        char* col;
        char* value;
        char* name;

        name= strtok(line, ",");
        row = strtok(NULL, ",");
        col = strtok(NULL, ",");
        value = strtok(NULL, ",");
        
        for (int i=1 ; i<=size ; i++)
        {
            char keyvaluepair[20];
        
            char iter[10];
            sprintf(iter, "%d", i);;


            strcpy(keyvaluepair, row);
            strcat(keyvaluepair, ",");
            strcat(keyvaluepair, iter);
            strcat(keyvaluepair, ":");
            strcat(keyvaluepair, name);
            strcat(keyvaluepair, ",");
            strcat(keyvaluepair, col);
            strcat(keyvaluepair, ",");
            strcat(keyvaluepair, value);

            printf("%s\n",keyvaluepair);
            
        }
    }
}
int main() {
    long int size = 0;
    printf("Enter the size of matrix : ");
    scanf("%ld", &size);
    
    FILE *input_file = fopen("M.txt", "r");
    if (input_file == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }
    FILE *file2 = fopen("N.txt", "r");
    if (input_file == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    char line[20];
    while (fgets(line, sizeof(line), input_file) != NULL) 
    {
        line[strcspn(line, "\n")] = 0;
    
        char* row;
        char* col;
        char* value;
        char* name;

        name= strtok(line, ",");
        row = strtok(NULL, ",");
        col = strtok(NULL, ",");
        value = strtok(NULL, ",");
        
        for (int i=1 ; i<=size ; i++)
        {
            char keyvaluepair[20];
        
            char iter[10];
            sprintf(iter, "%d", i);;


            strcpy(keyvaluepair, row);
            strcat(keyvaluepair, ",");
            strcat(keyvaluepair, iter);
            strcat(keyvaluepair, ":");
            strcat(keyvaluepair, name);
            strcat(keyvaluepair, ",");
            strcat(keyvaluepair, col);
            strcat(keyvaluepair, ",");
            strcat(keyvaluepair, value);

            printf("%s\n",keyvaluepair);
            
        }
    }
    
    fclose(input_file);


    while (fgets(line, sizeof(line), file2) != NULL) 
    {
        line[strcspn(line, "\n")] = 0;
    
        char* row;
        char* col;
        char* value;
        char* name;

        name= strtok(line, ",");
        row = strtok(NULL, ",");
        col = strtok(NULL, ",");
        value = strtok(NULL, ",");
        
        for (int i=1 ; i<=size ; i++)
        {
            char keyvaluepair[20];
        
            char iter[10];
            sprintf(iter, "%d", i);;


            strcpy(keyvaluepair, row);
            strcat(keyvaluepair, ",");
            strcat(keyvaluepair, iter);
            strcat(keyvaluepair, ":");
            strcat(keyvaluepair, name);
            strcat(keyvaluepair, ",");
            strcat(keyvaluepair, col);
            strcat(keyvaluepair, ",");
            strcat(keyvaluepair, value);

            printf("%s\n",keyvaluepair);
            
        }
    }
    
    fclose(input_file);

    return 0;
}

