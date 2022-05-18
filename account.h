#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#include "commonHelpers.h"

#define COUNTRY_LEN 30
#define LOGINNAME_LEN 10
#define PASSWORD_LEN 8
#define DISPLAYNAME_LEN 30

struct Demographic {
	int birthYear;
	double income;
	char country[COUNTRY_LEN + 1];
};

struct UserLogin {
	char displayName[DISPLAYNAME_LEN + 1];
	char loginName[LOGINNAME_LEN + 1];
	char password[PASSWORD_LEN + 1];
};

struct Account {
	int accountNum;
	char type;
	struct UserLogin login;
	struct Demographic demo;
};

int findAccountIndexByAcctNum(int matchAccountNumber, const struct Account* accounts, int maxNumElement, int prompt);
void getAccount(struct Account* account);
void getUserLogin(struct UserLogin* login);
void getDemographic(struct Demographic* demo);

void updateAccount(struct Account* accounts, int maxNumElement);
void updateUserLogin(struct UserLogin* login);
void updateDemographic(struct Demographic* demo);
void createNewAccount(struct Account* accounts, int maxNumElement);
int validateLogin(const struct Account* accounts, int maxNumElement);

void getLoginName(char* word);
void getPassword(char* word);
void getCountry(char* word);

void displayAccountSummaryHeader();
void displayAccountDetailHeader();
void displayAccountSummaryRecord(const struct Account* account);
void displayAccountDetailRecord(const struct Account* account);
void displayAllAccountSummaryRecords(const struct Account* accounts, int maxNumElement);
void displayAllAccountDetailRecords(const struct  Account* accounts, int maxNumElement);

int loadArchivedAccounts(struct Account archivedAccounts[], int maxNumElement);
int loadAccounts(struct Account accounts[], int maxNumElement);
int readAccounts(FILE* fp, struct Account accounts[], int maxNumElement);
int writeArchivedAccDateRecords(struct Account accounts[], int maxNumElement, const char* mode);
int writeAccountDateRecords(struct Account accounts[], int maxNumElement, const char* mode);
int appendAccountDataRecord(FILE* fp, struct Account* account);

#endif 