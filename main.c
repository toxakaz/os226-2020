#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int code = 0;		//the last return code belongs to the entire program space

int echo(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		printf("%s%c", argv[i], i == argc - 1 ? '\n' : ' ');
	}
	return argc - 1;
}

int retcode(int argc, char* argv[])
{
	printf("%d\n", code);
	return code;
}

void str_add(char** str, int* len, int* real_len, char c)
{
	if (*len >= *real_len)
	{
		char* new_string = (char*)malloc(sizeof(char) * (*real_len << 1));
		for (int i = 0; i < *real_len; i++)
			new_string[i] = (*str)[i];
		free(*str);
		*real_len <<= 1;
		*str = new_string;
	}

	(*str)[*len] = c;
	*len += 1;
}

void command_arg_add(char*** command, int* size, int* real_size, char* str)
{
	if (*size >= *real_size)
	{
		char** new_command = (char**)malloc(sizeof(char*) * (*real_size << 1));
		for (int i = 0; i < *size; i++)
			new_command[i] = (*command)[i];
		free(*command);
		*command = new_command;
		*real_size <<= 1;
	}

	(*command)[*size] = str;
	*size += 1;
}

int main(int argc, char* argv[])
{
	int automat[19][11] =
	{
		18, 18, 1, 18, 18, 9, 18, 0, 0, 18, -1,
		2, 18, 18, 18, 18, 18, 18, 18, 18, 18, 1,
		18, 18, 18, 3, 18, 18, 18, 18, 18, 18, -1,
		18, 18, 18, 18, 4, 18, 18, 18, 18, 18, -1,
		18, 18, 18, 18, 18, 18, 18, 5, 16, 18, -1,
		6, 6, 6, 6, 6, 6, 6, 8, 17, 6, 2,
		7, 7, 7, 7, 7, 7, 7, 5, 16, 7, 1,
		7, 7, 7, 7, 7, 7, 7, 5, 16, 7, -1,
		6, 6, 6, 6, 6, 6, 6, 8, 17, 6, -1,
		18, 18, 10, 18, 18, 18, 18, 18, 18, 18, 1,
		18, 18, 18, 18, 18, 18, 11, 18, 18, 18, -1,
		12, 18, 18, 18, 18, 18, 18, 18, 18, 18, -1,
		18, 18, 18, 18, 13, 18, 18, 18, 18, 18, -1,
		18, 14, 18, 18, 18, 18, 18, 18, 18, 18, -1,
		18, 18, 15, 18, 18, 18, 18, 18, 18, 18, -1,
		18, 18, 18, 18, 18, 18, 18, 16, 16, 18, -1,
		18, 18, 1, 18, 18, 9, 18, 0, 0, 18, 3,
		18, 18, 1, 18, 18, 9, 18, 0, 0, 18, 0,
		18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 4
	};

	int key[2 << (sizeof(char) << 3)];
	for (int i = 0; i < 2 << (sizeof(char) << 3); i++)
		key[i] = 9;
	for (int i = 0; i < strlen("cdehort ;"); i++)
		key["cdehort ;"[i]] = i;

	int command_real_size = 32;
	char** command = (char**)malloc(sizeof(char*) * command_real_size);
	int command_size = 0;

	int str_real_len = 32;
	char* str = (char*)malloc(sizeof(char) * str_real_len);
	int str_len = 0;

	for (;;)
	{
		int state = 0;
		char error = 0;

		for (char c = getchar(); c != '\n'; c = getchar())
			str_add(&str, &str_len, &str_real_len, c);
		str_add(&str, &str_len, &str_real_len, ';');

		char* last_point = str;

		for (int i = 0; i < str_len && !error; i++)
		{
			state = automat[state][key[str[i]]];
			switch (automat[state][10])
			{
			case 1:
				last_point = str + i;
				command_arg_add(&command, &command_size, &command_real_size, last_point);
				break;
			case 2:
				str[i] = '\0';
				break;
			case 3:
				str[i] = '\0';
			case 0:
				if (command[0][0] == 'e')
					code = echo(command_size, command);
				else
					code = retcode(command_size, command);
				command_size = 0;
				break;
			case 4:
				str[str_len - 1] = '\0';
				printf("can not process command: %s\n", last_point);
				code = -1;
				error = 1;
			}
		}

		str_len = 0;
	}
}