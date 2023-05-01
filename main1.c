#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int size=4;;
void reduce(char (*str)[100])
{
    for (int i = 0; i < size; i++)
    {
        printf("%s\n", str[i]);
    }

char *tok ,*tok1;
char *list_M[100][100*3];
    char *list_N[100][100*3];
    int m1 = 0, n1 = 0;
    int m2=0, n2=0;
    
    
    for (int i = 0; i < size; i++)
    
    {
            char *line = str[i];
            char *line1 = str[i];

            tok = strtok(line,"|");
            char *tok2 =  tok;
            while(tok2!= NULL)
            {
                 tok1 = strtok(tok2,":");
                 printf("tok1 : %s\n ",tok1);
                 while(tok1 != NULL)
                 {
                    if (strcmp(tok1,"M") == 0 )
                 {
                    list_M[m1][m2++] = strdup("M");
                    list_M[m1][m2++] = strdup(strtok(NULL, ","));
                    list_M[m1][m2++] = strdup(strtok(NULL, ","));
                    m1++;
                    m2=0;
            
                } 
                
                else if (strcmp(tok1, "N") == 0) 
                {
                    list_N[n1][n2++] = strdup("N");
                    list_N[n1][n2++] = strdup(strtok(NULL, ","));
                    list_N[n1][n2++] = strdup(strtok(NULL, ","));
                    n1 += 1;
                    n2=0;
                }
                tok1 = strtok(NULL,",");
                }

                  printf("final : %s\n ",tok);

     tok2 = strtok(NULL,"|");
  


            }    
        
          /*  printf("Line NUMBER : %d \n", i ); 
tok1 = strtok(line1,"|");
   while(tok1 != NULL)
   {
            printf("tok1 : %s\n ",tok1);
            if (strcmp(tok1,"M") == 0 )
            {
                 list_M[m1][m2++] = strdup("M");
                list_M[m1][m2++] = strdup(strtok(NULL, ","));
                list_M[m1][m2++] = strdup(strtok(NULL, ","));
                m1++;
                m2=0;
                printf("mmmm");
            
            }
             else if (strcmp(tok1, "N") == 0) {
               list_N[n1][n2++] = strdup("N");
                list_N[n1][n2++] = strdup(strtok(NULL, ","));
               list_N[n1][n2++] = strdup(strtok(NULL, ","));
                n1 += 1;
                n2=0;
                 printf("nnn");
            }
             tok1 = strtok(NULL,"|");
 }
         
            tok = strtok(NULL, "|");
//}
    */
        
}



 printf("M values:\n");
    for (int i = 0; i < m1; i++) {
        for(int j=  0;j < 3 ;j++){ 
        printf("%s ", list_M[i][j]);
        }
        printf("\n");
    }
    printf("\n");

     printf("N values:\n");
    for (int i = 0; i < n1; i++) {
        for(int j=  0;j < 3 ;j++){ 
        printf("%s ", list_N[i][j]);
        }
        printf("\n");
    }
    printf("\n");

}

int main()
{
    char arr[100][100] =
    {
        "1,1:M,1,1|M,2,2|N,1,1|N,1,2 ",
        "1,2:M,1,1|M,2,2|N,1,1|N,1,2",
        "2,1:M,1,1|M,2,2|N,1,1|N,1,2 ",
        "2,2:M,1,2|M,2,2|N,1,1|N,1,2"
    };

    reduce(arr);

    return 0;
}
