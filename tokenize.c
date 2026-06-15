/* SPDX-License-Identifier: MIT */
/*
 * @file			tokenize.c
 * @brief			reentrant tokenization function
 * @author			Sugiura Ryusuke <https://github.com/sugiura-ryusuke>
 * @date			2026/06/15
 *
 *
 * << How to build on Ubuntu / Cygwin >>
 *
 * $ gcc tokenize.c -DUNITTEST -o tokenize
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT(fmt, ...) \
	do { \
		(void)printf(fmt, ##__VA_ARGS__); \
		(void)fflush(stdout); \
	} while(0)

#define UNITTEST_ASSERT(value) unittest_assert((value), __func__, __LINE__, ("UNITTEST_ASSERT("#value")"))
#define UNITTEST_ASSERT_EQUAL(actual, expected) unittest_assert(((actual) == (expected)), __func__, __LINE__, ("UNITTEST_ASSERT_EQUAL("#actual", "#expected")"))
#define UNITTEST_ASSERT_STRING_EQUAL(actual, expected) unittest_assert(!(strcmp((const char*)(actual), (const char*)(expected))), __func__, __LINE__, ("UNITTEST_ASSERT_STRING_EQUAL("#actual", "#expected")"))

/**
 * @brief reentrant tokenization (replacement for strtok_r, can handle empty fields)  再入可能なトークン分割(strtok_rの空フィールド対応版)
 *
 * @param[in,out] str      point to the string to be parsed  トークン分割する文字列
 * @param[in]     delim    a set of bytes that delimit the tokens in the parsed string  トークン分割に使用するバイトの集合
 * @param[in,out] saveptr  a pointer to maintain the next token after next  次の次のトークンを保持するためのポインタ
 * @retutn a pointer to the next token, or NULL if there are no more tokens  次のトークンへのポインタ、トークンがなければNULL
 * @note  The contents of str are not preserved  文字列strの内容は保持されません
 */
char *tokenize(char *str, const char *delim, char **saveptr)
{
	char *token;
	char *p;
	const char *d;

	if (delim == NULL) {
		return NULL;
	}
	if (saveptr == NULL) {
		return NULL;
	}

	token = (str != NULL) ? str : *saveptr;

	if ((token == NULL) || (*token == '\0')) {
		*saveptr = NULL;
	}
	else {
		for (p = token; *p != '\0'; p++) {
			for (d = delim; *d != '\0'; d++) {
				if (*p == *d) {
					break;
				}
			}
			if (*p == *d) {
				*p = '\0';
				p++;
				*saveptr = p;
				break;
			}
		}
		if (*saveptr != p) {
			*saveptr = NULL;
		}
	}

	return token;
}

#ifdef UNITTEST
int unittest_assert(int value, const char *func, int linenum, const char *expression)
{
	if (value) {
		PRINT("%s():%d \033[32mPASSED\033[m \033[37m%s\033[m\n", func, linenum, expression);
	}
	else {
		PRINT("%s():%d \033[31mFAILED\033[m \033[33m%s\033[m\n", func, linenum, expression);
	}

	return 0;
}

void tokenize_test_001(void)
{
	char text[] = "ABC,DEF,GHI";
	char *str = text;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 3);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "ABC");
	UNITTEST_ASSERT_STRING_EQUAL(token[1], "DEF");
	UNITTEST_ASSERT_STRING_EQUAL(token[2], "GHI");
}

void tokenize_test_002(void)
{
	char text[] = "ABC,,DEF,,GHI";
	char *str = text;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 5);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "ABC");
	UNITTEST_ASSERT_STRING_EQUAL(token[1], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[2], "DEF");
	UNITTEST_ASSERT_STRING_EQUAL(token[3], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[4], "GHI");
}

void tokenize_test_003(void)
{
	char text[] = ",ABC,,DEF,";
	char *str = text;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 5);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[1], "ABC");
	UNITTEST_ASSERT_STRING_EQUAL(token[2], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[3], "DEF");
	UNITTEST_ASSERT_STRING_EQUAL(token[4], "");
}

void tokenize_test_004(void)
{
	char text[] = "ABC,DEF,,GHI,";
	char *str = text;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 5);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "ABC");
	UNITTEST_ASSERT_STRING_EQUAL(token[1], "DEF");
	UNITTEST_ASSERT_STRING_EQUAL(token[2], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[3], "GHI");
	UNITTEST_ASSERT_STRING_EQUAL(token[4], "");
}

void tokenize_test_005(void)
{
	char text[] = ",,ABC,,DEF";
	char *str = text;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 5);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[1], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[2], "ABC");
	UNITTEST_ASSERT_STRING_EQUAL(token[3], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[4], "DEF");
}

void tokenize_test_006(void)
{
	char text[] = "A,B,C";
	char *str = text;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 3);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "A");
	UNITTEST_ASSERT_STRING_EQUAL(token[1], "B");
	UNITTEST_ASSERT_STRING_EQUAL(token[2], "C");
}

void tokenize_test_007(void)
{
	char text[] = "A,,B,,C";
	char *str = text;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 5);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "A");
	UNITTEST_ASSERT_STRING_EQUAL(token[1], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[2], "B");
	UNITTEST_ASSERT_STRING_EQUAL(token[3], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[4], "C");
}

void tokenize_test_008(void)
{
	char text[] = ",A,,B,";
	char *str = text;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 5);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[1], "A");
	UNITTEST_ASSERT_STRING_EQUAL(token[2], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[3], "B");
	UNITTEST_ASSERT_STRING_EQUAL(token[4], "");
}

void tokenize_test_009(void)
{
	char text[] = "AB;CD,EF.GH";
	char *str = text;
	const char *delim = ";,.";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 4);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "AB");
	UNITTEST_ASSERT_STRING_EQUAL(token[1], "CD");
	UNITTEST_ASSERT_STRING_EQUAL(token[2], "EF");
	UNITTEST_ASSERT_STRING_EQUAL(token[3], "GH");
}

void tokenize_test_010(void)
{
	char text[] = "AB;CD,EF.GH";
	char *str = text;
	const char *delim = "";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 1);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "AB;CD,EF.GH");
}

void tokenize_test_011(void)
{
	char text[] = ",,,";
	char *str = text;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 4);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[1], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[2], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[3], "");
}

void tokenize_test_012(void)
{
	char text[] = ",,";
	char *str = text;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 3);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[1], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[2], "");
}

void tokenize_test_013(void)
{
	char text[] = ",";
	char *str = text;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 2);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "");
	UNITTEST_ASSERT_STRING_EQUAL(token[1], "");
}

void tokenize_test_014(void)
{
	char text[] = "";
	char *str = text;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 1);
	UNITTEST_ASSERT_STRING_EQUAL(token[0], "");
}

void tokenize_test_015(void)
{
	char *str = NULL;
	const char *delim = ",";
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 0);
}

void tokenize_test_016(void)
{
	char text[] = "ABC,DEF,GHI";
	char *str = text;
	const char *delim = NULL;
	char *saveptr = NULL;
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, &saveptr);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 0);
}

void tokenize_test_017(void)
{
	char text[] = "ABC,DEF,GHI";
	char *str = text;
	const char *delim = ",";
	char *token[16];
	size_t count;

	for (count = 0; count < 16; count++) {
		token[count] = tokenize(str, delim, NULL);
		if (token[count] == NULL) {
			break;
		}
		str = NULL;
	}

	UNITTEST_ASSERT_EQUAL(count, 0);
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	tokenize_test_001();
	tokenize_test_002();
	tokenize_test_003();
	tokenize_test_004();
	tokenize_test_005();
	tokenize_test_006();
	tokenize_test_007();
	tokenize_test_008();
	tokenize_test_009();
	tokenize_test_010();
	tokenize_test_011();
	tokenize_test_012();
	tokenize_test_013();
	tokenize_test_014();
	tokenize_test_015();
	tokenize_test_016();
	tokenize_test_017();

	return EXIT_SUCCESS;
}
#endif

