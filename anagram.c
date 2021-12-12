#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	int length;
	char value[128];
	int amountOfLetters[26];
} Word;

int defineWord(Word* word, const char* value)
{
	word->length = strlen(value);
	if(word->length > 127)
	{
		printf("Word too long\n");
		return 0;
	}

	int i = 0;
	while(i < word->length)
	{
		word->value[i] = value[i];
		i++;
	}
	word->value[i] = '\0';

	for(i = 0; i < 26; i++)
	{
		word->amountOfLetters[i] = 0;
	}

	for(i = 0; i < word->length; i++)
	{
		char letter = word->value[i];
		if((letter >= 'A' && letter <= 'Z') || (letter >= 'a' && letter <= 'z'))
		{
			if(letter > 'Z')
			{
				letter -= 32;
			}
			word->amountOfLetters[letter - 'A']++;
		}
	}

	return 1;
}

void getWordParams(Word* word)
{
	printf("length: %d\n", word->length);
	printf("value: %s\n", word->value);
	printf("letters\n");
	for(int i = 0; i < 26; i++)
	{
		printf("%c: %d\n", 'A' + i, word->amountOfLetters[i]);
	}
}

int isAnagram(Word* word, Word* anagram)
{
	for(int i = 0; i < 26; i++)
	{
		if(word->amountOfLetters[i] != anagram->amountOfLetters[i])
		{
			return 0;
		}
	}
	return 1;
}

int main(void)
{
	int stop = 0;
	while(stop == 0)
	{
		printf("Anagram checker\n");
		printf("c - Compare 2 user-specified words\n");
		printf("f - Find possible anagrams for a user-specified word\n");
		printf("q - Quit\n");
		char op = getchar();
		while(getchar() != '\n');
		switch(op)
		{
			case 'c':
			case 'C':
				Word word, anagram;
				char str1[128], str2[128];
				int lastChar;

				printf("\nEnter a word\n");
				fgets(str1, 128, stdin);
				lastChar = strlen(str1) - 1;
				if(str1[lastChar] == '\n')
				{
					str1[lastChar] = '\0';
				}

				printf("\nEnter another word\n");
				fgets(str2, 128, stdin);
				lastChar = strlen(str2) - 1;
				if(str2[lastChar] == '\n')
				{
					str2[lastChar] = '\0';
				}

				defineWord(&word, str1);
				defineWord(&anagram, str2);

				printf("\n");
				if(isAnagram(&word, &anagram) == 1)
				{
					printf("%s is an anagram of %s\n", anagram.value, word.value);
				}
				else
				{
					printf("%s is not an anagram of %s\n", anagram.value, word.value);
				}
				printf("\n");
				break;

			case 'f':
			case 'F':
				FILE* fptr = fopen("database", "r");
				if(fptr == NULL)
				{
					printf("Failed to load database file\n");
				}

				Word* database = malloc(sizeof (Word));
				int n = 0;
				char buffer[128];
				while(fgets(buffer, 128, fptr))
				{
					lastChar = strlen(buffer) - 1;
					if(buffer[lastChar] == '\n')
					{
						buffer[lastChar] = '\0';
					}
					database = realloc(database, sizeof (Word) * (++n));
					defineWord(&database[n-1], buffer);
				}
				fclose(fptr);

				char str[128];
				printf("\nEnter which word you would like to find anagrams for\n");
				fgets(str, 128, stdin);
				lastChar = strlen(str) - 1;
				if(str[lastChar] == '\n')
				{
					str[lastChar] = '\0';
				}

				Word comparison;
				defineWord(&comparison, str);

				printf("\nPossible anagrams:\n");
				for(int i = 0; i < n; i++)
				{
					if(isAnagram(&comparison, &database[i]) == 1 && strcmp(comparison.value, database[i].value) != 0)
					{
						printf("%s\n", database[i].value);
					}
				}
				printf("\n");

				free(database);
				break;

			case 'q':
			case 'Q':
				stop = 1;
				break;
		}
	}

	return 0;
}
