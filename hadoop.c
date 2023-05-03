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

int main(int argc, char *argv[]) 
{
	int rank;	
	int comsize; 
	
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comsize);
    
    //printf("Rank: %d , Size: %d\n",rank,comsize);
	int displacement[comsize];
	int counts[comsize];
	
	
    int size = 2;
    matrix_size = size;

	if (rank == 0)
    {
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
        printf("%s\n",recv_buffer);
        
        
		//char* output = map(recv_buffer,strlen(recv_buffer));	
        char* output = map(recv_buffer,counts[0]+1);	
		
		counts[0] = strlen(output);
		displacement[0] = 0;
		int max = counts[0];
		
		for(int i=1; i<comsize ; i++)
		{
			MPI_Recv(counts+i, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if(counts[i]>max)
			{
				max = counts[i];
			}
		}
		
		
		char maparray[comsize][max];
		for(int i=0; i<counts[0] ; i++)
		{
			maparray[0][i] = output[i];
		}
		
		printf("%s\n",output);
		
		
		for(int i=1; i<comsize ; i++)
		{
			//MPI_Recv(maparray[i], counts[i], MPI_CHAR, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//printf("%s\n",maparray[i]);
		}
		
		
		
    }
	else
	{
	
		MPI_Scatter(NULL, 0, MPI_INT, &counts[rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(NULL, 0, MPI_INT, &displacement[rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
		
        char recv_buffer[counts[rank]+1];
        recv_buffer[counts[rank]] = '\0';
        MPI_Recv(recv_buffer, counts[rank], MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        
        //printf("Expected: %d \t Actual: %ld \n" , counts[rank] , strlen(recv_buffer));
		printf("%s\n",recv_buffer);
		
		
		char* output = map(recv_buffer,counts[rank]+1);
		int osize = strlen(output);
		printf("%s\n",output);
		MPI_Send(&osize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		//MPI_Send(output,osize,MPI_CHAR,0,1,MPI_COMM_WORLD);
		
		
			
		
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
    return 0;
}
