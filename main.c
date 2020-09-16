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

char compare(char* str_1, char* str_2)
{
	int i = 0;
	for (; str_1[i] != '\0' && str_2[i] != '\0'; i++)
		if (str_1[i] != str_2[i])
			return 0;
	if (str_1[i] != str_2[i])
		return 0;
	return 1;
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

char* str_get_string(struct str* str)
{
	char* string = (char*)malloc(sizeof(char) * str->len);
	for (int i = 0; i < str->len; i++)
		string[i] = str->str[i];
	return string;
}

void str_free(struct str* str)
{
	free(str->str);
	free(str);
}

void str_reinit(struct str** str, int start_len)
{
	str_free(*str);
	*str = init(start_len);
}

void command_arg_add(char*** command, int* size, char* new_arg)
{
	char** new_command = (char**)malloc(sizeof(char*) * (*size + 1));
	for (int i = 0; i < *size; i++)
		new_command[i] = (*command)[i];
	if (*size)
		free(*command);
	*command = new_command;
	(*command)[(*size)++] = new_arg;
}

void command_free(char** command, int size)
{
	for (int i = 0; i < size; i++)
		free(command[i]);
	free(command);
}

int process(int* argc, char* argv[])
{
	int code = -1;
	if (compare(argv[0], "echo"))
		code = echo(*argc, argv);
	else if (compare(argv[0], "retcode"))
		code = retcode(*argc, argv);

	command_free(argv, *argc);
	*argc = 0;
	return code;
}

int main(int argc, char* argv[])
{
	int automat[17][11] =
	{
		16, 16, 1, 16, 16, 8, 16, 0, 0, 16, 0,
		2, 16, 16, 16, 16, 16, 16, 16, 16, 16, 1,
		16, 16, 16, 3, 16, 16, 16, 16, 16, 16, 1,
		16, 16, 16, 16, 4, 16, 16, 16, 16, 16, 1,
		16, 16, 16, 16, 16, 16, 16, 5, 15, 16, 1,
		6, 6, 6, 6, 6, 6, 6, 7, 0, 6, 2,
		6, 6, 6, 6, 6, 6, 6, 5, 15, 6, 1,
		6, 6, 6, 6, 6, 6, 6, 7, 0, 6, -1,
		16, 16, 9, 16, 16, 16, 16, 16, 16, 16, 1,
		16, 16, 16, 16, 16, 16, 10, 16, 16, 16, 1,
		11, 16, 16, 16, 16, 16, 16, 16, 16, 16, 1,
		16, 16, 16, 16, 12, 16, 16, 16, 16, 16, 1,
		16, 13, 16, 16, 16, 16, 16, 16, 16, 16, 1,
		16, 16, 14, 16, 16, 16, 16, 16, 16, 16, 1,
		16, 16, 16, 16, 16, 16, 16, 15, 15, 16, 1,
		16, 16, 1, 16, 16, 8, 16, 0, 0, 16, 10,
		16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 3
	};

	int key[2 << (sizeof(char) << 3)];
	for (int i = 0; i < 2 << (sizeof(char) << 3); i++)
		key[i] = 9;
	for (int i = 0; i < 9; i++)
		key["cdehort ;"[i]] = i;

	for (;;)
	{
		int state = 0;
		char c = getchar();
		char error = 0;

		char** command = 0;
		int size = 0;

		struct str* str = init(8);
		struct str* error_str = init(32);

		int i = 1;

		while (c != '\n')
		{
			str_add(error_str, c);

			state = automat[state][key[c]];
			switch (automat[state][10])
			{
			case 0:
				if (size > 0)
				{
					str_reinit(&error_str, 32);
					code = process(&size, command);
				}
				break;

			case 1:
				str_add(str, c);
				break;

			case 2:
				command_arg_add(&command, &size, str_get_string(str));
				str_reinit(&str, 8);
				break;

			case 3:
				if (!error)
					error = 1;
				break;

			case 10:
				command_arg_add(&command, &size, str_get_string(str));
				str_reinit(&str, 8);
				str_reinit(&error_str, 32);
				code = process(&size, command);
				break;
			}

			c = getchar();
		}

		if (error)
		{
			printf("can not process: %s", error_str->str);
			if (size > 0)
				command_free(command, &size);
		}
		else if (str->len > 1 || size > 0)
		{
			if (str->len > 1)
				command_arg_add(&command, &size, str_get_string(str));
			code = process(&size, command);
		}

		str_free(str);
		str_free(error_str);
	}
}