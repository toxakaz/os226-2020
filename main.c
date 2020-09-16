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

struct str
{
	int len;
	int real_len;
	char* str;
};

void str_add(struct str* str, char c)
{
	if (str->len >= str->real_len)
	{
		char* new_string = (char*)malloc(sizeof(char) * (str->real_len << 1));
		for (int i = 0; i < str->real_len; i++)
			new_string[i] = str->str[i];
		free(str->str);
		str->real_len <<= 1;
		str->str = new_string;
	}

	str->str[str->len - 1] = c;
	str->str[str->len++] = '\0';
}

struct str* init(int start_len)
{
	struct str* str = (struct str*)malloc(sizeof(struct str));
	str->len = 1;
	str->real_len = start_len;
	str->str = (char*)malloc(sizeof(char) * start_len);
	str->str[0] = '\0';

	return str;
}

void str_free(struct str* str)
{
	free(str->str);
	free(str);
}

int process(int argc, char* argv[])
{
	if (argv[0][0] == 'e')
		return echo(argc, argv);
	else
		return retcode(argc, argv);
}

struct command
{
	char** command;
	int size;
	int real_size;
};

struct command* init_command(int start_size)
{
	struct command* command = (struct command*)malloc(sizeof(struct command));
	command->size = 0;
	command->real_size = start_size;
	command->command = (char**)malloc(sizeof(char*) * start_size);
	return command;
}

void command_arg_add(struct command* command, char* str)
{
	if (command->size >= command->real_size)
	{
		char** new_command = (char**)malloc(sizeof(char*) * (command->real_size << 1));
		for (int i = 0; i < command->size; i++)
			new_command[i] = command->command[i];
		free(command->command);
		command->command = new_command;
		command->real_size <<= 1;
	}
	command->command[command->size++] = str;
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
	for (int i = 0; i < 9; i++)
		key["cdehort ;"[i]] = i;

	struct command* command = init_command(8);
	struct str* str = init(8);

	for (;;)
	{
		int state = 0;
		char error = 0;

		for (char c = getchar(); c != '\n'; c = getchar())
			str_add(str, c);
		str_add(str, ';');

		char* last_point = str->str;

		for (int i = 0; i < str->len && str->str[i] != '\0' && !error; i++)
		{
			state = automat[state][key[str->str[i]]];
			switch (automat[state][10])
			{
			case 1:
				last_point = &(str->str[i]);
				command_arg_add(command, last_point);
				break;
			case 2:
				str->str[i] = '\0';
				break;
			case 3:
				str->str[i] = '\0';
			case 0:
				code = process(command->size, command->command);
				command->size = 0;
				break;
			case 4:
				str->str[str->len - 2] = '\0';
				printf("can not process command: %s\n", last_point);
				code = -1;
				error = 1;
			}
		}

		str->len = 1;
		str->str[0] = '\0';
	}
}