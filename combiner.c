#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int matrix_size = 0;
int line_count = 0;


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

int count_tokens(char data[])
{
    int line_count = 0;
    char *token = strtok(data, "\n");
    while (token != NULL) 
    {
        line_count++;
        token = strtok(NULL, "\n");
    } 
    return line_count;
}

char** combiner (int msize , int comsize)
{
	int keys_count = (msize*msize);
    char keys[keys_count][10];
    //char tokens[keys_count][msize*msize*20];
    char* data = read_file("ReduceInput.txt");
    char **tokens = malloc(keys_count * sizeof(char *));
    if (tokens == NULL) {
        printf("Failed to allocate memory for string array.\n");
        return NULL;
    }
    
    for (int i = 0; i < keys_count; i++) {
        tokens[i] = malloc((msize*msize*20) * sizeof(char));
        if (tokens[i] == NULL) {
            printf("Failed to allocate memory for string at index %d.\n", i);
            return NULL;
        }
        memset(tokens[i], 0, (msize*msize*20) + 1);
    }
    for(int i=0;i<keys_count;i++)
    {
        strcpy(tokens[i],"");
        strcpy(keys[i],"");  
    }


	int i = 0;
	for (char *token = strtok(data, "\n"); token != NULL; token = strtok(NULL, "\n")) 
	{
        char first[10];
        strncpy(first, token, strcspn(token, ":"));
        first[strcspn(token, ":")] = '\0';
        //printf("Key: %s\n",first);
        int found = 0;
        for(int j=0 ; j<i ; j++)
        {
            //printf("Key1: %s , Key2: %s , %d \n",first,keys[j],strcmp(keys[j],first));
            if (strcmp(keys[j],first) == 0)
            {
                found = 1;
                //printf("Found Key: %s ---- %s \n",keys[j],first);
                strncat(tokens[j],token,strlen(token));
                strcat(tokens[j],"\n");
                break;
            }
        }
        if(found == 0)
        {
            strcpy(keys[i],first);
            strncat(tokens[i],token,strlen(token));
            strcat(tokens[i],"\n");
            i++;
        }
	}
    int keys_reducer = keys_count/comsize;
    int keys_written = 0;
    if(keys_count<comsize)
    {
        keys_reducer = 1;
    }    

    // printf("Keys Count: %d\n",keys_count);
    // for(int k=0 ;k<keys_count;k++)
    // {
    //     printf("%s",tokens[k]);
    // }
    for(int i=0; i<comsize;i++)
    {
        
        if(keys_written>=keys_count)
        {
            break;
        }
        //printf("Keys for Process: %d\n",i);
        char filename[10];
        int r = sprintf(filename, "%d", i);
        filename[r] ='.';
        filename[r+1] ='t';
        filename[r+2] ='x';
        filename[r+3] ='t';
        filename[r+4] ='\0';
        
        FILE *ofile = fopen(filename, "w");
        if (ofile == NULL) {
            printf("Error opening output file.\n");
            return NULL;
        }
        else
        {
            for(int j=0 ; j<keys_reducer ; j++)
            {
                fprintf(ofile, "%s",tokens[keys_written]);
                //printf("%s",tokens[keys_written]);
                keys_written++;
            }
        }
    }
    if(keys_count>comsize)
    {
        //printf("Writing Remaing Keys\n");
        int remaining = keys_count%comsize;
        char filename[10];
        int r = sprintf(filename, "%d", comsize-1);
        filename[r] ='.';
        filename[r+1] ='t';
        filename[r+2] ='x';
        filename[r+3] ='t';
        filename[r+4] ='\0';
        
        FILE *ofile = fopen(filename, "w");
        for(int i=0; i<remaining;i++)
        {
            fprintf(ofile, "%s",tokens[keys_written]);
            keys_written++;
        }
    }
    return tokens;
}

int main(int argc, char *argv[]) 
{
    int size = 2;
    int comsize = 2;

    char results[2][100] =
    {
    "1,1:M,1,2\n1,2:M,1,2\n1,1:M,2,7\n1,2:M,2,7\n2,1:M,1,1\n2,2:M,1,1\n2,1:N,1,1\n2,2:N,1,1",
    "2,1:M,2,8\n2,2:M,2,8\n1,1:N,1,5\n1,2:N,1,5\n1,1:N,2,3\n1,2:N,2,3\n2,1:N,2,0\n2,2:N,2,0"
    };
    char** data = combiner(2,2);

    for(int i=0; i<4; i++)
    {
        printf(data[i]);
    }
    return 0;
}
