#include <mpi.h>
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

char* map (char * data1 , int dsize )
{
	int line_count = 0;

    // Count the Number of Lines recieved
	for(int i=0 ; data1[i]!='\0' ; i++)
	{
		if(data1[i] == '\n')
		{
			line_count++;
		}
	}
	
    // Allocate required memory
    char tokens[line_count][100];
	char* returnarray = malloc(sizeof(char)*line_count*100);
	strcpy(returnarray, "");
	int i = 0;
    // Seprate Each token and save it in an array 
	for (char *token = strtok(data1, "\n"); token != NULL; token = strtok(NULL, "\n")) 
	{
	    strcpy(tokens[i], token);
	    i++;
	}

   // For each token generate array[i][k] pair 
    for ( int j=0 ; j<line_count ; j++)
    {
        char* token = malloc(strlen(tokens[j])*sizeof(char));
        strcpy(token,tokens[j]);

        // remove "\r"
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

            //printf("%s\n",keyvaluepair);
            strcat(returnarray , keyvaluepair);
            strcat(returnarray , "\n");
            
        }
    }
    strcat(returnarray , "\0");
    return returnarray;
}
char** combiner ( char* data, int msize , int comsize)
{
	int keys_count = (msize*msize);
    char keys[keys_count][10];
    //char tokens[keys_count][msize*msize*20];
    
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
        for(int j=0; j<(msize*msize*20); j++)
        {
        	tokens[i][j] = '\0';
        }
        strcpy(keys[i],"");  
    }


	int i = 0;
	for (char *token = strtok(data, "\n"); token != NULL; token = strtok(NULL, "\n")) 
	{
        char first[10];
        strncpy(first, token, strcspn(token, ":"));
        first[strcspn(token, ":")] = '\0';
        int found = 0;
        for(int j=0 ; j<i ; j++)
        {
            if (strcmp(keys[j],first) == 0)
            {
                found = 1;
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
    

    printf("Keys Count: %d\n",keys_count);
    //for(int k=0 ;k<keys_count;k++)
    //{
    
    //    printf("%s\n",tokens[k]);  
    //}
    return tokens;
}



int main(int argc, char *argv[]) 
{
	int rank;	
	int comsize; 
	char maparray[comsize][100];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comsize);
    char** reduceInput;
    //printf("Rank: %d , Size: %d\n",rank,comsize);
	int displacement[comsize];
	int counts[comsize];
	
	char name[20];
	int name_len;
	MPI_Get_processor_name(name,&name_len);
	
    int size = 2;
    matrix_size = size;

	if (rank == 0)
    {
    	printf("Map Job %d Running on %s\n",rank, name);
		
		
		char* matrix1 = read_file("M.txt");
		char* matrix2 = read_file("N.txt");
		strcat(matrix1,matrix2);
		
		char data1[strlen(matrix1)];	
		strcpy(data1,matrix1);
		
		int line_count = count_tokens(matrix1);
		
		printf("Number of Tokens: %d\n",line_count);
		
		int lines_proc = line_count/comsize;
		
		
		int j=0;
		printf("Lines Proc %d\n",lines_proc);
		
	
		for(int i=0; i<comsize ; i++)
		{
			int k=0;
			int send_size = 0;
			displacement[i] = j;
			while(k!=lines_proc)
			{
				if(data1[j] == '\n')
				{
					k++;
				}
				j++;
				send_size++;
			}
			counts[i] = send_size;
		}
		for(int i=0 ; i<comsize; i++)
		{
			printf("Count %d \t Displacement %d \n" , counts[i],displacement[i]);
		}
		
		MPI_Scatter(counts, 1, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(displacement, 1, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
		
		for (int i = 1; i < comsize; i++) 
		{
            MPI_Send(&data1[displacement[i]], counts[i], MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }





        char recv_buffer[counts[0]+1];
        recv_buffer[counts[0]] = '\0';
        
        for(int i=0; i<counts[0] ; i++)
        {
        	recv_buffer[i] = data1[i];
        }
        printf("Recieved: \n%s\n",recv_buffer);
        
        char* output = map(recv_buffer,counts[0]+1);	
		
		counts[0] = strlen(output);
		displacement[0] = 0;
		
		int total_count = counts[0];
		
		for(int i=1; i<comsize ; i++)
		{
			MPI_Recv(counts+i, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total_count += counts[i];
		}
		
		char* mapoutput = malloc((total_count+1)*sizeof(char));
		
		int k=0;
		for(int i=0; i<counts[0] ; i++)
		{
			maparray[0][i] = output[i];
			mapoutput[k] = output[i];
			k++;
		}
		
		maparray[0][counts[0]] = '\0';
		printf("Output: \n%s\n",maparray[0]);
		for(int i=1; i<comsize ; i++)
		{
			MPI_Recv(maparray[i], counts[i], MPI_CHAR, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			maparray[i][counts[i]] = '\0';
			printf("Output: \n%s\n",maparray[i]);
			for(int j=0; j<counts[i] ; j++)
			{
				mapoutput[k] = maparray[i][j];
				k++;
			}
			
		}
		
		reduceInput = combiner(mapoutput,matrix_size,comsize);
		
		for (int i = 0; i < matrix_size*matrix_size; i++) 
		{
			printf("\nLength: %ld\n",strlen(reduceInput[i]));
			for (int j = 0; j < strlen(reduceInput[i]); j++) 
			{
				printf("%c", reduceInput[i][j]);
			}
			printf("\n");
		}

    }
	else
	{
	
		printf("Map Job %d Running on %s\n",rank, name);
		
		MPI_Scatter(NULL, 0, MPI_INT, &counts[rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(NULL, 0, MPI_INT, &displacement[rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
		
        char recv_buffer[counts[rank]+1];
        recv_buffer[counts[rank]] = '\0';
        MPI_Recv(recv_buffer, counts[rank], MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        
        printf("Expected: %d \t Actual: %ld \n" , counts[rank] , strlen(recv_buffer));
		printf("Recieved:\n%s\n",recv_buffer);
		
		
		char* output = map(recv_buffer,counts[rank]+1);
		int osize = strlen(output);
		
		MPI_Send(&osize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(output,osize,MPI_CHAR,0,1,MPI_COMM_WORLD);
		printf("Rank: %d Sending back :%d\n",rank,osize);
		free(output);
		
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	/*
	if (rank == 0)
	{
		printf("Scattering Data to Processes for Reduce Job\n");
		int keys_sent = 0;
		int keys_count = matrix_size*matrix_size;
		int keys_reducer = keys_count/comsize;
		for (int i = 1; i < comsize; i++) 
		{
			if(keys_sent>=keys_count)
			{
				break;
			}
			for(int j=0; j<keys_reducer ;j++)
			{
				int s_size = strlen(reduceInput[keys_sent]);
				char key[s_size+1];
				for(int l=0; l<s_size;l++)
				{
					key[l] = reduceInput[keys_sent][l];
					printf("%c",key[l]);
				}
				key[s_size] = '\0';
				
	            MPI_Send(key, s_size, MPI_CHAR, i, 3, MPI_COMM_WORLD);
	            keys_sent++;
	        }
        }
        

	}
	else
	{
		//printf("Reduce Job %d Running on %s\n",rank, name);
		char* buffer = (char*) malloc((matrix_size*30)*sizeof(char));
		
		MPI_Recv(buffer, matrix_size*29, MPI_CHAR, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		
		printf("Rank %d received\n", rank);
		for (int i = 0; i < matrix_size*30; i++) 
		{
		    printf("%c", buffer[i]);
		}
		printf("\n");
			
		
		
	}
	*/
	
	
	//MPI_Barrier(MPI_COMM_WORLD);
	printf("Goodbye From %d\n",rank);
	MPI_Finalize();
    return 0;
}
