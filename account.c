#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "account.h"

#define ACCOUNTDATAFILE "accounts.txt"
#define ARC_ACCOUNTFILE "accounts_arc.txt"

//Search the array received in argument 2 for a match on a desired account number
int findAccountIndexByAcctNum(int matchAccountNumber, const struct Account* accounts, int maxNumElement, int prompt) 
{
	int i, enteredAccountNum;
	int found = 0;
	int findArrayIndex = -1;

	if (prompt == 0) {
		enteredAccountNum = matchAccountNumber;
	}
	else {
		printf("Enter the account#: ");
		enteredAccountNum = getPositiveInteger();
	}

	for (i = 0; found == 0 && i < maxNumElement; i++) {
		if (accounts[i].accountNum == enteredAccountNum) {
			findArrayIndex = i;
			found = 1;
		}
	}
	return findArrayIndex;
}

// Get new record for account info (account number & account type)
void getAccount(struct Account* account) 
{
	const char typeOption[3] = "AC";

	printf("New Account Data (Account#:%d)\n", account->accountNum);
	printf("----------------------------------------\n");

	printf("Enter the account type (A=Agent | C=Customer): ");
	account->type = getCharOption(typeOption);
}

// Get new record for user login info (login name & display name, password)
void getUserLogin(struct UserLogin* login) 
{
	printf("User Login Data Input\n");               
	printf("----------------------------------------\n"); 

	getLoginName(login->loginName);

	printf("Enter the display name (%d chars max): ", DISPLAYNAME_LEN);
	getCString(login->displayName, 1, DISPLAYNAME_LEN);

	getPassword(login->password);
}

// Get new record for user demographic info (birth year & income & country)
void getDemographic(struct Demographic* demo) 
{
	const int MIN_AGE = 18;
	const int MAX_AGE = 110;
	int currentYearValue;

	printf("Demographic Data Input\n");               
	printf("----------------------------------------\n");

	printf("Enter birth year (current age must be between %d and %d): ", MIN_AGE, MAX_AGE);
	currentYearValue = currentYear(); // Get the current 4-digit year from the system
	demo->birthYear = getIntFromRange(currentYearValue - MAX_AGE, currentYearValue - MIN_AGE);

	printf("Enter the household Income: $");
	demo->income = getPositiveDouble();

	getCountry(demo->country);
}

void updateAccount(struct Account* accounts, int maxmaxNumElement) 
{
	int selection, arrayIndexNum;
	const char agentOption[3] = "AC";

	arrayIndexNum = findAccountIndexByAcctNum(0, accounts, maxmaxNumElement, 1);
	putchar('\n');

	if (arrayIndexNum != -1) {

		do
		{
			printf("Account: %d - Update Options\n", accounts[arrayIndexNum].accountNum);
			printf("----------------------------------------\n");
			printf("1) Update account type (current value: %c)\n", accounts[arrayIndexNum].type);
			printf("2) Login\n");
			printf("3) Demographics\n");
			printf("0) Done\n");
			printf("Selection: ");

			selection = getIntFromRange(0, 3);
			putchar('\n');

			switch (selection)
			{
			case 0:
				break;
				//Update account type
			case 1:
				printf("Enter the account type (%C=Agent | %C=Customer): ", agentOption[0], agentOption[1]);
				accounts[arrayIndexNum].type = getCharOption(agentOption);
				break;
			case 2:
				updateUserLogin(&(accounts[arrayIndexNum].login));
				break;
			case 3:
				updateDemographic(&(accounts[arrayIndexNum].demo));
				break;
			default:
				printf("ERROR: Invalid selection!\n\n");
				break;
			}
		} while (selection);
	}
	else {
		printf("ERROR:  Access Denied.\n\n");
	}
}

//Display a Update user login menu (display name & password)
void updateUserLogin(struct UserLogin* login) {
	int selection;
	char loginedUser[DISPLAYNAME_LEN + 1] = { '\0' };

	strcpy(loginedUser, login->displayName);

	do
	{
		printf("User Login: %s - Update Options\n", login->loginName);
		printf("----------------------------------------\n");
		printf("1) Display name (current value: %s)\n", login->displayName);
		printf("2) Password\n");
		printf("0) Done\n");
		printf("Selection: ");

		selection = getIntFromRange(0, 2);
		putchar('\n');

		switch (selection)
		{
			//Exit the function
		case 0:
			break;
			//Update display name
		case 1:
			printf("Enter the display name (%d chars max): ", DISPLAYNAME_LEN);
			getCString(login->displayName, 1, DISPLAYNAME_LEN);
			putchar('\n');
			break;
			//Update password
		case 2:
			getPassword(login->password);
			putchar('\n');
			break;
		default:
			printf("ERROR: Invalid selection!\n\n");
			break;
		}
	} while (selection);
}

//Display a Update demographic menu (income & country)
void updateDemographic(struct Demographic* demo) 
{
	int selection;

	do
	{
		printf("Demographic Update Options\n");
		printf("----------------------------------------\n");
		printf("1) Household Income (current value: $%.2lf)\n", demo->income);
		printf("2) Country (current value: %s)\n", demo->country);
		printf("0) Done\n");
		printf("Selection: ");

		selection = getIntFromRange(0, 2);
		putchar('\n');

		switch (selection)
		{
			//Exit function
		case 0:
			break;
			//Update household income
		case 1:
			printf("Enter the household Income: $");
			demo->income = getPositiveDouble();
			putchar('\n');
			break;
			//Update country
		case 2:
			getCountry(demo->country);
			break;
		default:
			printf("ERROR: Invalid selection!\n\n");
			break;
		}
	} while (selection);
}

//create a new account
void createNewAccount(struct Account* accounts, int maxNumElement) 
{
	int i, arrayIndexNum, highestAccountNum;
	highestAccountNum = 0;
	struct Account tempAccount = { 0 };
	
	arrayIndexNum = findAccountIndexByAcctNum(0, accounts, maxNumElement, 0);

	//Add new information if there's an empty index in the accounts array
	if (arrayIndexNum != -1) {

	//Automatically set the number for the new account 
		for (i = 0; i < maxNumElement; i++) {
			if (accounts[i].accountNum > highestAccountNum) {
				highestAccountNum = accounts[i].accountNum;
			
			}
		}
		tempAccount.accountNum = highestAccountNum + 1;

		//Get input for all the information of a new user
		getAccount(&tempAccount);
		putchar('\n');
		getUserLogin(&tempAccount.login);
		putchar('\n');
		getDemographic(&tempAccount.demo);

		accounts[arrayIndexNum] = tempAccount;
		printf("*** New account added! ***\n\n");
		pauseExecution();
	}
	else {
		printf("ERROR: Account listing is FULL, call ITS Support!\n\n");
		pauseExecution();
	}
}

//Validate login
int validateLogin(const struct Account* accounts, int maxNumElement) 
{
	int accountArrayIndex, remainAttempt, compareLogin, comparePassword, keepAttempt;
	compareLogin = comparePassword = 0;
	accountArrayIndex = -1;
	keepAttempt = 1;

	char enteredPassword[MAX_WORD] = { 0 };
	char enteredLoginName[MAX_WORD] = { 0 };

	remainAttempt = 3;

	do 
	{
		accountArrayIndex = findAccountIndexByAcctNum(0, accounts, maxNumElement, 1);

		printf("User Login    : ");
		scanf("%[^\n]*c", enteredLoginName);
		clearStandardInputBuffer();
		printf("Password      : ");
		scanf("%[^\n]", enteredPassword);
		clearStandardInputBuffer();

		//check if the entered login name and password match with the data with the valid entered account number
		compareLogin = strcmp(accounts[accountArrayIndex].login.loginName, enteredLoginName);
		comparePassword = strcmp(accounts[accountArrayIndex].login.password, enteredPassword);

		if (compareLogin == 0 && comparePassword == 0) 
		{
			keepAttempt = 0;
			putchar('\n');
		}
		else 
		{
			remainAttempt -= 1;
			accountArrayIndex = -1;
			printf("INVALID user login/password combination! [attempts remaining:%d]\n\n", remainAttempt);
		}

		//If login fails after 3 attemps, go to the main menu
		if (remainAttempt == 0) 
		{
			keepAttempt = 0;
			printf("ERROR:  Login failed!\n\n");
			pauseExecution();
		}

	} while (keepAttempt);

	return accountArrayIndex;
}

//Get a vaild login name
void getLoginName(char* word) 
{
	int i, checkSpace;
	int keepGoing = 1;

	do {
		checkSpace = 0;

		printf("Enter user login (%d chars max): ", LOGINNAME_LEN);
		getCString(word, 1, LOGINNAME_LEN);

		for (i = 0; word[i] != '\0'; i++) 
		{
			if (isspace(word[i])) {
				checkSpace = 1;
			}
		} if (checkSpace) 
		{
			printf("ERROR:  The user login must NOT contain whitespace characters.\n");
		}
		else {
			keepGoing = 0;
		}

	} while (keepGoing);
}

//Get a vaild password
void getPassword(char* word) 
{
	int i, j, digitCount, upperCaseCount, lowercaseCount, symbolCount, keepGoing;
	const char symbolArray[9] = { '!', '@', '#', '$', '%', '^', '&', '*' };

	do 
	{
		digitCount = upperCaseCount = lowercaseCount = symbolCount = 0;
		keepGoing = 1;

		printf("Enter the password (must be %d chars in length): ", PASSWORD_LEN);
		getCString(word, PASSWORD_LEN, PASSWORD_LEN);

		for (i = 0; word[i] != '\0'; i++) 
		{
			if (isdigit(word[i])) 
			{
				digitCount++;
			}
			else if (isupper(word[i])) 
			{
				upperCaseCount++;
			}
			else if (islower(word[i])) 
			{
				lowercaseCount++;
			}
			else 
			{
				for (j = 0; j < 8; j++) 
				{
					if (word[i] == symbolArray[j]) 
					{
						symbolCount++;
					}
				}
			}
		}

		if (digitCount == 2 && upperCaseCount == 2 && lowercaseCount == 2 && symbolCount == 2) 
		{
			keepGoing = 0;
		}
		else {
			printf("SECURITY: Password must contain 2 of each:\n");
			printf("          Digit: 0-9\n");
			printf("          UPPERCASE character\n");
			printf("          lowercase character\n");
			printf("          symbol character: %s\n", symbolArray);
		}

	} while (keepGoing);
}

//Get a vaild country name
void getCountry(char* word) 
{
	int i;

	printf("Enter the country (%d chars max.): ", COUNTRY_LEN);
	getCString(word, 1, COUNTRY_LEN);	
	putchar('\n');

	for (i = 0; word[i] != '\0'; i++) {
		word[i] = toupper(word[i]);
	}
}

//Display a formatted table header for displayAccountSummaryRecord
void displayAccountSummaryHeader() {
	printf("Acct# Acct.Type Birth\n");
	printf("----- --------- -----\n");
}

//Display a formatted table header for displayAccountDetailHeader
void displayAccountDetailHeader() {
	printf("Acct# Acct.Type Birth Income      Country    Disp.Name       Login      Password\n");
	printf("----- --------- ----- ----------- ---------- --------------- ---------- --------\n");
}

//Display a formatted record for account number, account type, birth year
void displayAccountSummaryRecord(const struct Account* account) {

	char* typeString;

	typeString = typeDisplayString(account->type);

	printf("%05d %-9s %5d\n", account->accountNum, typeString, account->demo.birthYear);
}

//Display a formatted record for the detailed info
void displayAccountDetailRecord(const struct Account* account) {

	char* typeString;
	int i;
	char hiddenPwd[PASSWORD_LEN + 1] = { '*', '*', '*' , '*' , '*' , '*' , '*' , '*' };

	//Check the account type so that it can represent the account type ¡®A¡¯: AGENT, ¡®C¡¯: CUSTOMER)
	typeString = typeDisplayString(account->type);

	//Change the password(copied) with asterisks(*) used for every-other character displayed
	for (i = 0; i < PASSWORD_LEN; i += 2) {
		hiddenPwd[i] = account->login.password[i];
	}
	printf("%05d %-9s %5d $%10.2lf %-10s %-15s %-10s %8s\n", account->accountNum, typeString, account->demo.birthYear, account->demo.income, account->demo.country, account->login.displayName, account->login.loginName, hiddenPwd);
}

//Display a summary view of all the valid account records
void displayAllAccountSummaryRecords(const struct Account* accounts, int maxNumElement) {

	int i;

	displayAccountSummaryHeader();

	for (i = 0; i < maxNumElement; i++) {
		if (accounts[i].accountNum > 0) {
			displayAccountSummaryRecord(&accounts[i]);
		}
	}
	putchar('\n');
	pauseExecution();
}

//Display a detailed view of all the valid account records
void displayAllAccountDetailRecords(const struct Account* accounts, int maxNumElement) {

	int i;

	displayAccountDetailHeader();

	for (i = 0; i < maxNumElement; i++) {
		if (accounts[i].accountNum > 0) {
			displayAccountDetailRecord(&accounts[i]);
		}
	}
	putchar('\n');
	pauseExecution();
}

//Load archived accounts records
int loadArchivedAccounts(struct Account archivedAccounts[], int maxNumElement)
{
	int recCount = 0;

	FILE* fp = fopen(ARC_ACCOUNTFILE, "r");

	if (fp != NULL)
	{
		recCount = readAccounts(fp, archivedAccounts, maxNumElement);
		// close the file
		fflush(fp); 
		fclose(fp);
		fp = NULL;
	}

	return recCount;
}

//Load accounts records
int loadAccounts(struct Account accounts[], int maxNumElement)
{
	int recCount = 0;

	FILE* fp = fopen(ACCOUNTDATAFILE, "r");

	if (fp != NULL)
	{
		recCount = readAccounts(fp, accounts, maxNumElement);

		fflush(fp); 
		fclose(fp);
		fp = NULL;
	}
	else {
		puts("ERROR: UNABLE TO ACCESS FILE!!!\n");
	}

	return recCount;
}

//Read account records
int readAccounts(FILE* fp, struct Account accounts[], int maxNumElement)
{
	int recCount = 0, fieldCount;

	if (fp != NULL)
	{
		do
		{
			fieldCount = fscanf(fp, "%d~%c~%30[^~]~%10[^~]~%8[^~]~%d~%lf~%30[^\n]",
				&accounts[recCount].accountNum,
				&accounts[recCount].type,
				accounts[recCount].login.displayName,
				accounts[recCount].login.loginName,
				accounts[recCount].login.password,
				&accounts[recCount].demo.birthYear,
				&accounts[recCount].demo.income,
				accounts[recCount].demo.country);
			if (fieldCount == 8)
			{
				recCount++;
			}
		} while (!feof(fp) && recCount < maxNumElement);
	}
	else {
		puts("ERROR: UNABLE TO ACCESS FILE!!!\n");
	}

	return recCount;
}

//Write removed account records in "account_arc.txt"
int writeArchivedAccDateRecords(struct Account accounts[], int maxNumElement, const char* mode)
{
	int i, recCount = 0;

	FILE* fp = fopen(ARC_ACCOUNTFILE, mode);

	if (fp != NULL)
	{
		for (i = 0; i < maxNumElement; i++)
		{
			recCount += appendAccountDataRecord(fp, &accounts[i]);
		}
		fflush(fp); 
		fclose(fp);
		fp = NULL;
	}
	else {
		puts("ERROR: UNABLE TO ACCESS FILE!!!\n");
	}

	return recCount;
}

//Write account records in "ticket.txt"
int writeAccountDateRecords(struct Account accounts[], int maxNumElement, const char* mode)
{
	int i, recCount = 0;

	FILE* fp = fopen(ACCOUNTDATAFILE, mode);

	if (fp != NULL)
	{
		for (i = 0; i < maxNumElement; i++)
		{
			recCount += appendAccountDataRecord(fp, &accounts[i]);
		}
		fflush(fp); 
		fclose(fp);
		fp = NULL;
	}
	else {
		puts("ERROR: UNABLE TO ACCESS FILE!!!\n");
	}

	return recCount;
}

//Append an account record
int appendAccountDataRecord(FILE* fp, struct Account* account)
{
	int result = 0;

	if (fp != NULL)
	{
		// Field Delimeter: '~' 
		if (account->accountNum > 0) {
			result = fprintf(fp, "%d~%c~%s~%s~%s~%d~%lf~%s",
				account->accountNum,
				account->type,
				account->login.displayName,
				account->login.loginName,
				account->login.password,
				account->demo.birthYear,
				account->demo.income,
				account->demo.country);
			result = fputc('\n', fp);
		}
	}
	else {
		puts("ERROR: UNABLE TO ACCESS FILE!!!\n");
	}

	return result > 0 ? 1 : 0;
}
