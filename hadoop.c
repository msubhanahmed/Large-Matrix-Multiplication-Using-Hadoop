#include <stdio.h>
#include <string.h>
#include <stdlib.h>

long int matrix_size = 0;


char* read_file(char* filename) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);
    string[fsize] = '\0';
    return string;
}

long int count_tokens(char data[])
{
    long int line_count = 0;
    char *token = strtok(data, "\n");
    while (token != NULL) 
    {
        line_count++;
        //printf("%s\n",token);
        token = strtok(NULL, "\n");
    } 
    return line_count;
}

int map (char data[])
{
    char datac[strlen(data)+1];
    strcpy(datac,data);

    long int line_count = count_tokens(data);
    printf("Number of Tokens: %d\n",line_count);
    char tokens[line_count][100];

    
    long int i = 0;
    for (char *token = strtok(datac, "\n"); token != NULL; token = strtok(NULL, "\n")) 
    {
        strcpy(tokens[i], token);
        i++;
    }



    for (long int j=0 ; j<line_count ; j++)
    {
        char* token = malloc(strlen(tokens[j])*sizeof(char));
        strcpy(token,tokens[j]);
        token[strcspn(token, "\r")] = 0;
    
        char* row;char* col;char* value;char* name;

        name= strtok(token, ",");
        row = strtok(NULL, ",");
        col = strtok(NULL, ",");
        value = strtok(NULL, ",");
        
        for (int i=1 ; i<=matrix_size ; i++)
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
    long int size = 10;


    matrix_size = size;


    char* matrix1 = read_file("M.txt");
    //char* matrix2 = read_file("N.txt");

    map(matrix1);
    
    return 0;
}

