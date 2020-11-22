
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#define RRN_LENGTH 15
#define DATE_LENGTH 15

struct transaction
{
    char rrn[RRN_LENGTH];
    enum type {CREDIT, DEBIT, REVERSAL} type;
    char date[DATE_LENGTH];
    struct transaction *previous;
    struct transaction *next;
};

struct transaction *first_transaction = NULL;

void print_command(void);
void print_get_command(void);
void add_transaction(void);

int CJSON_CDECL main(void)
{
  printf("Commands\n");
  print_command();

  char command;
  
  printf("\nEnter a command: ");

  for (;;) {
    command = getchar();
    
    switch (command)
    {
    case 'a': case 'A':
      printf("add\n");
      break;
    case 'g': case 'G':
      print_get_command();
      break;
    case 'd': case 'D': 
      printf("delete\n");
      break;
    case 'u': case 'U': 
      printf("edit\n");
      break;
    case '\n':
      break;
    case 'x': case 'X':
      return 0;
    default:
      print_command();
      printf("\nEnter a valid command: ");
      break;
  //     transaction = cJSON_CreateObject();
  //     if (transaction == NULL)
  //       return 1;
  //     cJSON_AddItemToArray(transactions, transaction);
      
  //     char rrn_provided[RRN_LENGTH];
  //     scanf("%s", rrn_provided);

  //     rrn = cJSON_CreateString(rrn_provided);
  //     if (rrn == NULL)
  //       return 1;
  //     cJSON_AddItemToObject(transaction, "RRN", rrn);

  //     printf("Enter type\n")
  //     printf("0 - debit, 1 - credit, 2 - reversal\n")

  //     scanf("%d", &type_provided)

  //     type = cJSON_CreateString(match_type(type_provided));
  //     if (type == NULL)
  //       return 1;
  //     cJSON_AddItemToObject(transaction, "type", type);

  //     char date_provided[DATE_LENGTH];
  //     scanf("%s", date_provided);

  //     date = cJSON_CreateString(date_provided);
  //     if (date == NULL)
  //       return 1;
  //     cJSON_AddItemToObject(transaction, "date", date);

  //     break;
    }
  }

  return 0;
}

void print_command(void)
{
  printf(
    "\n a - add transaction,"
    "\n g - get transaction,"
    "\n d - delete transaction by RRN,"
    "\n u - update transaction by RRN"
    "\n x - terminate program\n"
  );
}

void print_get_command(void)
{
  printf(
    "\n 1 - get transaction by RRN,"
    "\n 2 - get transaction by date,"
    "\n 3 - get transaction by date range,"
    "\n 4 - get transaction by type,"
    "\n x - terminate program\n"
  );
}

void add_transaction(void)
{
  struct transaction *current, *new_transaction;

  new_transaction = malloc(sizeof(struct transaction));
  if (new_transaction == NULL) {
    printf("Database is full; can't add more transactions.\n");
    return;
  }
// TODO
  // new_transaction->rrn = generate_rrn();

  for (current = first_transaction, new_transaction->previous = NULL;
       current != NULL; new_transaction->previous = current, current = current->next)
       ;

  printf("0 - credit, 1 - debit, 2 - reversal\n\n");
  printf("Select transaction type: ");
  scanf("%d", &new_transaction->type);

// TODO
  // new_transaction->date = time.now();

  new_transaction->next = current;
  if (new_transaction->previous == NULL)
    first_transaction = new_transaction;
  else
    new_transaction->previous->next = new_transaction;  
}

// char *match_type(int type) 
// {
//   switch (type) {
//   case 0: return "debit";
//   case 1: return "credit";
//   case 2: return "reversal";
//   default: return "invalid";
//   }
// }


