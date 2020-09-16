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

char** command_create(int* index, int size, char* str)
{
	char** command = (char**)malloc(sizeof(char*) * size);
	for (int i = 0; i < size; i++)
		command[i] = str + index[i];
	return command;
}

void index_add(int** index, int* size, int* real_size, int in)
{
	if (*size >= *real_size)
	{
		int* new_ind = (int*)malloc(sizeof(int) * (*real_size << 1));
		for (int i = 0; i < *real_size; i++)
			new_ind[i] = (*index)[i];
		free(*index);
		*real_size <<= 1;
		*index = new_ind;
	}

	(*index)[*size] = in;
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

	int index_real_size = 8;
	int* index = (int*)malloc(sizeof(int) * index_real_size);
	int index_size = 0;
	char** command;

	int str_real_len = 8;
	char* str = (char*)malloc(sizeof(char) * str_real_len);
	int str_len = 0;

	int state = 0;
	char error = 0;
	int error_point = 0;
	char end = 0;

	char* buffer = (char*)malloc(sizeof(char) * 2);

	while (fgets(buffer, 2, stdin))
	{
		char c = buffer[0];
		if (c == '\n' || c == '\0')
		{
			c = ';';
			end = 1;
		}

		if (!(error && end))
		{

			str_add(&str, &str_len, &str_real_len, c);
			if (error)
				continue;

			state = automat[state][key[c]];
			switch (automat[state][10])
			{
			case 1:
				error_point = str_len - 1;
				index_add(&index, &index_size, &index_real_size, str_len - 1);
				break;
			case 2:
				str[str_len - 1] = '\0';
				break;
			case 3:
				str[str_len - 1] = '\0';
			case 0:
				command = command_create(index, index_size, str);
				if (command[0][0] == 'e')
					code = echo(index_size, command);
				else
					code = retcode(index_size, command);
				free(command);
				index_size = 0;
				str_len = 0;
				error_point = 0;
				break;
			case 4:
				error = 1;
			}
		}
		else
			printf("invalid input: ", str + error_point);

		if (end)
		{
			state = 0;
			error = 0;
			error_point = 0;
			end = 0;
			index_size = 0;
			str_len = 0;
		}
	}

	free(index);
	free(str);
	free(buffer);

	return 0;
}