/*
 * This program writes a file with ten lines of random characters. During the write process, these 
 * random characters are stored in a two-dimensional array. Then the program reads the output file 
 * and then compares it against what's stored in local memory. Afterwards, the file is then deleted.
 *
 *
 * 	Note: 
 * 		sample cmd - 
 *              Ex.  ./app output.txt "887654321" time.csv
 *									Or
 *              Ex.  ./a.out testing.txt "7895452" t.csv
 */

#define ROW 10			//Number of lines/rows to write
#define CHARS 120		//Number of characters/columns to write
#define CHARBEGIN 65	//first readable ascii character
#define CHAREND 126		//last readable ascii character
#define TIME 100000		//Sleep time is in microseconds

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<sys/time.h>


//function prototypes
char randomChar(); //GENERATES RANDOM CHARACTERS

void writeFile(char[], char[][CHARS]); //FOR WRITING TO FILE
void readFile(char[], char[][CHARS]); //READING FROM THE FILE

void printArray(char[][CHARS]); 
void compareArrays(char[][CHARS], char[][CHARS]); //COMPARE ARRAY TO A COPY ARRAY

void printLine(int, char[][CHARS], char*); //

void deleteFile(char*);					//DELETE THE FILE AT THE END


/*********************************************************************************************
 * The main function takes in arguments and calls the writeFile(), readFile(), printArray(), *
 * compareArrays(), and deleteFile() methods that create a 2 dimensional array, fill it with *
 * random char characters, writes to file, then reads file, stores read data to another 2d   *
 * array, compares the two arrays, and deletes the file.                                     *
 * Preconditions:                                                                            *
 * Expects argv[1] to store a string filename i.e. "output.txt"                              *
 * Expects argv[2] to store the start time passed in as a string  i.e. "2345654334"          *
 * Expects argv[3] to store the output filename for the process time i.e. "outputCSV.csv"    *
 *********************************************************************************************/
int main(int argc, char* argv[])
{
   
   char  str[ROW][CHARS];  					//2D ARRAY OF SIZE ROW X CHARS	
   char copy[ROW][CHARS];  					//2D ARRAY OF EQUAL SIZE, A COPY USED FOR READING
   
   srand(time(NULL));  	   					//FOR TRUE RANDOMNESS.
   
   char* fName = argv[1];  			     	//STORE THE FILENAME AS FIRST ARGUMENT 
   char* ptr;					
   
   long start = strtol(argv[2], &ptr, 10);	//CONVERT STRING --> LONG	
   
   printf("START TIME: %ld\n", start); 
   
   struct timeval TVAL;
   gettimeofday(&TVAL, NULL);			    					//GETS TIME IN MICROSECONDS
   long int     end = (TVAL.tv_sec * 100000L + TVAL.tv_usec); 	//Convert SECONDS & USECS --> USECS
   long int    diff = (end - start);			    			//FINDS THE DIFFERENCE
   char*    csvFile = argv[3];			    					//GET TIMER OUTPUT FILENAME
   FILE*  outputCSV = fopen(csvFile, "a");	    				//OPEN CSV FILE				
   
   printf("\nCHILD TIME ENDED : %ld\n", end);
   printf("TIME DIFF: %ld\n", diff);   
   fprintf(outputCSV, "%ld,\n", diff);		    				//WRITE TIME DIFF DELIMITED BY ,& \n
   printf("TIMER WAS WRITTEN TO FILE\n");
   
   fclose(outputCSV); 				    						//CLOSE FILE

   writeFile(fName, str);  										//CALL writeFile(), WRITE RANDOM CHAR TO FILE, STORE IN str
   readFile(fName, copy);										//CALL readFile()  READ IN FILE, COPY ARRAY
   printf("\n");
   printArray(copy);											//CALL printArray() TO PRINT COPY ARRAY 
   compareArrays(str, copy);									//CALL compareArrays() TO COMPARE IF ARRAY copy == str ARRAY
   deleteFile(fName);											//CALL deleteFile() TO DELETE FILE 
   return 0;
}//END MAIN


/******************************************************/
char randomChar()
{
   char randchar;
   randchar = (char) (CHARBEGIN + rand() % (CHAREND - CHARBEGIN));
   return randchar;
}//END RANDOMALPHA


/*************************************************************/
void writeFile(char fileName[], char str[][CHARS]){
   
   int i;
   int j;
   
   FILE *output;
   
   output = fopen(fileName, "w");			//OPEN/CREATE IF FILE DOES NOT EXIST, WRITE "w" SET.
   printf("WRITING TO FILE...\n");
   
   for(i = 0; i < ROW; i++)
   {
      printf("ARRAY FILLING... %d\n", i);
      usleep(TIME);							//SYSTEM PAUSE
      
	  for(j = 0; j < CHARS; j++)
	  {  
        str[i][j] = randomChar();    		//CALL randomChar(), STORE A CHAR AT LOCATION str[i][j]
      }//END FOR
      
      fwrite(str[i], sizeof(char), sizeof(str[i]), output);	//WRITE EVERY LINE STORED IN THE ARRAY TO output FILE 
     
	  printf("NOW WRITING...\n");
     
	  fprintf(output, "%s", "\r\n");						//WRITE NEWLINE DELIMITER TO FILE
   
   }//END FOR
   printf("DONE.\n");
   fclose(output);											//CLOSE FILE
}//END WRITEFILE

/*********************************************************************************/
void readFile(char fileName[], char copy[][CHARS])
{
    int i;

    FILE *input; 
   
    input = fopen(fileName, "r");							//OPEN FILE TO BE READ
   
    while(!feof(input))
	{                                        				//WHILE EOF NOT REACHED
		if(i == 0)	
	    	fseek(input, i * 120, SEEK_SET);                //CURSOR SET TO BEGINNING OF FILE
		else 
	    	fseek(input, i * 120 + i * 2, SEEK_SET);        //SET CURSOR TO CALC POS TO ACCOMADATE FOR DELIMITERS "\r\n"
															
		printf("ftell(): %ld\n",ftell(input));				//PRINT CURSOR LOCATION TO SCREEN
        													
		usleep(TIME);										//TEMPORARY PAUSE PROGRAM (LENGTHENS PROCESSES)
															
		fread(copy[i],sizeof(char),sizeof(copy[i]),input);	//READ FILE BY EACH LINE, POPULATE THE ARRAY
		i++;
    }//END WHILE
    
    printf("READING COMPLETE\n");							
    fclose(input);											//ClOSE FILE
}//END READFILE

/**********************************************************************/
void printLine(int num, char arr[][CHARS], char* name)
{
    int i;
    printf("\n%s\n", name);
    for(i = 0; i < CHARS; i++)
	{
        printf("%c", arr[num][i]);
    }//END FOR
    printf("\n");
}//END PRINTLINE

/************************************************************************/
void printArray(char arr[][CHARS])
{
   int i, j;
   for(i = 0; i < ROW; i++)
   {
      printf("%d.\n", i);
      for(j = 0; j < CHARS; j++)
	  {
      	printf("%c", arr[i][j]);
      }//END FORFOR
      printf("\n");
   
   }//ENDFOR
}//END PRINTARRAY

/*************************************************************************/
void compareArrays(char str[][CHARS], char copy[][CHARS])
{
    int random = rand() % 10;		 			//GENERATE RANDOM # 0 - 9 FOR INDEX
    printf("\nRANDOM #: %d\n", random);
    
    printLine(random, str, "str");   			//CALL printLine(), PRINT RANDOM ROW OF str  TO SCREEN
	printLine(random, copy, "copy");			//CALL printLine(), PRINT RANDOM ROW OF copy TO SCREEN
    int flag = 1; 								//FLAG SET TO TRUE
    int i = 0;
    char* message = "STRINGS MATCH!!";     		//DEFAULT MESSAGE IF FLAG == TRUE
    
    while(i < CHARS && flag) 					//WHILE i < (# OF COLUMNS) AND FLAG == TRUE
	{                     
    
	   if(str[random][i] != copy[random][i]) //IF CHAR1 != CHAR2, FLAG = 0, CHANGE MESSAGE OUTPUT
	   {   
           flag = 0;
	   	   message = "FALSE NOT EQUAL "; 
       }//END IF        
    
	   i++;
    }//END WHILE
    printf("%s\n\n\n", message);
    
}//END COMPAREARRAYS

/******************************************************************************/
//WARNING: DONT USE 'if(!remove(fileName))' AN ERROR MAY OCCUR 
void deleteFile(char* fileName)
{
    if(remove(fileName) == 0)  			//IF REMOVE RETURNS 0 THE FILE SUCESSFULY DELETED
	        printf("FILE %s SUCCESSFULLY DELETED\n", fileName);
	else 
       		printf("ERROR: FILE FAILED TO DELETE: %s.\n", fileName);
}//END DELETEFILE

