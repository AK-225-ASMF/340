#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define MAXBUFF 8012

// Creates a structure that contains the floppy disks vital info and characteristics
typedef struct floppy_disk
{
        char *name;
        unsigned short number_of_fats;          // #16 size = 1
        unsigned short sectors_used_per_fat;    // #22 size = 2
        unsigned short sectors_per_cluster;     // #13 size = 1
        unsigned short root_entries;            // #17 size = 2
        unsigned short bytes_per_sector;        // #11 size = 2

} floppy;

// floppy functions
void structure(floppy *myfloppy);
void print_file_info(unsigned char file_buff[]);
void traverse(floppy *myfloppy, int sector, int flag, char *current_directory);
void mount(floppy *myfloppy , char *name);
void help(void);
void showfat(floppy *myfloppy);

// shell functions
void print_array(char *array[]);

char* takeinfo(const char *prompt);
int count_words(char *string);
char **string_array(char *string);

//--------------------------------------------------------------------------------

int main(void)
{	
	// Create Floppy element
        floppy newfloppy;
        floppy *myfloppy = &newfloppy;

	unsigned short mounted = 0;
	while(1)
	{
		char current_directory[150] = "/";
		// Create a buffer and take input from the command line
		char *buff = NULL;
		buff = takeinfo("prompt: ");
		
		// Create a String array and parse the buff into it
		char **buff_parsed;
		buff_parsed = string_array(buff);
		int n_words = count_words(buff);
		printf("n_words %i\n", n_words);//debug
		// If an empty command line is entered
		if(n_words < 1)
		{
			continue;
		}
		
		//-----------------------------------------------------------------------------------------		
		
		// Run the comands.
		// Check to see if the comand is quit
		if(!strcmp(buff_parsed[0], "quit"))
		{
				exit(1);
		}
		else if(!strcmp(buff_parsed[0], "help"))
		{
			help();
		}
		// Execute comands	
		else if(!strcmp(buff_parsed[0], "clear"))
		{
			if(fork() == 0)
			{
				char *args[2] = { "clear", NULL};
				execv("/usr/bin/clear", args);
				exit(1);
			}
			else
			{
				wait(NULL);
			}
		}
		else if(!strcmp(buff_parsed[0], "fmount"))
		{	
			char temp[strlen(buff_parsed[1]) + 2];
			memcpy(temp,"./", 2);
			
			memcpy(temp, buff_parsed[1], strlen(buff_parsed[1]));
			if( access( buff_parsed[1], F_OK ) != -1 ) 
			{
				fprintf(stderr,"Floppy succesfully mounted\n");
				mount(myfloppy, buff_parsed[1]);
				mounted = 1;
			}
			else 
			{
				//file doesn't exist
				fprintf(stderr,"File/Floppy can't be found\n");
			}
			
		}
		else if(!strcmp(buff_parsed[0], "fumount"))
		{
			if(mounted == 0)
				printf("There's nothing to unmount\n");
			else
			{
				mounted = 0;
				myfloppy = NULL;
				fprintf(stderr,"Unmount succesfull\n");
			}
		}
		else if(!strcmp(buff_parsed[0], "traverse"))
		{
			if(n_words > 1)
			{	// if the -l flag is used print with additional info
				if(!strcmp(buff_parsed[1], "-l"))
				{
					traverse(myfloppy, 19, 1, current_directory);
				}
				else
					fprintf(stderr, "flag doesn't exist");
			}else
			{
				traverse(myfloppy, 19, 0, current_directory);
			}
		}
		else if(!strcmp(buff_parsed[0], "structure"))
		{
			structure(myfloppy);
		}
		else if(!strcmp(buff_parsed[0], "showfat"))
		{
			showfat(myfloppy);
		}
		else
		{
			fprintf(stderr,"This command doesn't exist\n");
		}
	}	
}
