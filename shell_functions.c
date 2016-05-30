#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXBUFF 8012

char* takeinfo(const char *prompt);
int count_words(char *string);
char **string_array(char *string);

// Displays a prompt and returns input from the command line in the form of a string
char* takeinfo(const char *prompt)
{
	fprintf(stdout, "%s\n", prompt);
	char buff[MAXBUFF];
	long n_chars = read(0, buff, MAXBUFF);
	
	if(buff[n_chars - 1] == '\n')
		buff[n_chars - 1] = '\0';
	else
		buff[n_chars] = '\0';
	
	char *return_string = malloc(sizeof(char) * n_chars);
	memcpy(return_string, buff, n_chars);
	return return_string;	
}

// Counts the number of words in a string
int count_words(char *string)
{
	int i, count; count = 0;
	if(strlen(string) == 0)
		return 0;
	if (string[0] != ' ' && string[0] != '\n')
		count++;
	int len = strlen(string);
	printf("len %i\n", len );//debug
	for(i = 0 ; i < len - 1 ; i++)
	{
		// if char is ' ' and next is num or alph add a word. 
		if(string[i] == ' ' && string[i+1] != ' ' && string[i+1] != '\n')
			count++;

	}	
	return count;
}

// Takes a string and returns a string array 
char **string_array(char *string)
{
	int length = count_words(string);
        char **array_strings = malloc(sizeof(char *) * length );
        int i, j, in_word; in_word = 0;
        int stringlen = strlen(string) ;
        for(i = j = 0; i < stringlen; i++)
        {
//              printf("status |%c| |%i|\n", string[i], in_word); //debug
                // inside a word or multiple spaces
                if((string[i] != ' ' && in_word == 1) || (string[i] == ' ' && in_word == 0))
                        continue;
                // entry of a word
                if((string[i] != ' ' || string[i] != '\n') && in_word == 0)
                {
                        in_word = 1;
//                      printf("|%s|\n",&string[i]); //debug
                        array_strings[j++] = &string[i];
                }

		//exit of a word
                if((string[i] == ' ' || string[i] == '\n' ) && in_word == 1)
                {
                        in_word = 0;
                        string[i] = '\0';
                }
        }
        return array_strings;
}

//test main
int main(void)

{

	char *input = takeinfo("prompt: ");
	printf("1:%s\n", input);
	int n_words = 4;
	char **commands = string_array(input);
	printf("2: 1st |%s| last |%s|", commands[0], commands[n_words]);
}

