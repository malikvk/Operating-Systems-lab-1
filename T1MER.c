#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 100	

//function prototypes
long int getTime(struct timeval);
void launch(char*, pid_t);


/***********************************************************************************/
int main()
{
    int i;
    char* fileName1 = "T1MER.csv";		//FILENAME TO BE USED
    pid_t pid1;							//CREATE PROCESS ID
	 
    char* style = "_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_#_"; 
    for(i = 0; i < SIZE; i++)
	{
        printf("%s LOOP #%d %s\n", style, i, style);	
        launch(fileName1, pid1);        //CALL launch(), FOR RUNNING ROUTINE OF FORKING PROCESS
    }//END FOR
}//END MAIN

/*****************************************************************************************/
long int getTime(struct timeval TVAL)
{
    gettimeofday(&TVAL, NULL);
    return TVAL.tv_sec * 1000000L + TVAL.tv_usec;
}//END GETTIME

/*****************************************************************************************/
void launch(char* filename, pid_t pid)
{
    struct timeval TVAL;
    long int start = getTime(TVAL);				
    char startTime[50];
    
    sprintf(startTime, "%ld", start);				//CONVERT start TO CHAR ARRAY
    
    printf("START: %ld\n", start);
    pid = fork();									//FORK CURRENT PROCESS
           
    if(pid == 0) //CHILD PROCESS
	{  
        printf("CHILD ID: %d. PARENT ID: %d. \n", getpid(),  getppid());
        
        //Execute the executable file which will replace current child process 
		/*********************CHANGE ./APP EXECUTABLE***********************/
        execlp("./OSL1.out", "./OSL1.out", "output.txt", startTime, filename, NULL); 	
			
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
        waitpid(pid, NULL, 0); //WAIT FOR CHILD PROCESS TO COMPLETE
    }//END ELSE

}//END LAUNCH

