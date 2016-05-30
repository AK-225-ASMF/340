#include <string.h>
#include <stdio.h>
#include <stdlib.h>
char **string_array(char *string, int length)
{
	char **array_strings = malloc(sizeof(char *) * length );
	int i, j, in_word; in_word = 0;
	int stringlen = strlen(string) ;
	for(i = j = 0; i < stringlen; i++)
	{
//		printf("status |%c| |%i|\n", string[i], in_word);
		// inside a word or multiple spaces
		if((string[i] != ' ' && in_word == 1) || (string[i] == ' ' && in_word == 0))
			continue;
		// entry or exit of a word
		if((string[i] != ' ' || string[i] != '\n') && in_word == 0)
		{
			in_word = 1;
//			printf("|%s|\n",&string[i]);
			array_strings[j++] = &string[i];
		}
	
		if((string[i] == ' ' || string[i] == '\n' ) && in_word == 1)
		{
			in_word = 0;
			string[i] = '\0';
		}
	}
	return array_strings;

}

int main(void)
{
	char string[] = "superman is a dick";
	char **parsed = string_array(string, 4);
	
	printf("|%s| |%s| |%s| \n", parsed[0], parsed[1], parsed[2]);

}
