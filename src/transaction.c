
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "cJSON.h"
#include "transaction.h"
#include "utils.h"

struct transaction *firstTransaction = NULL;

/*****************************************************
 * findTransactionByRRN: Looks up a rrn in the    *
 *            transactions tree. Returns a pointer   *
 *            to the node containing the rrn; if the *
 *            transaction number is not found,       *
 *            returns NULL.                          *
 *****************************************************/
struct transaction *findTransactionByRRN(long rrn)
{
  static struct transaction *t;

  for (t = firstTransaction;
       t != NULL && rrn != t->rrn;
       t = t->next)
    ;
  return t;
}

/**********************************************************
 * addTransaction: Prompts the user for information      *
 *         about a new transaction and then inserts the   *
 *         transaction into the database. Prints an error *
 *         message and returns prematurely if the         *
 *         transaction already exists or the database is  *
 *         full                                           *
 *********************************************************/
void addTransaction(void)
{
  struct transaction *new_transaction;
  cJSON *transaction = NULL;
  char *out = NULL;

  new_transaction = malloc(sizeof(struct transaction));
  if (new_transaction == NULL)
  {
    printf("\nDatabase is full; can't add more transactions.\n");
    return;
  }
  transaction = cJSON_CreateObject();

  new_transaction->next = firstTransaction;
  if (firstTransaction != NULL)
    firstTransaction->previous = new_transaction;
  firstTransaction = new_transaction;
  new_transaction->previous = NULL;

  // TODO improve rrn generator
  new_transaction->rrn = generateRRN();
  cJSON_AddNumberToObject(transaction, "RRN", new_transaction->rrn);

  new_transaction->date = time(NULL);
  struct tm tm = *localtime(&new_transaction->date);
  new_transaction->year = tm.tm_year + 1900;
  cJSON_AddNumberToObject(transaction, "year", new_transaction->year);
  new_transaction->month = tm.tm_mon + 1;
  cJSON_AddNumberToObject(transaction, "month", new_transaction->month);
  new_transaction->day = tm.tm_mday;
  cJSON_AddNumberToObject(transaction, "day", new_transaction->day);

  printTypes();

  printf("Select transaction type: ");

  scanf(" %d", (int *) (&new_transaction->type));
  cJSON_AddStringToObject(transaction, "type", matchType(new_transaction->type));

  while (getchar() != '\n')
    ;

  out = cJSON_Print(transaction);
  printf(out);

  free(out);
  cJSON_Delete(transaction);
}

/**********************************************************
 * freeTransaction: Frees memory allocated for            *   
 *                  transactions.                         *
 *********************************************************/
void freeTransaction(void) 
{
  while (firstTransaction != NULL)
  {
    struct transaction *currentTransaction = firstTransaction;
    firstTransaction = firstTransaction->next;
    free(currentTransaction);
  }
}

/**********************************************************
 * getTransaction: Prompts the user to enter an           *
 *         operation code on what transactions to get and *
 *         executes it.                                   *
 *********************************************************/
void getTransaction(void)
{
  printGetCommand();

  char command;

  for (;;)
  {
    printf("\nPress key 'm' to see command menu.");
    printf("\nEnter how to get transaction: ");

    command = getchar();

    while (getchar() != '\n') /* skips to end of line */
      ;

    switch (command)
    {
    case '1':
      getTransactionByRRN();
      break;

    case '2':
      getTransactionsByDate();
      break;

    case '3':
      getTransactionsByDateRange();
      break;

    case '4':
      getTransactionsByType();
      break;

    case 'm':
    case 'M':
      printGetCommand();
      break;

    case 'q':
    case 'Q':
      return;

    case 'x':
    case 'X':
      exit(EXIT_SUCCESS);

    default:
      printGetCommand();
      printf("\nInvalid command\n");
      break;
    }
  }
}

/**********************************************************
 * getTransactionByRRN: Prompts the user to enter the     *
 *         RRN, then looks up the transaction in the      *
 *         database. If the transaction exists, prints out*
 *         the transaction information; if not, prints an *
 *         error message.                                 *
 *********************************************************/
void getTransactionByRRN(void)
{
  long rrn;
  struct transaction *t;
  cJSON *transaction = NULL; 
  char *out = NULL;

  transaction = cJSON_CreateObject();
  if (transaction == NULL)
    cJSON_Delete(transaction);

  printf("Enter RRN: ");
  scanf(" %ld", &rrn);
  t = findTransactionByRRN(rrn);
  if (t != NULL)
  {
    cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
    cJSON_AddNumberToObject(transaction, "year", t->year);
    cJSON_AddNumberToObject(transaction, "month", t->month);
    cJSON_AddNumberToObject(transaction, "day", t->day);
    cJSON_AddStringToObject(transaction, "type", matchType(t->type));
  }
  else
    printf("\nTransaction not found.\n");

  while (getchar() != '\n')
    ;

  out = cJSON_Print(transaction);
  printf(out);

  free(out);
  cJSON_Delete(transaction);
}

/**********************************************************
 * getTransactionsByDate: Prompts the user to enter       *
 *         the date to look up, then looks up all         *
 *         transactions that match in the database. If    *
 *         the transaction exists, prints out the         *
 *         transaction information.                       *
 *********************************************************/
void getTransactionsByDate(void)
{
  struct transaction *t;
  int year;
  int month;
  int day;
  cJSON *transactions = NULL;
  cJSON *transaction = NULL;
  char *out = NULL;

  transactions = cJSON_CreateArray();
  if (transactions == NULL)
    cJSON_Delete(transactions);

  printf("\nEnter date(DD/MM/YYYY): ");
  scanf(" %d/ %d/ %d", &day, &month, &year);
  int count = 0;

  for (t = firstTransaction; t != NULL; t = t->next)
  {
    if (isSameDay(year, month, day, t))
    {
      cJSON_AddItemToArray(transactions, transaction = cJSON_CreateObject());
      cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
      cJSON_AddNumberToObject(transaction, "year", t->year);
      cJSON_AddNumberToObject(transaction, "month", t->month);
      cJSON_AddNumberToObject(transaction, "day", t->day);
      cJSON_AddStringToObject(transaction, "type", matchType(t->type));
      count++;
    }
  }

  while (getchar() != '\n')
    ;

  if (count)
  {
    out = cJSON_Print(transactions);
    printf(out);
  }
  else
    printf("\nThere are no transactions matching this query.\n");

  free(out);
  cJSON_Delete(transactions);
}

/**********************************************************
 * getTransactionsByDateRange: Prompts the user to        *
 *         enter the start and end dates to look up, then *
 *         looks up all transactions that match in the    *
 *         database. If the transaction exists, prints    *
 *         out the transaction information.               *
 *********************************************************/
void getTransactionsByDateRange(void)
{
  struct transaction *t;
  int startYear, endYear;
  int startMonth, endMonth;
  int startDay, endDay;
  cJSON *transactions = NULL;
  cJSON *transaction = NULL;
  char *out = NULL;

  transactions = cJSON_CreateArray();
  if (transactions == NULL)
    cJSON_Delete(transactions);

  printf("\nEnter start date(DD/MM/YYYY): ");
  scanf(" %d/ %d/ %d", &startDay, &startMonth, &startYear);
  printf("\nEnter end date(DD/MM/YYYY): ");
  scanf(" %d/ %d/ %d", &endDay, &endMonth, &endYear);
  int count = 0;

  for (t = firstTransaction; t != NULL; t = t->next)
  {
    if (isGreaterOrEqualDay(startYear, startMonth, startDay, t) && isLesserOrEqualDay(endYear, endMonth, endDay, t))
    {
      cJSON_AddItemToArray(transactions, transaction = cJSON_CreateObject());
      cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
      cJSON_AddNumberToObject(transaction, "year", t->year);
      cJSON_AddNumberToObject(transaction, "month", t->month);
      cJSON_AddNumberToObject(transaction, "day", t->day);
      cJSON_AddStringToObject(transaction, "type", matchType(t->type));
      count++;
    }
  }

  while (getchar() != '\n')
    ;

  if (count)
  {
    out = cJSON_Print(transactions);
    printf(out);
  }
  else
    printf("\nThere are no transactions matching this query.\n");

  free(out);
  cJSON_Delete(transactions);
}

/**********************************************************
 * getTransactionsByType: Prompts the user to enter       *
 *         the type to look up, then looks up all         *
 *         transactions that match in the database. If    *
 *         the transaction exists, prints out the         *
 *         transaction information.                       *
 *********************************************************/
void getTransactionsByType(void)
{
  struct transaction *t;
  enum transactionType type;
  cJSON *transactions = NULL;
  cJSON *transaction = NULL;
  char *out = NULL;

  transactions = cJSON_CreateArray();
  if (transactions == NULL)
    cJSON_Delete(transactions);

  printTypes();
  printf("\nEnter type: ");
  scanf(" %d", (int *)&type);
  int count = 0;

  for (t = firstTransaction; t != NULL; t = t->next)
  {
    if (type == t->type)
    {
      cJSON_AddItemToArray(transactions, transaction = cJSON_CreateObject());
      cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
      cJSON_AddNumberToObject(transaction, "year", t->year);
      cJSON_AddNumberToObject(transaction, "month", t->month);
      cJSON_AddNumberToObject(transaction, "day", t->day);
      cJSON_AddStringToObject(transaction, "type", matchType(t->type));
      count++;
    }
  }

  while (getchar() != '\n')
    ;

  if (count)
  {
    out = cJSON_Print(transactions);
    printf(out);
  }
  else
    printf("\nThere are no transactions matching this query.\n");

  free(out);
  cJSON_Delete(transactions);
}

/**********************************************************
 * updateTransaction: Prompts the user to enter a         *
 *         transaction RRN. Prints an error message if    *
 *         the transaction doesn't exist; otherwise,      *
 *         prompts the user to enter change in            *
 *         database.                                      *
 *********************************************************/
void updateTransaction(void)
{
  long rrn;
  struct transaction *t;
  cJSON *transaction = NULL;
  char *out = NULL;

  transaction = cJSON_CreateObject();

  printf("Enter RRN: ");
  scanf(" %ld", &rrn);
  t = findTransactionByRRN(rrn);
  if (t != NULL)
  {
    cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
    cJSON_AddNumberToObject(transaction, "year", t->year);
    cJSON_AddNumberToObject(transaction, "month", t->month);
    cJSON_AddNumberToObject(transaction, "day", t->day);

    printTypes();

    printf("\nChange transaction type: ");

    scanf(" %d", (int *) &t->type);
    cJSON_AddStringToObject(transaction, "type", matchType(t->type));
  }
  else
    printf("\nTransaction not found.\n");

  while (getchar() != '\n')
    ;

  out = cJSON_Print(transaction);
  printf(out);

  free(out);
  cJSON_Delete(transaction);
}

/**********************************************************
 * deleteTransaction: Prompts the user to enter a         *
 *         transaction RRN. Prints an error message if    *
 *         the transaction doesn't exist; otherwise,      *
 *         deletes the transaction in the database.       *
 *********************************************************/
void deleteTransaction(void)
{
  long rrn;
  struct transaction *t;

  printf("Enter RRN: ");
  scanf(" %ld", &rrn);
  while (getchar() != '\n')
    ;

  t = findTransactionByRRN(rrn);
  if (t != NULL)
  {
    char affirm;
    printf("\nAre you sure you want to delete this transaction?\n y/n\n");
    affirm = getchar();

    while (getchar() != '\n')
      ;

    switch (affirm)
    {
    case 'y':
    case 'Y':
      if (t->previous != NULL)
        t->previous->next = t->next;

      if (t->next != NULL)
        t->next->previous = t->previous;

      free(t);
      printf("\nTransaction deleted successfully.\n");
      break;

    default:
      break;
    }
  }
  else
    printf("\nTransaction not found.\n");
}

/**********************************************************
 * traverseTransaction: Prompts the user to enter a       *
 *         command to traverse the transactions tree.     *
 *********************************************************/
void traverseTransaction(void)
{
  struct transaction *t;
  cJSON *transaction = NULL;
  char *out = NULL;
  char move;

  transaction = cJSON_CreateObject();
  t = firstTransaction;

  if (t != NULL)
  {
    cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
    cJSON_AddNumberToObject(transaction, "year", t->year);
    cJSON_AddNumberToObject(transaction, "month", t->month);
    cJSON_AddNumberToObject(transaction, "day", t->day);
    cJSON_AddStringToObject(transaction, "type", matchType(t->type));

    out = cJSON_Print(transaction);
    printf(out);

    free(out);
    cJSON_Delete(transaction);
  }
  else
  {
    printf("\nThere are no transactions recorded\n");
    return;
  }

  for (;;)
  {
    printf("\n\nb - to move back, f - to move forward, m - to go back to menu\n");
    printf("Enter command: ");
    move = getchar();
    while (getchar() != '\n') /* skips to end of line */
      ;

    switch (move)
    {
    case 'b':
    case 'B':
      if (t->previous != NULL)
        t = t->previous;
      else
        printf("\nThis is the first element in the list\n\n");
      break;

    case 'f':
    case 'F':
      if (t->next != NULL)
        t = t->next;
      else
        printf("\nThis is the last element in the list\n\n");
      break;

    case 'm':
    case 'M':
      return;

    default:
      printf("\nInvalid input\n");
      break;
    }

    transaction = cJSON_CreateObject();
    if (t != NULL)
    {
      cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
      cJSON_AddNumberToObject(transaction, "year", t->year);
      cJSON_AddNumberToObject(transaction, "month", t->month);
      cJSON_AddNumberToObject(transaction, "day", t->day);
      cJSON_AddStringToObject(transaction, "type", matchType(t->type));

      out = cJSON_Print(transaction);
      printf("%s\n", out);
    }
    free(out);
    cJSON_Delete(transaction);
  }
}
