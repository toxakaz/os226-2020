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

char** command_create(int* index, int size, char* str)
{
	char** command = (char**)malloc(sizeof(char*) * size);
	for (int i = 0; i < size; i++)
		command[i] = str + index[i];
	return command;
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

	const int max_size = 1024;

	int* index = (int*)malloc(sizeof(int) * max_size);
	int index_size;
	char** command;

	char* input_str = (char*)malloc(sizeof(char) * max_size);

	while (fgets(input_str, max_size, stdin))
	{
		int state = 0;
		int error_point = 0;
		char end = 0;
		index_size = 0;

		for (int i = 0;; i++)
		{
			if (input_str[i] == '\n' || input_str[i] == '\0')
			{
				input_str[i] = ';';
				end = 1;
			}

			state = automat[state][key[input_str[i]]];
			switch (automat[state][10])
			{
			case 1:
				error_point = i;
				index[index_size++] = i;
				break;
			case 2:
				input_str[i] = '\0';
				break;
			case 3:
				input_str[i] = '\0';
			case 0:
				command = command_create(index, index_size, input_str);
				if (command[0][0] == 'e')
					code = echo(index_size, command);
				else
					code = retcode(index_size, command);
				free(command);
				index_size = 0;
				error_point = 0;
				break;
			case 4:
				printf("can not process command: %s\n", input_str + error_point);
				code = -1;
				end = 1;
			}

			if (end)
				break;
		}
	}

	free(index);
	free(input_str);

	return 0;
}