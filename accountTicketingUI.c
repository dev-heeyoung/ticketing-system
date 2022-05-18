#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "accountTicketingUI.h"

#define ACCOUNT_SIZE 15

//Entry point to the application logic.
void applicationStartup(struct AccountTicketingData* data) {

	int accountArrayIndex = -1;
	struct Account account = {0};

	do {
			//Display a login menu and search the array index that matches on a desired account number
			accountArrayIndex = menuLogin(data->accounts, data->ACCOUNT_MAX_SIZE);

			if (accountArrayIndex != -1) {
				account = data->accounts[accountArrayIndex];

				//Display a agent's main menu if the type is 'A'
				if (data->accounts[accountArrayIndex].type == 'A') {
					menuAgent(data, &account);
				}
				else {
					//Display a agent's main menu if the type is 'C'
					menuClient(data->tickets, &account, data->TICKET_MAX_SIZE);
				}
			}
	} while (accountArrayIndex != -1);

	printf("==============================================\n");
	printf("Account Ticketing System - Terminated\n");
	printf("==============================================\n");
}

//Displays a login menu
int menuLogin(const struct Account* accounts, int maxNumElement) {

	int selection, confirm, accountArrayIndex;
	accountArrayIndex = -1;

	do
	{
		printf("==============================================\n");
		printf("Account Ticketing System - Login\n");
		printf("==============================================\n");
		printf("1) Login to the system\n");
		printf("0) Exit application\n");
		printf("----------------------------------------------\n\n");
		printf("Selection: ");
		
		selection = getIntFromRange(0, 1);
		putchar('\n');

		switch (selection)
		{
		//Exit application
		case 0:
			printf("Are you sure you want to exit? ([Y]es|[N]o): ");
			confirm = getConfirm(1);
			putchar('\n');
			if (confirm == 0) {
				selection = -1;
			}
			break;
		//Login to the system
		case 1: 
			accountArrayIndex = validateLogin(accounts, maxNumElement);
			if (accountArrayIndex != -1) {
				selection = 0;
			}
			break;
		default:
			printf("ERROR: Invalid selection!\n\n");
			break;
		}
		
	} while (selection);

	return accountArrayIndex;
}

//Display a agent's main menus
void menuAgent(struct AccountTicketingData* data, const struct Account* account) {

	int selection, accountRecords;
	accountRecords = 0;

	struct Account removedAccounts[ACCOUNT_SIZE] = { {0} }; 

	do
	{
		printf("AGENT: %s (%d)\n", account->login.displayName, account->accountNum);
		printf("==============================================\n");
		printf("Account Ticketing System - Agent Menu\n");
		printf("==============================================\n");
		printf(" 1) Add a new account\n");
		printf(" 2) Modify an existing account\n");
		printf(" 3) Remove an account\n");
		printf(" 4) List accounts: summary view\n");
		printf(" 5) List accounts: detailed view\n");
		printf("----------------------------------------------\n");
		printf(" 6) List new tickets\n");
		printf(" 7) List active tickets\n");
		printf(" 8) List closed tickets\n");
		printf(" 9) Manage a ticket\n");
		printf("10) Archive closed tickets\n");
		printf("----------------------------------------------\n");
		printf("11) View archived account statistics\n");
		printf("12) View archived tickets statistics\n");
		printf("----------------------------------------------\n");
		printf("0) Logout\n\n");
		printf("Selection: ");

		selection = getIntFromRange(0, 12);
		putchar('\n');

		switch (selection)
		{
			//Logout
		case 0:
			viewNumOfRecords(data);
			break;
			//Add a new account
		case 1:
			createNewAccount(data->accounts, data->ACCOUNT_MAX_SIZE);
			break;
			//Modify an existing account
		case 2:
			updateAccount(data->accounts, data->ACCOUNT_MAX_SIZE);
			break;
			//Remove an account
		case 3:
			removeAccount(data, account);
			break;
			//Display a simple summary view of all the valid account records
		case 4:
			displayAllAccountSummaryRecords(data->accounts, data->ACCOUNT_MAX_SIZE);
			break;
			//Display a detailed view of all the valid account records
		case 5:
			displayAllAccountDetailRecords(data->accounts, data->ACCOUNT_MAX_SIZE);
			break;
			//Display the new ticket (status: OPEN(1) && counter = 1)
		case 6:
			displayNewTicketRecords(data->tickets, data->TICKET_MAX_SIZE);
			break;
			//Display the active tickets (status: OPEN(1))
		case 7:
			displayActiveTicketRecords(data->tickets, data->TICKET_MAX_SIZE);
			break;
			//Display the closed tickets (status: OPEN(0))
		case 8:
			displayClosedTicketRecords(data->tickets, data->TICKET_MAX_SIZE);
			break;
			//Manage a ticket (add a message, close ticket, Re-open ticket)
		case 9:
			manageTicket(data->tickets, account->type, account->login.displayName, data->TICKET_MAX_SIZE);
			break;
			//Remove closed tickets from the system and store them in "tickets_arc.txt"
		case 10:
			archiveClosedTickets(data->tickets, data->TICKET_MAX_SIZE);
			break;
			//View the number of archived accounts
		case 11:
			accountRecords = loadArchivedAccounts(removedAccounts, data->ACCOUNT_MAX_SIZE);
			printf("There are %d account(s) currently archived.\n\n", accountRecords);
			pauseExecution();
			break;
			//View the number of archived tickets
		case 12:
			viewArchivedTicketsStat(data->TICKET_MAX_SIZE);
			break;
		default:
			printf("ERROR: Invalid selection!\n\n");
			break;
		}
	} while (selection);
}

//Display New Client Main menu
void menuClient(struct Ticket* tickets, const struct Account* account, int maxNumElement) {

	int selection, records;
	
	do 
	{
		printf("CUSTOMER: %s (%d)\n", account->login.displayName, account->accountNum);
		printf("==============================================\n");
		printf("Customer Main Menu\n");
		printf("==============================================\n");
		printf("1) View your account detail\n");
		printf("2) Create a new ticket\n");
		printf("3) Modify an active ticket\n");
		printf("4) List my tickets\n");
		printf("----------------------------------------------\n");
		printf("0) Logout\n\n");
		printf("Selection: ");

		selection = getIntFromRange(0, 4);
		putchar('\n');

		switch (selection)
		{
			//Display the number of saved tickets and logout
		case 0:
			records = writeTicketDataRecords(tickets, maxNumElement, "w");
			if (records) {
				printf("Saving session modifications...\n");
				printf("   %d tickets saved.\n", records);
			}
			printf("### LOGGED OUT ###\n\n");
			break;
			//Display the customer's account detail
		case 1:
			displayAccountDetailHeader();
			displayAccountDetailRecord(account);
			putchar('\n');
			pauseExecution();
			break;
			//Create a new ticket (modify the subject, add a message, close ticket)
		case 2:
			creatNewTicket(tickets, account->accountNum, account->login.displayName, maxNumElement);
			break;
			//Update the current logined customer's active tickets
		case 3:
			updateTicket(tickets, account->accountNum, account->type, account->login.displayName, maxNumElement);
			break;
			//Display the current logined customer's all the tickets
		case 4:
			displayMyTicketRecords(tickets, account->accountNum, maxNumElement);
			break;
		default:
			printf("ERROR: Invalid selection!\n\n");
			break;
		}
	} while (selection);
}

//Remove accounts and tickets and store archieved account&ticket records to files.
void removeAccount(struct AccountTicketingData* data, const struct Account* account) 
{
	int arrayIndexNum, confirm, i, ticketRecords, AccountRecords;
	ticketRecords = 0;

	arrayIndexNum = findAccountIndexByAcctNum(0, data->accounts, data->ACCOUNT_MAX_SIZE, 1);

	//Deny the removal if the account number entered is the same as the logged-in user's account
	if (account->accountNum == data->accounts[arrayIndexNum].accountNum)
	{
		putchar('\n');
		printf("ERROR: You can't remove your own account!\n\n");
	}
	else 
	{
		displayAccountDetailHeader();
		displayAccountDetailRecord(&data->accounts[arrayIndexNum]);

		//Obtain confirmation for the removal
		printf("\nAre you sure you want to remove this record? ([Y]es|[N]o): ");
		confirm = getConfirm(2);		
		if (confirm == 1) 
		{
			for (i = 0; i < data->TICKET_MAX_SIZE; i++) 
			{//Find the ticket for the account the user wants to delete
				if (data->tickets[i].ticketNum > 0 && data->tickets[i].accountNum == data->accounts[arrayIndexNum].accountNum)
				{//Archive the ticket if it's closed
					if (data->tickets[i].ticketStatus == 0) 
					{
						ticketRecords += writeArchivedTicDataRecords(&data->tickets[i], 1, "a");
					} //Empty ticket spot for a new ticket
					data->tickets[i].accountNum = 0;
					data->tickets[i].ticketNum = 0;
				}
			} //Archive the account
			AccountRecords = writeArchivedAccDateRecords(&data->accounts[arrayIndexNum], 1, "a");
			if (AccountRecords) 
			{ //Empty the account spot for new account
				data->accounts[arrayIndexNum].accountNum = 0;
			}
			putchar('\n');
			printf("*** Account Removed! ***\n\n");
		}
		else 
		{
			printf("*** No changes made! ***\n\n");
		}
	}
	pauseExecution();
}

//Display the number of saved account and ticket records 
void viewNumOfRecords(struct AccountTicketingData* data)
{
	int accountRecords, ticketRecords = 0;

	accountRecords = writeAccountDateRecords(data->accounts, data->ACCOUNT_MAX_SIZE, "w");
	ticketRecords = writeTicketDataRecords(data->tickets, data->TICKET_MAX_SIZE, "w");

	if (accountRecords && ticketRecords)
	{
		printf("Saving session modifications...\n");
		printf("   %d account saved.\n", accountRecords);
		printf("   %d tickets saved.\n", ticketRecords);
		printf("### LOGGED OUT ###\n\n");
	}
}




