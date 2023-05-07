/*
//------------------------------------------------
Muhammad Subhan(20I0873)
Jawad Ahmed(20I0945)
CS-D
Parallel and Distributed Computing 
Project 
//-----------------------------------------------------
*/


#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int matrix_size = 0;
int line_count = 0;



//file reading using  a function read_file
//-----------------------------------------------------

char* read_file(char* filename) 
{
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


//-----------------------------------------------------
int count_tokens(char data[])
{
    int line_count = 0;
    char *token = strtok(data, "\n");       //tokenizing the data on '\n'
    while (token != NULL) 
    {
        line_count++;
        token = strtok(NULL, "\n");     
    } 
    return line_count;                          //total lines and  tokens for processing the map / reduce function 
}


//----------------------------------------------------------------------

//Map-Function : 


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
        

        //continuously concatenate the keyvalue pair
        for (int i=1 ; i<=matrix_size ; i++)
        {
            char keyvaluepair[20];
        
            char iter[10];
            sprintf(iter, "%d", i);;
			if(strcmp(name,"M") == 0)  //IF M is found during Tokenizing of line
            {
                    strcpy(keyvaluepair, row);
                    strcat(keyvaluepair, ",");
                    strcat(keyvaluepair, iter);
                    strcat(keyvaluepair, ":");
                    strcat(keyvaluepair, name);
                    strcat(keyvaluepair, ",");
                    strcat(keyvaluepair, col);
                    strcat(keyvaluepair, ",");
                    strcat(keyvaluepair, value);

                    printf("M: %s\n",keyvaluepair);
                    strcat(returnarray , keyvaluepair);
                    strcat(returnarray , "\n");
            }
            else if (strcmp(name,"N")==0)   //IF N is found during Tokenizing of line
            {
                    strcpy(keyvaluepair, iter);
                    strcat(keyvaluepair, ",");
                    strcat(keyvaluepair, row);
                    strcat(keyvaluepair, ":");
                    strcat(keyvaluepair, name);
                    strcat(keyvaluepair, ",");
                    strcat(keyvaluepair, col);
                    strcat(keyvaluepair, ",");
                    strcat(keyvaluepair, value);

                    printf("N: %s\n",keyvaluepair);
                    strcat(returnarray , keyvaluepair);
                    strcat(returnarray , "\n");
            }
            else
            {
            	printf("No matching matrix name found!!\n");
            }
        }
    }
    strcat(returnarray , "\0");
    return returnarray;
}

//Combiner funciton to allocate 


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



//Reducer Function 
//------------------------------------------------------------------------------------------------
void reduce(char * data ) {

    char * ndata = malloc(strlen(data)*sizeof(char));
    strcpy(ndata,data);   //copy the data string to ndata 

    int count = count_tokens(ndata);
    int c2 = count;  

    free(ndata);
    char list[count][strlen(data)];

    int k=0;
   // printf("%ld",strlen(data));
    for(int i=0; i<count ; i++)
    {
        if(k>= strlen(data))
        {
            break;
        }
        for(int j=0; data[k]!='\n';j++)
        {
            if(k>= strlen(data))
            {
                break;
            }
            list[i][j] = data[k];
            list[i][j+1] = '\0';
            k++;
        }
        k++;
    }

    //Creating 2D double pointer char array to store their Respective Values 
    char** list_M = (char**)malloc(count * sizeof(char*));
    for (int i = 0; i < count; i++) {
        list_M[i] = (char*)malloc(count * sizeof(char));
    }
    char** list_N = (char**)malloc(count * sizeof(char*));
    for (int i = 0; i < count; i++) {
        list_N[i] = (char*)malloc(count * sizeof(char));
    }


    //---------------------------------------------------------
    
    int m1 = 0, n1 = 0;
    int m2=0, n2=0;
    char ch1[2] ;
    

    //Storing the values based on tokenizing of line and storing in respective M OR N array.
    //----------------------------------------------------------------------------------
                for(int i=0; i<count ; i++)
                {
                    char line[strlen(list[i])];
                    strcpy(line,list[i]);
                    line[strcspn(line, "\n")] = 0;
                    char *token = strtok(line, ":");
                    while (token != NULL) {

                        if (strcmp(token, "M") == 0)
                        {
                            
                            list_M[m1][m2++] = line[0];
                            list_M[m1][m2++] = line[2];
                            char *aa = strdup("M");
                            list_M[m1][m2++] = *aa;
                            char *bb =  strdup(strtok(NULL, ","));
                            list_M[m1][m2++] = *bb;
                            char *cc= strdup(strtok(NULL, ","));
                            list_M[m1][m2++] = *cc; 
                            m1++;
                            m2=0;
                        
                        } 
                    
                        else if (strcmp(token, "N") == 0)
                        {
                            list_N[n1][n2++] = line[0];
                            list_N[n1][n2++] = line[2];
                            char *aa= strdup("N");
                            list_N[n1][n2++] = *aa;
                            char *bb = strdup(strtok(NULL, ","));
                            list_N[n1][n2++] = *bb;
                            char *cc =  strdup(strtok(NULL, ","));
                            list_N[n1][n2++] =*cc;
                            n1 += 1;
                            n2=0;
                        }
                        token = strtok(NULL, ",");
                    }
                }
    //----------------------------------------------------------------------------------
    char keyvaluepair[20];
    int size = m1;
    int result=0;

    char distinct[m1][m1];
    for (int i = 0; i < m1; i++) {
        for (int j = 0; j < m1; j++) {
            distinct[i][j] = 0;
        }
    }
    int co = 0;
    int found=0;

    //finding thr distinct the pairs of arrays
    //----------------------------------------------------------------------------------
    for (int i = 0; i < size; i++) {
        found = 0;
        for (int j = 0; j < count; j++) {
            if ((list_M[i][0]) == distinct[j][0] && (list_M[i][1])== distinct[j][1] ){
                found = 1;
                break;
            }
        }
        if (!found) {
            distinct[co][0] = list_M[i][0];
            distinct[co][1] = list_M[i][1];
            co++;
        }
    }
    //----------------------------------------------------------------------------------

    //Multipliction of respective distinct key values 

    //----------------------------------------------------------------------------------
          
          
          
            int res = 0;
            int First=0;
            int Second=0;
            for (int i =0;i<co;i++)
            {
                char m_a  = distinct[i][0];
                char m_b = distinct[i][1];

                for (int j = 0;j < size;j++)
                {
                    char n_a = list_N[j][0];
                    char n_b = list_N[j][1];


                    if ( m_a == n_a && m_b == n_b)
                        {

                            if(list_M[j][3] == list_N[j][3])
                            {
                                First =   atoi(&list_M[j][4]);
                                Second=   atoi(&list_N[j][4]);
                                res += First*Second;
                            
                            }
                            else
                            continue;
                        }
                        
                        else
                            continue;

                }

                    printf("%c,%c : %d\n",distinct[i][0],distinct[i][1],res);
                    //fprintf(ptr,"%c,%c : %d\n",distinct[i][0],distinct[i][1],res);
                    res = 0;
            }    

     //----------------------------------------------------------------------------------
}


//------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) 
{
	int rank;	
	int comsize; 
	char maparray[comsize][100];

    //INITIALIZATION OF MPI PROCESSES

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


    //----------------------------------------------------------------------------------
	if (rank == 0)
    {
                printf("Map Job %d Running on %s\n",rank, name);
                
                
                char* matrix1 = read_file("M.txt");
                char* matrix2 = read_file("N.txt");
                strcat(matrix1,matrix2);
                
                char data1[strlen(matrix1)];	
                strcpy(data1,matrix1);
                
                int line_count = count_tokens(matrix1);
                
                //printf("Number of Tokens: %d\n",line_count);
                
                int lines_proc = line_count/comsize;
                
                
                int j=0;
                //printf("Lines Proc %d\n",lines_proc);
                
            
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
		/*
		for(int i=0 ; i<comsize; i++)
		{
			printf("Count %d \t Displacement %d \n" , counts[i],displacement[i]);
		}*/



        //scattering the differnett chunks 
		
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
        //printf("Recieved: \n%s\n",recv_buffer);
        
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
			mapoutput[k] = output[i];
			k++;
		}
		
		for(int i=1; i<comsize ; i++)
		{
			char recievearray[counts[i]];
			MPI_Recv(recievearray, counts[i], MPI_CHAR, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			maparray[i][counts[i]] = '\0';
			//printf("Output: \n%s\n",maparray[i]);
			for(int j=0; j<counts[i] ; j++)
			{
				mapoutput[k] = recievearray[j];
				//printf("%c",mapoutput[k]);
				k++;
			}
			
		}
        //COMBINER FUNCTION TO OUPUT ARRAY OF MAP :

		reduceInput = combiner(mapoutput,matrix_size,comsize);  
		
		for (int i = 0; i < matrix_size*matrix_size; i++) 
		{
			//printf("\nLength: %ld\n",strlen(reduceInput[i]));
			for (int j = 0; j < strlen(reduceInput[i]); j++) 
			{
				//printf("%c", reduceInput[i][j]);
			}
			//printf("\n");
		}

    }
	else
	{

        //THE RANK CURRENTLY PROCESSING THE MAP JOB
	
		printf("Map Job %d Running on %s\n",rank, name);                    
		
		MPI_Scatter(NULL, 0, MPI_INT, &counts[rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(NULL, 0, MPI_INT, &displacement[rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
		
        char recv_buffer[counts[rank]+1];
        recv_buffer[counts[rank]] = '\0';
        MPI_Recv(recv_buffer, counts[rank], MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        
        //printf("Expected: %d \t Actual: %ld \n" , counts[rank] , strlen(recv_buffer));
		//printf("Recieved:\n%s\n",recv_buffer);
		
		
		char* output = map(recv_buffer,counts[rank]+1);
		int osize = strlen(output);
		
		MPI_Send(&osize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(output,osize,MPI_CHAR,0,1,MPI_COMM_WORLD);
		
	}
	

	MPI_Barrier(MPI_COMM_WORLD);



	//Scattering Data to Processes for Reduce Job	
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
					//printf("%c",key[l]);
				}
				key[s_size] = '\0';
				
	            MPI_Send(key, s_size, MPI_CHAR, i, 3, MPI_COMM_WORLD);
	            keys_sent++;
	        }
        }
        int remaining_keys = keys_count - keys_sent;
        if(remaining_keys>0)
        {
        	printf("Rank 0 started Reduce Job\n");
	        int s_size = strlen(reduceInput[keys_sent])*remaining_keys;
        	char key[s_size+1];
			for(int l=0; l<s_size;l++)
			{
				key[l] = reduceInput[keys_sent][l];
				//printf("%c",key[l]);
			}
			key[s_size] = '\0';
			
			reduce(key);
        	
        }
        
        

	}
    
    //THE RANK OF PROOCESS CURRENTLY PROCESSING THE REDUCE JOB
	else
	{
		//printf("Reduce Job %d Running on %s\n",rank, name);
		char* buffer = (char*) malloc((matrix_size*30)*sizeof(char));
		
		MPI_Recv(buffer, matrix_size*29, MPI_CHAR, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		

		printf("Rank %d started Reduce Job\n", rank);
		//printf("Reduce:\n%s\n",buffer);
		
		
		reduce(buffer);
		
		
		
	}
	MPI_Barrier(MPI_COMM_WORLD);
	printf("Goodbye From %d\n",rank);
	MPI_Finalize();
    return 0;
}

