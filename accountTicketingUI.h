#ifndef ACCOUNT_TICKETING_UI_H_
#define ACCOUNT_TICKETING_UI_H_

#include "account.h"
#include "ticket.h"
#include "commonHelpers.h"

struct AccountTicketingData {
	struct Account* accounts; 
	const int ACCOUNT_MAX_SIZE; 
	struct Ticket* tickets; 
	const int TICKET_MAX_SIZE; 
};

void applicationStartup(struct AccountTicketingData* data);
int menuLogin(const struct Account* accounts, int maxNumElement);
void menuAgent(struct AccountTicketingData* data, const struct  Account* accounts);
void menuClient(struct Ticket* tiekcts, const struct Account* accounts, int maxNumElement);
void removeAccount(struct AccountTicketingData* data, const struct Account* account);
void viewNumOfRecords(struct AccountTicketingData* data);


#endif 