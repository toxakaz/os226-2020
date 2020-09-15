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

char* read_string()
{
	const int base_size = 32;

	char* input_string = (char*)malloc(sizeof(char) * base_size);
	int size = 0;

	input_string[0] = '\0';
	int input_string_size = base_size;

	for (;;)
	{
		for (; size < input_string_size; size++)
		{
			input_string[size] = getchar();
			if (input_string[size] == '\n')
				break;
		}
		if (input_string[size] == '\n')
			break;

		char* new_string = (char*)malloc(sizeof(char) * (input_string_size << 1));
		for (int i = 0; i < input_string_size; i++)
			new_string[i] = input_string[i];

		free(input_string);
		input_string = new_string;
		input_string_size <<= 1;
	}

	input_string[size++] = '\0';

	return input_string;
}

char* substring(char* string, int start_pos, int len, char del_input_string)
{
	if (len <= 0)
	{
		len = start_pos;
		for (; string[len] != '\0'; len++);
		len -= start_pos;
	}

	char* new_string = (char*)malloc(sizeof(char) * (len + 1));

	for (int i = start_pos; i < start_pos + len; i++)
		new_string[i - start_pos] = string[i];
	new_string[len] = '\0';

	if (del_input_string)
		free(string);

	return new_string;
}

char compare(char* string_1, char* string_2, char del_input_string_1, char del_input_string_2)
{
	int i = 0;
	for (; string_1[i] != '\0' && string_2[i] != '\0'; i++)
		if (string_1[i] != string_2[i])
		{
			if (del_input_string_1)
				free(string_1);
			if (del_input_string_2)
				free(string_2);
			return 0;
		}
	if (string_1[i] != string_2[i])
	{
		if (del_input_string_1)
			free(string_1);
		if (del_input_string_2)
			free(string_2);
		return 0;
	}
	if (del_input_string_1)
		free(string_1);
	if (del_input_string_2)
		free(string_2);
	return 1;
}

char*** parse(char* string, int* out_size, int** out_size_in, char del_input_string)
{
	int x = 0;
	int y = 0;
	char*** str_arrs = 0;
	int str_arrs_size = 0;
	int* str_arrs_in_size = 0;
	char end = 0;

	for (;;)
	{
		char** str_arr = 0;
		int str_arr_size = 0;
		char str_added = 0;

		for (;;)
		{
			for (x = y; string[x] == ' '; x++);
			for (y = x; string[y] != ' ' && string[y] != '\0' && string[y] != ';'; y++);

			if (x == y)
			{
				if (string[y] == ';')
				{
					for (; string[y] == ';'; y++);
					break;
				}
				else if (string[y] == '\0')
				{
					if (del_input_string)
						free(string);
					if (!str_added)
					{
						*out_size = str_arrs_size;
						*out_size_in = str_arrs_in_size;
						return str_arrs;
					}
					else
					{
						end = 1;
						break;
					}
				}
			}

			char** new_str_arr = (char**)malloc(sizeof(char*) * (str_arr_size + 1));
			for (int i = 0; i < str_arr_size; i++)
				new_str_arr[i] = str_arr[i];
			new_str_arr[str_arr_size++] = substring(string, x, y - x, 0);
			if (str_arr != 0)
				free(str_arr);
			str_arr = new_str_arr;

			if (!str_added)
			{
				int* new_str_arrs_in_size = (int*)malloc(sizeof(int) * (str_arrs_size + 1));
				for (int i = 0; i < str_arrs_size; i++)
					new_str_arrs_in_size[i] = str_arrs_in_size[i];
				new_str_arrs_in_size[str_arrs_size] = 1;
				if (str_arrs_in_size != 0)
					free(str_arrs_in_size);
				str_arrs_in_size = new_str_arrs_in_size;
				str_added = 1;
			}
			else
				str_arrs_in_size[str_arrs_size]++;

			if (string[y] == ';')
			{
				for (; string[y] == ';'; y++);
				break;
			}
		}

		if (str_added)
		{
			char*** new_str_arrs = (char***)malloc(sizeof(char**) * (str_arrs_size + 1));
			for (int i = 0; i < str_arrs_size; i++)
				new_str_arrs[i] = str_arrs[i];
			new_str_arrs[str_arrs_size++] = str_arr;
			if (str_arrs != 0)
				free(str_arrs);
			str_arrs = new_str_arrs;
		}

		if (end)
		{
			*out_size = str_arrs_size;
			*out_size_in = str_arrs_in_size;
			return str_arrs;
		}
	}
}

void free_parse(char*** parse, int size, int* size_in)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size_in[i]; j++)
			free(parse[i][j]);
		free(parse[i]);
	}
	free(parse);
}

char* concat_str_arr(char** strings, int count, char* separator, char del_input_strings, char del_input_separator)
{
	int size = 32;
	int counter = 0;
	char* out_string = (char*)malloc(sizeof(char) * size);
	for (int i = 0; i < count; i++)
		for (char separat = 0; separat < 2; separat++)
			for (int j = 0; ((strings[i][j] != '\0' || i == count - 1) && !separat) || (separator[j] != '\0' && separat); j++)
			{
				if (counter >= size)
				{
					char* new_out_string = (char*)malloc(sizeof(char) * (size << 1));
					for (int g = 0; g < size; g++)
						new_out_string[g] = out_string[g];
					free(out_string);
					out_string = new_out_string;
					size <<= 1;
				}
				out_string[counter++] = !separat ? strings[i][j] : separator[j];
				if (strings[i][j] == '\0' && !separat)
					break;
			}

	if (del_input_strings)
		for (int i = 0; i < count; i++)
			free(strings);
	if (del_input_separator)
		free(separator);

	return out_string;
}

int main(int argc, char* argv[])
{
	for (;;)
	{
		int commands_count;
		int* command_arg_count;
		char*** commands = parse(read_string(), &commands_count, &command_arg_count, 1);

		for (int i = 0; i < commands_count; i++)
		{
			if (compare(commands[i][0], "echo", 0, 0))
				code = echo(command_arg_count[i], commands[i]);
			else if (compare(commands[i][0], "retcode", 0, 0))
			{
				if (command_arg_count[i] > 1)
					printf("retcode command error\n");
				else
					retcode(command_arg_count[i], commands[i]);
			}
			else
			{
				char* str = concat_str_arr(commands[i], command_arg_count[i], " ", 0, 0);
				printf("unknown command: %s\n", str);
				free(str);
				break;
			}
		}

		free_parse(commands, commands_count, command_arg_count);
		free(command_arg_count);
	}
	return 0;
}