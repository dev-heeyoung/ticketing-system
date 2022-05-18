#ifndef COMMON_HELPERS_H_
#define COMMON_HELPERS_H_

#define MAX_WORD 150

int getInteger(void);
int getPositiveInteger(void);
double getDouble(void);
double getPositiveDouble(void);
int getIntFromRange(int minValue, int maxValue);
char getCharOption(const char validCharacters[]);
void getCString(char* word, int minStringLength, int maxStringLength);

int currentYear(void);
void clearStandardInputBuffer(void);
void pauseExecution(void);
char* statusDisplayString(const int ticketStatus);
char* typeDisplayString(const char accountType);
int getConfirm(const int confirmOption);

#endif 