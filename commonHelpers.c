#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "commonHelpers.h"

//promt user input and validates the value entered is an integer
int getInteger(void)
{
	int value;
	char newLine;
	int keepGoing = 1;

	do {
		newLine = 'x';

		scanf("%d%c", &value, &newLine);

		if (newLine == '\n' && value % 1 == 0) {
			keepGoing = 0;
		}
		else
		{
			clearStandardInputBuffer();
			printf("ERROR: Value must be an integer: ");
		}

	} while (keepGoing == 1);

	return value;
}

//promt user input and validates the value entered is a positive integer value
int getPositiveInteger(void) {

	int value;
	int keepGoing = 1;

	do {
		value = getInteger();

		if (value > 0) {
			keepGoing = 0;
		}
		else {
			printf("ERROR: Value must be a positive integer greater than zero: ");
		}

	} while (keepGoing == 1);

	return value;
}

//promt user input and validates the value entered is a double
double getDouble(void) {
	double value;
	char newLine;
	int keepGoing = 1;
	
	do {
		newLine = 'x';

		scanf("%lf%c", &value, &newLine);
		
		if (newLine == '\n' && ((int)value != value || value == 0.0)) {
			keepGoing = 0;
		}
		else {
			if (newLine != '\n')
			{
				clearStandardInputBuffer();
			}
			printf("ERROR: Value must be a double floating-point number: ");
	
		}

	} while (keepGoing == 1);

	return value;
}

//promt user input and validates the value entered is a positive double
double getPositiveDouble(void)
{
	double value;
	int keepGoing = 1;

	do {
		value = getDouble();

		if (value > 0) {
			keepGoing = 0;
		}
		else {
			printf("ERROR: Value must be a positive double floating-point number: ");
		}

	} while (keepGoing == 1);

	return value;
}

//promt user input and validates the value entered is between the two arguments received by the function
int getIntFromRange(int minValue, int maxValue)
{

	int value;
	int keepGoing = 1;

	do {
		value = getInteger();

		if (value >= minValue && value <= maxValue) {
			keepGoing = 0;
		}
		else {
			printf("ERROR: Value must be between %d and %d inclusive: ", minValue, maxValue);
		}

	} while (keepGoing == 1);

	return value;
}

//promt user input and validate the entered character matches any of the characters in the received C string argument
char getCharOption(const char validCharacters[])
{
	int keepGoing = 1;
	int checkCharacter;
	int i;
	int letterLenth;
	char singleChar;
	char newLine;

	letterLenth = strlen(validCharacters);

	do {
		newLine = 'x';

		scanf("%c%c", &singleChar, &newLine);

		for (i = 0, checkCharacter = 0; checkCharacter == 0 && i < letterLenth; i++) {
			if (singleChar == validCharacters[i]) {
				checkCharacter = 1;
			}
		}

		if (newLine == '\n' && checkCharacter == 1) {
			keepGoing = 0;
		}
		else
		{
			if (newLine != '\n') {
				clearStandardInputBuffer();
			}
			printf("ERROR: Character must be one of [%s]: ", validCharacters);
		}

	} while (keepGoing == 1);

	return singleChar;
}

//promt and validate user input for a C string value with a length (number of characters) between minStringLength and maxStringLength
void getCString(char* word, int minStringLength, int maxStringLength)
{
	int lengthOfEnterWord;
	int keepGoing = 1;
	char enterWord[MAX_WORD +1] = { '\0' };

	do
	{
		scanf("%[^\n]*c", enterWord);
		clearStandardInputBuffer();

		lengthOfEnterWord = strlen(enterWord);

		if (lengthOfEnterWord >= minStringLength && lengthOfEnterWord <= maxStringLength) {
			keepGoing = 0;
		}
		else
		{
			if (minStringLength == maxStringLength) {
				printf("ERROR: String length must be exactly %d chars: ", minStringLength);
			}
			else if (lengthOfEnterWord > maxStringLength) {
				printf("ERROR: String length must be no more than %d chars: ", maxStringLength);
			}
			else {
				printf("ERROR: String length must be between %d and %d chars: ", minStringLength, maxStringLength);
			}
		}

	} while (keepGoing == 1);

	strcpy(word, enterWord);
}

// Get the current 4-digit year from the system
int currentYear(void)
{
	time_t currentTime = time(NULL);
	return localtime(&currentTime)->tm_year + 1900;
}

// Empty the standard input buffer 
void clearStandardInputBuffer(void)
{
	while (getchar() != '\n')
	{
		; // On purpose: do nothing
	}
}

// Pause execution until user enters the enter key 
void pauseExecution(void) 
{
	printf("<< ENTER key to Continue... >>"); clearStandardInputBuffer(); putchar('\n');
}

//Get string to be displayed depends on the status
char* statusDisplayString(const int ticketStatus) 
{
	char* statusString = "CLOSED";

	if (ticketStatus == 1) 
	{
		statusString = "ACTIVE";
	}
	return statusString;
}

//Get string to be displayed depends on the accout type
char* typeDisplayString(const char accountType) {
	char* typeString = "CUSTOMER";

	if (accountType == 'A') 
	{
		typeString = "AGENT";
	}
	return typeString;
}

//Get confrim (confirmOption(1): YyNn, confirmOption(2): YN)
int getConfirm(const int confirmOption) 
{
	int YesOrNo;
	char confirm;
	const char confirmOption1[5] = "YyNn";    
	const char confirmOption2[3] = "YN";
	
	//if the option is "YyNn"
	if (confirmOption == 1)
	{
		confirm = getCharOption(confirmOption1);
		if (confirm == 'Y' || confirm == 'y') {
			YesOrNo = 1;
		}
		else {
		YesOrNo = 0;
		}
	}
	//if the option is "YN"
	else {
		confirm = getCharOption(confirmOption2);
		if (confirm == 'Y') {
			YesOrNo = 1;
		}
		else {
			YesOrNo = 0;
		}
	}
	return YesOrNo; //1:Yes, 2:No
}
