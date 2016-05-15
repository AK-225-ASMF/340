#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXBUFF 8012


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

int main(void)
{

	char *input = takeinfo("prompt: ");
	printf("%s\n", input);

}
