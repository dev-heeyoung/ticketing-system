#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "ticket.h"

#define TICKETDATAFILE "tickets.txt"
#define ARC_TICKETFILE "tickets_arc.txt"

#define TICKET_SIZE 17

//Search the array received in argument 2 for a match on a desired ticket number
int findTicketIndexByAcctNum(int matchTicketNumber, const struct Ticket* tickets, int maxNumElement, int prompt) {

	int i, enteredTicketNum;
	int found = 0;
	int findArrayIndex = -1;

	if (prompt == 0) {
		enteredTicketNum = matchTicketNumber;
	}
	else {
		printf("Enter the ticket number to view the messages or\n0 to return to previous menu: ");

		do {
			enteredTicketNum = getInteger();
			if (enteredTicketNum < 0) {
				printf("ERROR: Value must be positive or zero: ");
			} // Exit if the user enter 0 (return -2)
			else if (enteredTicketNum == 0) {
				findArrayIndex = -2;
				
			}
		} while (enteredTicketNum < 0);
	}

	found = 0;

	//Search the array index for a match on a desired ticket number
	if (findArrayIndex !=-2 ) {
		for (i = 0; found == 0 && i < maxNumElement; i++) {
			if (tickets[i].ticketNum == enteredTicketNum) {
				findArrayIndex = i;
				found = 1;
			}
		} 
		if (found == 0 && enteredTicketNum != 0) {
			printf("ERROR: Invalid ticket number\n");
		}	
	}
	
	return findArrayIndex;
}

//Find an empthy message record index
int findEmpthyMsgRecordIndex(const struct Ticket* ticket)
{
	int i, found, indexNum;

	found = 0;
	indexNum = -1;

	for (i = 0; found == 0 && i < MESSAGE_SIZE; i++) {
		if (ticket->messageRecords[i].accountType == 0) {
			found = 1;
			indexNum = i;
		}
	}
	return indexNum;
}

//Update the current logined customer's active tickets
void updateTicket(struct Ticket* tickets, const int accountNum, const char accountType, const char* displayName, int maxNumElement)
{
	int enteredTicketNum, selection;
	int findArrayIndex = -1;
	char* statusString;

	//Get input for ticket number and find the array index for a match on a desired ticket number
	printf("Enter ticket number: ");
	enteredTicketNum = getInteger();
	putchar('\n');
	findArrayIndex = findTicketIndexByAcctNum(enteredTicketNum, tickets, maxNumElement, 0);
	statusString = statusDisplayString(tickets[findArrayIndex].ticketStatus);

	// Display Update ticket menu if the account number matches
	if (tickets[findArrayIndex].accountNum == accountNum)
	{
		if (tickets[findArrayIndex].ticketStatus == 1)
		{
			do {
				printf("----------------------------------------\n");
				printf("Ticket %06d - Update Options\n", tickets[findArrayIndex].ticketNum);
				printf("----------------------------------------\n");
				printf("Status  : %s\n", statusString);
				printf("Subject : %s\n", tickets[findArrayIndex].subjectLine);
				printf("----------------------------------------\n");
				printf("1) Modify the subject\n");
				printf("2) Add a message\n");
				printf("3) Close ticket\n");
				printf("0) Done\n");
				printf("Selection: ");

				selection = getIntFromRange(0, 3);
				putchar('\n');

				switch (selection)
				{
				case 0:
					pauseExecution();
					break;
					//Modify the subject
				case 1:
					printf("Enter the revised ticket SUBJECT (%d chars. maximum): ", SUBJECTLINE_LEN);
					getCString(tickets[findArrayIndex].subjectLine, 1, SUBJECTLINE_LEN);
					putchar('\n');
					break;
					//Add a message
				case 2:
					addMessage(&tickets[findArrayIndex], accountType, displayName);
					break;
					//Close ticket
				case 3:
					closeTicket(&tickets[findArrayIndex], accountType, displayName);
					pauseExecution();
					selection = 0;
					break;
				default:
					break;
				}
			} while (selection);
		}
		else {
			printf("ERROR: Ticket is closed - changes are not permitted.\n\n");
			pauseExecution();
		}
	}
	else {
		printf("ERROR: Invalid ticket number - you may only modify your own ticket.\n\n");
		pauseExecution();
	}
}

//Add message
void addMessage(struct Ticket* ticket, const char accountType, const char* displayName) 
{
	int indexNum = -1;

	//Deny if the ticket is not active
	if (ticket->ticketStatus == 0) {
		printf("ERROR: Ticket is closed - new messages are not permitted.\n\n");
	}
	else {//Find the empty message records array index for a new message
		indexNum = findEmpthyMsgRecordIndex(ticket);

		if (indexNum == -1) {
			printf("ERROR: Message limit has been reached, call ITS Support!\n\n");
		}
		else {
			printf("Enter the ticket message details (%d chars. maximum). Press the ENTER key to submit:\n", MESSAGE_LEN);
			getCString(ticket->messageRecords[indexNum].messageDetails, 1, MESSAGE_LEN);
			putchar('\n');
			//Give the same account type and display as the logined accounts' info for the new message
			ticket->messageRecords[indexNum].accountType = accountType;
			strcpy(ticket->messageRecords[indexNum].displayName, displayName);
			ticket->numOfMessages += 1;
		}
	}
}

//Manage a ticket (add a message, close ticket, Re-open ticket)
void manageTicket(struct Ticket* tickets, const char accountType, const char* displayName, int maxNumElement)
{
	int getIndexNum, selection, enteredNum;
	char* statusString;

	printf("Enter ticket number: ");
	enteredNum = getInteger();
	getIndexNum = findTicketIndexByAcctNum(enteredNum, tickets, maxNumElement, 0);
	putchar('\n');

	do {
		statusString = statusDisplayString(tickets[getIndexNum].ticketStatus);

		printf("----------------------------------------\n");
		printf("Ticket %06d - Update Options\n", tickets[getIndexNum].ticketNum);
		printf("----------------------------------------\n");
		printf("Status  : %s\n", statusString);
		printf("Subject : %s\n", tickets[getIndexNum].subjectLine);
		printf("Acct#   : %d\n", tickets[getIndexNum].accountNum);
		printf("Customer: %s\n", tickets[getIndexNum].messageRecords->displayName);
		printf("----------------------------------------\n");
		printf("1) Add a message\n");
		printf("2) Close ticket\n");
		printf("3) Re-open ticket\n");
		printf("0) Done\n");
		printf("Selection: ");
	
		selection = getIntFromRange(0, 3);
		putchar('\n');

		switch (selection)
		{
		case 0:
			break;
			//Add a message
		case 1:
			addMessage(&tickets[getIndexNum], accountType,  displayName);
			break;
			//Close ticket
		case 2:
			closeTicket(&tickets[getIndexNum], accountType, displayName);
			break;
			//Re-open ticket
		case 3:
			reOpenTicket(&tickets[getIndexNum]);
			break;
		default:
			break;
		}
	} while (selection != 0);
}

//Close an active ticket
void closeTicket(struct Ticket* ticket, const char accountType, const char* displayName) 
{
	int indexNum, confirm;
	indexNum = -1;

	//Deny the close ticket if it's alreay closed  
	if (ticket->ticketStatus == 0) {
		printf("ERROR: Ticket is already closed!\n\n");
	}
	else {
		//confirm closing
		printf("Are you sure you CLOSE this ticket? ([Y]es|[N]o): ");
		confirm = getConfirm(2);
		putchar('\n');

		if (confirm == 1) 
		{//Find the empty message records array index for a new message
			indexNum = findEmpthyMsgRecordIndex(ticket);

			if (indexNum != -1)
			{
				printf("Do you want to leave a closing message? ([Y]es|[N]o): ");
				confirm = getConfirm(2);
				putchar('\n');
				
				if (confirm == 1) {
					addMessage(ticket, accountType, displayName);
				}
			}

			ticket->ticketStatus = 0;
			printf("*** Ticket closed! ***\n\n");
		}
	}
}

//Creat a new ticket (modify the subject, add a message, close ticket)
void creatNewTicket(struct Ticket* tickets, const int accountNum, const char* displayName, int maxNumElement) 
{
	int i, findIndexArray, highestTicketNum;
	highestTicketNum = 0;

	findIndexArray = findTicketIndexByAcctNum(0, tickets, maxNumElement, 0);

	if (findIndexArray == -1) 
	{
		printf("ERROR: Ticket listing is FULL, call ITS Support!\n\n");
	}
	else 
	{//Find the highest tickt number and set the number for a new ticket automatically
		for (i = 0; i < maxNumElement; i++) {
			if (tickets[i].ticketNum > highestTicketNum) {
				highestTicketNum = tickets[i].ticketNum;
			}
		}
		findIndexArray = i - 1;

		printf("New Ticket (Ticket#:%06d)\n", highestTicketNum + 1);
		printf("----------------------------------------\n");

		//New ticket setting
		tickets[findIndexArray].accountNum = accountNum;
		tickets[findIndexArray].ticketNum = highestTicketNum + 1;
		tickets[findIndexArray].ticketStatus = 1;
		tickets[findIndexArray].numOfMessages = 1;
		tickets[findIndexArray].messageRecords->accountType = 'C';
		strcpy(tickets[findIndexArray].messageRecords->displayName, displayName);

		printf("Enter the ticket SUBJECT (%d chars. maximum): ", SUBJECTLINE_LEN);
		getCString(tickets[findIndexArray].subjectLine, 1, SUBJECTLINE_LEN);
		putchar('\n');

		printf("Enter the ticket message details (%d chars. maximum). Press the ENTER key to submit:\n", MESSAGE_LEN);
		getCString(tickets[findIndexArray].messageRecords->messageDetails, 1, MESSAGE_LEN);
		putchar('\n');

		printf("*** New ticket created! ***\n\n");
	}
	pauseExecution();
}

//Active a closed ticket
void reOpenTicket(struct Ticket* ticket) 
{
	int confirm;

	if (ticket->ticketStatus == 1) 
	{
		printf("ERROR: Ticket is already active!\n\n");
	}
	else
	{
		printf("Are you sure you RE-OPEN this closed ticket? ([Y]es|[N]o): ");
		confirm = getConfirm(2);
		putchar('\n');

		if (confirm == 1) {
			ticket->ticketStatus = 1;
			printf("*** Ticket re-opened! ***\n\n");
		}
	}
}

//Display the user's own tickets
void displayMyTicketRecords(const struct Ticket* tickets, int accountNum, int maxNumElement)
{
	int i, findIndexArray;
	char* statusString;

	do
	{
		printf("------ ------ ------------------------------ --------\n");
		printf("Ticket Status Subject                        Messages\n");
		printf("------ ------ ------------------------------ --------\n");

		for (i = 0; i < maxNumElement; i++) {
			if (accountNum == tickets[i].accountNum) {

				statusString = statusDisplayString(tickets[i].ticketStatus);

				printf("%06d %-6s %-30s %5d\n", tickets[i].ticketNum, statusString, tickets[i].subjectLine, tickets[i].numOfMessages);
			}
		}
		printf("------ ------ ------------------------------ --------\n\n");

		findIndexArray = findTicketIndexByAcctNum(0, tickets, maxNumElement, 1);
		putchar('\n');

		if (findIndexArray >= 0)
		{
			if (tickets[findIndexArray].accountNum == accountNum)
			{
				displayDetailMessageRecords(&tickets[findIndexArray]);
			}
			else
			{
				printf("ERROR: Invalid ticket number - you may only access your own tickets.\n\n");
				pauseExecution();
			}
		}

	} while (findIndexArray != -2);
}

//Display ticket header
void displayTicketHeader() {
	printf("------ ----- --------------- ------ ------------------------------ --------\n");
	printf("Ticket Acct# Display Name    Status Subject                        Messages\n");
	printf("------ ----- --------------- ------ ------------------------------ --------\n");
}

//Display the specific ticket summary
void displayTicketSummary(const struct Ticket* ticket) {
	char* statusString;

	statusString = statusDisplayString(ticket->ticketStatus);

	printf("%06d %5d %-15s %-6s %-30s %5d\n", ticket->ticketNum, ticket->accountNum, ticket->messageRecords->displayName, statusString, ticket->subjectLine, ticket->numOfMessages);
}

//Display new tickets
void displayNewTicketRecords(const struct Ticket* tickets, int maxNumElement)
{
	int i, arrayIndexNum;
	arrayIndexNum = -1;

	do {
		displayTicketHeader();
		for (i = 0; i < maxNumElement; i++) {
			if (tickets[i].ticketStatus == 1 && tickets[i].numOfMessages == 1) {
				displayTicketSummary(&tickets[i]);
			}
		}
		printf("------ ----- --------------- ------ ------------------------------ --------\n\n");

		arrayIndexNum = findTicketIndexByAcctNum(0, tickets, maxNumElement, 1);
		putchar('\n');
		if (arrayIndexNum >= 0) {
			displayDetailMessageRecords(&tickets[arrayIndexNum]);
		}
	} while (arrayIndexNum != -2);
}

//Display active tickets
void displayActiveTicketRecords(const struct Ticket* tickets, int maxNumElement) 
{
	int i, arrayIndexNum;
	arrayIndexNum = -1;

	do {
		displayTicketHeader();
		for (i = 0; i < maxNumElement; i++) {
			if (tickets[i].ticketStatus == 1 && tickets[i].ticketNum > 0) {
				displayTicketSummary(&tickets[i]);
			}
		}
		printf("------ ----- --------------- ------ ------------------------------ --------\n\n");

		arrayIndexNum = findTicketIndexByAcctNum(0, tickets, maxNumElement, 1);
		putchar('\n');
		if (arrayIndexNum >= 0) {
			displayDetailMessageRecords(&tickets[arrayIndexNum]);
		}
	} while (arrayIndexNum != -2);
}

//Display closed tickets
void displayClosedTicketRecords(const struct Ticket* tickets, int maxNumElement)
{
	int i, arrayIndexNum;
	arrayIndexNum = -1;

	do {
		displayTicketHeader();
		for (i = 0; i < maxNumElement; i++) {
			if (tickets[i].ticketStatus == 0 && tickets[i].ticketNum > 0) {
				displayTicketSummary(&tickets[i]);
			}
		}
		printf("------ ----- --------------- ------ ------------------------------ --------\n\n");

		arrayIndexNum = findTicketIndexByAcctNum(0, tickets, maxNumElement, 1);
		putchar('\n');
		if (arrayIndexNum >= 0) {
			displayDetailMessageRecords(&tickets[arrayIndexNum]);
		}
	} while (arrayIndexNum != -2);

}

//Display the entered ticket number's detail message records
void displayDetailMessageRecords(const struct Ticket* ticket) {

	int i, messageCount;

	char* statusString;
	char* typeString;

	//Display detail message records that are matched with entered ticket number
	statusString = statusDisplayString(ticket->ticketStatus);

	printf("================================================================================\n");
	printf("%06d (%s) Re: %s\n", ticket->ticketNum, statusString, ticket->subjectLine);
	printf("================================================================================\n");

	messageCount = 0;

	for (i = 0; i < MESSAGE_SIZE; i++) {
		//Display message records if it's not empty
		if (ticket->messageRecords[i].accountType != 0) {
			typeString = typeDisplayString(ticket->messageRecords[i].accountType);

			printf("%s (%s):\n", typeString, ticket->messageRecords[i].displayName);
			printf("   %s\n\n", ticket->messageRecords[i].messageDetails);
			messageCount += 1;

			//Show 5 messages and display pause section 
			if (messageCount % 5 == 0) {
				pauseExecution();
			}
		}
	}
	if (messageCount % 5 != 0) {
		pauseExecution();
	}
}

//archive close tickets
void archiveClosedTickets(struct Ticket* tickets, int maxNumElement) {
	int confirm, ticketRecords, i;
	ticketRecords = 0;

	printf("Are you sure? This action cannot be reversed. ([Y]es|[N]o): ");
	confirm = getConfirm(2);
	putchar('\n');

	if (confirm == 1)
	{
		for (i = 0; i < maxNumElement; i++)
		{//Store tickets that are not active in "ticket_arc.txt" file
			if (tickets[i].ticketNum > 0 && tickets[i].ticketStatus == 0)
			{
				ticketRecords += writeArchivedTicDataRecords(&tickets[i], 1, "a");
				tickets[i].ticketNum = 0;
			}
		}
	}
	printf("*** %d tickets archived ***\n\n", ticketRecords);
	pauseExecution();
}

//Display the number of archived tickets and messages
void viewArchivedTicketsStat(int maxNumElement)
{
	int ticketRecords, messageCount, i, j;
	ticketRecords = messageCount = 0;

	struct Ticket removedTickets[TICKET_SIZE] = { {0} };

	ticketRecords = loadArchivedTickets(removedTickets, maxNumElement);
	for (i = 0; i < maxNumElement; i++) {
		for (j = 0; j < MESSAGE_SIZE; j++) {
			if (removedTickets[i].ticketNum > 0 && removedTickets[i].messageRecords[j].accountType > 0) {
				messageCount += 1;
			}
		}
	}
	printf("There are %d ticket(s) and a total of %d message(s) archived.\n\n", ticketRecords, messageCount);
	pauseExecution();
}

//Load archived tickets records
int loadArchivedTickets(struct Ticket archivedTickets[], int maxNumElement)
{
	int recCount, fieldCount;
	recCount = fieldCount = 0;

	FILE* fp = fopen(ARC_TICKETFILE, "r");

	if (fp != NULL)
	{
		// iterate the file until no more records can be read
		// read a record:
		recCount = readTickets(fp, archivedTickets, maxNumElement);
		fflush(fp); // forces outstanding char's in the input buffer(file) to be read.
		fclose(fp);
		fp = NULL;
	}
	return recCount;
}

//Load tickets records
int loadTickets(struct Ticket tickets[], int maxNumElement)
{
	int recCount, fieldCount;
	recCount = fieldCount = 0;

	FILE* fp = fopen(TICKETDATAFILE, "r");

	if (fp != NULL)
	{
		// iterate the file until no more records can be read...
		// read a record:
		recCount = readTickets(fp, tickets, maxNumElement);
		fflush(fp); // forces outstanding char's in the input buffer(file) to be read.
		fclose(fp);
		fp = NULL;
	}
	else
	{
		puts("ERROR: UNABLE TO ACCESS FILE!!!\n");
	}
	return recCount;
}

//Read ticket records
int readTickets(FILE* fp, struct Ticket tickets[], int maxNumElement)
{
	int recCount, fieldCount, i;
	recCount = fieldCount = 0;

	if (fp != NULL)
	{
		// iterate the file until no more records can be read...
		// read a record:
		do
		{
			fieldCount = fscanf(fp, "%d|%d|%d|%30[^|]|%d|",
				&tickets[recCount].ticketNum,
				&tickets[recCount].accountNum,
				&tickets[recCount].ticketStatus,
				tickets[recCount].subjectLine,
				&tickets[recCount].numOfMessages);

			if (fieldCount == 5) {
				if (tickets[recCount].numOfMessages > 0) {
					for (i = 0, fieldCount = 3; fieldCount == 3 && i < tickets[recCount].numOfMessages; i++) {
						fieldCount = fscanf(fp, "%c|%30[^|]|%150[^|]|", &tickets[recCount].messageRecords[i].accountType,
							tickets[recCount].messageRecords[i].displayName,
							tickets[recCount].messageRecords[i].messageDetails);
					}
				}
				while (fgetc(fp) != '\n');

				recCount++;
			}
		} while (!feof(fp) && recCount < maxNumElement);
	}
	else {
		puts("ERROR: UNABLE TO ACCESS FILE!!!\n");
	}

	return recCount;
}

//Write removed ticket records in "ticket_arc.txt"
int writeArchivedTicDataRecords(struct Ticket tickets[], int maxNumElement, const char* mode)
{
	int i, recordCount = 0;

	FILE* fp = fopen(ARC_TICKETFILE, mode);

	if (fp != NULL)
	{
		for (i = 0; i < maxNumElement; i++)
		{
			recordCount += appendTicketDataRecord(fp, &tickets[i]);
		}
		fflush(fp); 
		fclose(fp);
		fp = NULL;
	}
	else {
		puts("ERROR: UNABLE TO ACCESS FILE!!!\n");
	}

	return recordCount;
}

//Write ticket records in "ticket.txt"
int writeTicketDataRecords(struct Ticket tickets[], int maxNumElement, const char* mode)
{
	int i, recordCount = 0;

	FILE* fp = fopen(TICKETDATAFILE, mode);

	if (fp != NULL)
	{
		for (i = 0; i < maxNumElement; i++)
		{
			recordCount += appendTicketDataRecord(fp, &tickets[i]);
		}
		fflush(fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		puts("ERROR: UNABLE TO ACCESS FILE!!!\n");
	}

	return recordCount;
}

//Append ticket records
int appendTicketDataRecord(FILE* fp, struct Ticket* ticket)
{
	int result = 0, i;

	// have a connection to a file! 
	if (fp != NULL)
	{
		// Field Delimeter: '|' (Pipe symbol)
		if (ticket->ticketNum > 0) {
			result = fprintf(fp, "%d|%d|%d|%s|%d|",
				ticket->ticketNum,
				ticket->accountNum,
				ticket->ticketStatus,
				ticket->subjectLine,
				ticket->numOfMessages);
			if (result > 0) {
				for (i = 0; result && i < ticket->numOfMessages; i++) {
					result = fprintf(fp, "%c|%s|%s|",
						ticket->messageRecords[i].accountType,
						ticket->messageRecords[i].displayName,
						ticket->messageRecords[i].messageDetails);
				}
			}
			result = fputc('\n', fp);
		}
	}
	else {
		puts("ERROR: UNABLE TO ACCESS FILE!!!\n");
	}

	return result > 0 ? 1 : 0;
}


