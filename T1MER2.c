#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 100 

//function prototypes
long int getTime(struct timeval);
void launch(char*, char*, pid_t, pid_t, int, int);
void launch2(char*, pid_t, int);

           
 /*********************************************************************************************/
int main()
{
    int i;
    
    char* fileName1 = "T1MER2_a.csv";			    //FILENAME  TO BE USED
    char* fileName2 = "T1MER2_b.csv";			    //FILENAME2 TO BE USED
    pid_t pid1;							
    pid_t pid2;

	FILE* output1 = fopen(fileName1, "w");		    //Create a file to write to for first child process
    FILE* output2 = fopen(fileName2, "w");		    //Create a file to write to for second child process
    
	
    
	char* style = "_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_";     
	for(i = 0; i < SIZE; i++)
	 {
        printf("%s iteration %d %s\n", style, i, style);
		launch(fileName1, fileName2, pid1, pid2, 1, 2);     //Call launch() to create a process forked two times
     }//END FOR
     
}//END MAIN

/*************************************************************************/
long int getTime(struct timeval TVAL)
{
    gettimeofday(&TVAL, NULL);
    return TVAL.tv_sec * 1000000L + TVAL.tv_usec;
}//END GETTIME


/*********************************************************************************/
void launch(char* filename, char* filename2, pid_t pid, pid_t pid2, int num, int num2)
{
    struct timeval t;
    long int start = getTime(t);			
    char startTime[50];
    sprintf(startTime, "%ld", start);	//CONVERT start TO CHAR ARRAY
    
    printf("START: %ld\n", start);
    pid = fork();						//FORK CURRENT PROCESS
           
    if(pid == 0) //CHILD PROCESS
	{  
        printf("CHILD %d ID %d. PARENT ID:s %d. \n", num, getpid(),  getppid());
  		
		//Execute the executable file which will replace current child process 	/****/
        execlp("./OSL1.out", "./OSL1.out", "outFile1.txt", startTime, filename, NULL); 	
        exit(0);
    }// END IF
	else if(pid < 0)  //FORK ERROR HANDLER		
    {   
		perror("FORK ERROR"); //PRINT ERROR
        exit(-1);						
    }//END ELSE-IF
	else //PARENT PROCESS
	{ 
        printf("PARENT ID: %d \tCHILD ID: %d. \n", getpid(),  pid);
		printf("CHILD 2 FORK() BEING CREATED...\n");
		launch2(filename2, pid2, 2);	//CALL launch2(),CREATE 2ND CHILD
        waitpid(pid, NULL, 0); 			//WAIT FOR CHILD PROCESS TO COMPLETE
    }//END ELSE
}

/******************************************************************************/
 
void launch2(char* filename, pid_t pid, int num)
{
    struct timeval TVAL;
    long int start = getTime(TVAL);				
    char startTime[50];
    
    sprintf(startTime, "%ld", start);	//CONVERT start TO CHAR ARRAY
    
	printf("Start: %ld\n", start);   
    pid = fork();	//FORK CURRENT PROCESS 
           
    if(pid == 0) //CHILD PROCESS
	 {  
        printf("CHILD %d ID: %d. \tPARENT ID: %d. \n", num, getpid(),  getppid());
	//Execute the executable file which will replace current child process /*******/
        execlp("./OSL1.out", "./OSL1.out", "outFile2.txt", startTime, filename, NULL); 		
        exit(0);
     }//END IF
     
		else if(pid < 0)  //FORK ERROR HANDLER		
    {   
		perror("FORK ERROR"); //PRINT ERROR
        exit(-1);						
    }//END ELSE-IF
    
	else  //PARENT PROCESS
    {
		printf("PARENT ID: %d \tCHILD ID: %d. \n", getpid(),  pid);
        waitpid(pid, NULL, 0); //WAIT FOR CHILD 2ND PROCESS TO COMPLETE
    }//END ELSE
}//END LUANCH2
