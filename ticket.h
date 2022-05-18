#ifndef TICKET_H_
#define TICKET_H_

#include "commonHelpers.h"

#define MESSAGE_LEN 150
#define SUBJECTLINE_LEN 30
#define MESSAGE_SIZE 20
#define DISPLAYNAME_LEN 30

struct Message {
	char accountType;
	char displayName[DISPLAYNAME_LEN + 1];
	char messageDetails[MESSAGE_LEN + 1];
};

struct Ticket {
	int ticketNum;
	int accountNum;
	int ticketStatus;
	char subjectLine[SUBJECTLINE_LEN + 1];
	int numOfMessages;
	struct Message messageRecords[MESSAGE_SIZE];
};

int findTicketIndexByAcctNum(int matchTicketNumber, const struct Ticket* tickets, int maxNumElement, int prompt);
int findEmpthyMsgRecordIndex(const struct Ticket* ticket);
void updateTicket(struct Ticket* tickets, const int accountNum, const char accountType, const char* displayName, int maxNumElement);
void addMessage(struct Ticket* ticket, const char accountType, const char* displayName);
void manageTicket(struct Ticket* tickets, const char accountType, const char* displayName, int maxNumElement);
void closeTicket(struct Ticket* ticket, const char accountType, const char* displayName);
void creatNewTicket(struct Ticket* tickets, const int accountNum, const char* displayName, int maxNumElement);
void reOpenTicket(struct Ticket* ticket);

void displayTicketHeader(void);
void displayTicketSummary(const struct Ticket* ticket);
void displayNewTicketRecords(const struct Ticket* tickets, int maxNumElement);
void displayActiveTicketRecords(const struct Ticket* tickets, int maxNumElement);
void displayClosedTicketRecords(const struct Ticket* tickets, int maxNumElement);
void displayMyTicketRecords(const struct Ticket* tickets, const int accountNum, int maxNumElement);
void displayDetailMessageRecords(const struct Ticket* ticket);
void archiveClosedTickets(struct Ticket* tickets, int maxNumElement);
void viewArchivedTicketsStat(int maxNumElement);

int loadArchivedTickets(struct Ticket archivedTickets[], int maxNumElement);
int loadTickets(struct Ticket tickets[], int maxNumElement);
int readTickets(FILE* fp, struct Ticket tickets[], int maxNumElement);
int writeArchivedTicDataRecords(struct Ticket tickets[], int maxNumElement, const char* mode);
int writeTicketDataRecords(struct Ticket tickets[], int maxNumElement, const char* mode);
int appendTicketDataRecord(FILE* fp, struct Ticket* ticket);







#endif


