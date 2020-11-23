
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "cJSON.h"

enum type
{
  PURCHASE,
  WITHDRAWAL,
  DEPOSIT,
  REFUND,
  REVERSAL,
  BALANCE_INQUIRY,
  PAYMENTS,
  INTER_ACCOUNT_TRANSFER,
};

struct transaction
{
  long rrn;
  enum type type;
  time_t date;
  int year;
  int month;
  int day;
  struct transaction *previous;
  struct transaction *next;
};

struct transaction *first_transaction = NULL;

/* prototypes */
void print_command(void);
void print_get_command(void);
void print_types(void);
bool is_same_day(int year, int month, int day, struct transaction *t);
bool is_lesser_or_equal_day(int year, int month, int day, struct transaction *t);
bool is_greater_or_equal_day(int year, int month, int day, struct transaction *t);
struct transaction *find_transaction_by_rrn(long rrn);
void add_transaction(void);
void get_transaction(void);
void get_transaction_by_rrn(void);
void get_transactions_by_date(void);
void get_transactions_by_date_range(void);
void get_transactions_by_type(void);
void update_transaction(void);
void delete_transaction(void);
void traverse(void);
char *match_type(enum type type);
long generate_rrn(void);

/**********************************************************
 * main: Prompts the user to enter a command then         *
 *         executes it while in a continious loop         *
 *         Loop terminates when 'x' or 'X' key is pressed *
 *********************************************************/
int CJSON_CDECL main(void)
{
  printf("Commands\n");
  print_command();

  char command;

  for (;;)
  {
    printf("\nPress key 'm' to see command menu.");
    printf("\nEnter a command: ");

    command = getchar();

    while (getchar() != '\n') /* skips to end of line */
      ;

    switch (command)
    {
    case 'a':
    case 'A':
      add_transaction();
      break;
    case 'g':
    case 'G':
      get_transaction();
      break;
    case 't':
    case 'T':
      traverse();
      break;
    case 'd':
    case 'D':
      delete_transaction();
      break;
    case 'u':
    case 'U':
      update_transaction();
      break;
    case 'm':
    case 'M':
      print_command();
      break;
    case 'x':
    case 'X':
      return 0;
    default:
      print_command();
      printf("\nInvalid command\n");
      break;
    }
  }

  return 0;
}

/**********************************************************
 * print_command: Prints the available commands to console*
 *********************************************************/
void print_command(void)
{
  printf(
      "\n a - add transaction,"
      "\n g - get transaction,"
      "\n d - delete transaction by RRN,"
      "\n u - update transaction by RRN"
      "\n x - terminate program\n");
}

/**********************************************************
 * print_get_command: Prints the available commands for   *
 *           getting transactions to console              *
 *********************************************************/
void print_get_command(void)
{
  printf(
      "\n 1 - get transaction by RRN,"
      "\n 2 - get transaction by date,"
      "\n 3 - get transaction by date range,"
      "\n 4 - get transaction by type,"
      "\n x - terminate program\n");
}

/**********************************************************
 * print_get_command: Prints the available transaction    *
 *           types to console                             *
 *********************************************************/
void print_types(void)
{  
  printf("0 - purchase,\n"
         "1 - withdrawal,\n"
         "2 - deposit,\n"
         "3 - refund,\n"
         "4 - reversal,\n"
         "5 - balance inquiry,\n"
         "6 - payments,\n"
         "7 - inter-account transfer\n\n");

}

/**********************************************************
 * is_same_day: Compares two dates. Returns true if dates *
 *           are equal, false otherwise                   *
 *********************************************************/
bool is_same_day(int year, int month, int day, struct transaction *t)
{
  return year == t->year && month == t->month && day == t->day;
}

/**********************************************************
 * is_greater_or_equal_day: Compares two dates. Returns   *
 *           true if date in transaction is greater or    *
 *           equal to test date, false otherwise          *
 *********************************************************/
bool is_greater_or_equal_day(int year, int month, int day, struct transaction *t)
{
  return t->year >= year && t->month >= month && t->day >= day;
}

/**********************************************************
 * is_lesser_or_equal_day: Compares two dates. Returns    *
 *           true if date in transaction is lesser or     *
 *           equal to test date, false otherwise          *
 *********************************************************/
bool is_lesser_or_equal_day(int year, int month, int day, struct transaction *t)
{
  return t->year <= year && t->month <= month && t->day <= day;
}

/*****************************************************
 * find_transaction_by_rrn: Looks up a rrn in the    *
 *            transactions tree. Returns a pointer   *
 *            to the node containing the rrn; if the *
 *            transaction number is not found,       *
 *            returns NULL.                          *
 *****************************************************/
struct transaction *find_transaction_by_rrn(long rrn)
{
  static struct transaction *t;

  for (t = first_transaction;
       t != NULL && rrn != t->rrn;
       t = t->next)
    ;
  if (t != NULL && rrn == t->rrn)
    return t;
  return NULL;
}

/**********************************************************
 * add_transaction: Prompts the user for information      *
 *         about a new transaction and then inserts the   *
 *         transaction into the database. Prints an error *
 *         message and returns prematurely if the         *
 *         transaction already exists or the database is  *
 *         full                                           *
 *********************************************************/
void add_transaction(void)
{
  struct transaction *new_transaction;
  cJSON *transaction = NULL;
  char *out = NULL;

  new_transaction = malloc(sizeof(struct transaction));
  if (new_transaction == NULL)
  {
    printf("Database is full; can't add more transactions.\n");
    return;
  }
  transaction = cJSON_CreateObject();

  new_transaction->next = first_transaction;
  if (first_transaction != NULL)
    first_transaction->previous = new_transaction;
  first_transaction = new_transaction;
  new_transaction->previous = NULL;

  // TODO improve rrn generator
  new_transaction->rrn = generate_rrn();
  cJSON_AddNumberToObject(transaction, "RRN", new_transaction->rrn);

  new_transaction->date = time(NULL);
  cJSON_AddNumberToObject(transaction, "date", new_transaction->date);

  print_types();

  printf("Select transaction type: ");

  scanf(" %d", &new_transaction->type);
  cJSON_AddStringToObject(transaction, "type", match_type(new_transaction->type));

  while (getchar() != '\n')
    ;

  out = cJSON_Print(transaction);
  printf(out);

  free(out);
  cJSON_Delete(transaction);
}

/**********************************************************
 * get_transaction: Prompts the user to enter an          *
 *         operation code on what transactions to get and *
 *         executes it.                                   *
 *********************************************************/
void get_transaction(void)
{
  print_get_command();

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
      get_transaction_by_rrn();
      break;

    case '2':
      get_transactions_by_date();
      break;

    case '3':
      get_transactions_by_date_range();
      break;

    case '4':
      get_transactions_by_type();
      break;

    case 'm':
      print_get_command();
      break;

    case 'q':
    case 'Q':
      return;

    default:
      print_get_command();
      printf("\nInvalid command\n");
      break;
    }
  }
}

/**********************************************************
 * get_transaction_by_rrn: Prompts the user to enter the  *
 *         RRN, then looks up the transaction in the      *
 *         database. If the transaction exists, prints out*
 *         the transaction information; if not, prints an *
 *         error message.                                 *
 *********************************************************/
void get_transaction_by_rrn(void)
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
  t = find_transaction_by_rrn(rrn);
  if (t != NULL)
  {
    cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
    cJSON_AddNumberToObject(transaction, "date", t->date);
    cJSON_AddStringToObject(transaction, "type", match_type(t->type));
  }
  else
    printf("Transaction not found.\n");

  while (getchar() != '\n')
    ;

  out = cJSON_Print(transaction);
  printf(out);

  free(out);
  cJSON_Delete(transaction);
}

/**********************************************************
 * get_transactions_by_date: Prompts the user to enter    *
 *         the date to look up, then looks up all         *
 *         transactions that match in the database. If    *
 *         the transaction exists, prints out the         *
 *         transaction information.                       *
 *********************************************************/
void get_transactions_by_date(void)
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

  print_types();
  printf("\nEnter date(DD/MM/YYYY): ");
  scanf(" %d/ %d/ %d", &day, &month, &year);

  for (t = first_transaction; t != NULL; t = t->next)
  {
    if (is_same_day(year, month, day, t))
    {
      cJSON_AddItemToArray(transactions, transaction = cJSON_CreateObject());
      cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
      cJSON_AddNumberToObject(transaction, "date", t->date);
      cJSON_AddStringToObject(transaction, "type", match_type(t->type));
    }
  }
  
  while (getchar() != '\n')
    ;

  out = cJSON_Print(transactions);
  printf(out);

  free(out);
  cJSON_Delete(transactions);
}

/**********************************************************
 * get_transactions_by_date_range: Prompts the user to    *
 *         enter the start and end dates to look up, then *
 *         looks up all transactions that match in the    *
 *         database. If the transaction exists, prints    *
 *         out the transaction information.               *
 *********************************************************/
void get_transactions_by_date_range(void)
{
  struct transaction *t;
  int year1, year2;
  int month1, month2;
  int day1, day2;
  cJSON *transactions = NULL;
  cJSON *transaction = NULL;
  char *out = NULL;

  transactions = cJSON_CreateArray();
  if (transactions == NULL)
    cJSON_Delete(transactions);

  print_types();
  printf("\nEnter start date(DD/MM/YYYY): ");
  scanf(" %d/ %d/ %d", &day1, &month1, &year1);
  printf("\nEnter end date(DD/MM/YYYY): ");
  scanf(" %d/ %d/ %d", &day2, &month2, &year2);

  for (t = first_transaction; t != NULL; t = t->next)
  {
    if (is_greater_or_equal_day(year1, month1, day1, t) && is_lesser_or_equal_day(year2, month2, day2, t))
    {
      cJSON_AddItemToArray(transactions, transaction = cJSON_CreateObject());
      cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
      cJSON_AddNumberToObject(transaction, "date", t->date);
      cJSON_AddStringToObject(transaction, "type", match_type(t->type));
    }
  }
  
  while (getchar() != '\n')
    ;

  out = cJSON_Print(transactions);
  printf(out);

  free(out);
  cJSON_Delete(transactions);
}

/**********************************************************
 * get_transactions_by_type: Prompts the user to enter    *
 *         the type to look up, then looks up all         *
 *         transactions that match in the database. If    *
 *         the transaction exists, prints out the         *
 *         transaction information.                       *
 *********************************************************/
void get_transactions_by_type(void)
{
  struct transaction *t;
  enum type type;
  cJSON *transactions = NULL;
  cJSON *transaction = NULL;
  char *out = NULL;

  transactions = cJSON_CreateArray();
  if (transactions == NULL)
    cJSON_Delete(transactions);

  print_types();
  printf("\nEnter type: ");
  scanf(" %d", &type);

  for (t = first_transaction; t != NULL; t = t->next)
  {
    if (type == t->type)
    {
      cJSON_AddItemToArray(transactions, transaction = cJSON_CreateObject());
      cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
      cJSON_AddNumberToObject(transaction, "date", t->date);
      cJSON_AddStringToObject(transaction, "type", match_type(t->type));
    }
  }
  
  while (getchar() != '\n')
    ;

  out = cJSON_Print(transactions);
  printf(out);

  free(out);
  cJSON_Delete(transactions);
}

/**********************************************************
 * update_transaction: Prompts the user to enter a        *
 *         transaction RRN. Prints an error message if    *
 *         the transaction doesn't exist; otherwise,      *
 *         prompts the user to enter change in            *
 *         database.                                      *
 *********************************************************/
void update_transaction(void)
{
  long rrn;
  struct transaction *t;
  cJSON *transaction = NULL;
  char *out = NULL;

  transaction = cJSON_CreateObject();

  printf("Enter RRN: ");
  scanf(" %ld", &rrn);
  t = find_transaction_by_rrn(rrn);
  if (t != NULL)
  {
    cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
    cJSON_AddNumberToObject(transaction, "date", t->date);

    print_types();

    printf("Change transaction type: ");

    scanf(" %d", &t->type);
    cJSON_AddStringToObject(transaction, "type", match_type(t->type));
  }
  else
    printf("Transaction not found.\n");

  while (getchar() != '\n')
    ;

  out = cJSON_Print(transaction);
  printf(out);

  free(out);
  cJSON_Delete(transaction);
}

/**********************************************************
 * delete_transaction: Prompts the user to enter a        *
 *         transaction RRN. Prints an error message if    *
 *         the transaction doesn't exist; otherwise,      *
 *         deletes the transaction in the database.       *
 *********************************************************/
void delete_transaction(void)
{
  long rrn;
  struct transaction *t;
  cJSON *transaction = NULL;
  char *out = NULL;

  transaction = cJSON_CreateObject();

  printf("Enter RRN: ");
  scanf(" %ld", &rrn);
  while (getchar() != '\n')
    ;

  t = find_transaction_by_rrn(rrn);
  if (t != NULL)
  {
    char affirm;
    printf("Are you sure you want to delete this transaction?\n y/n\n");
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

      printf("Transaction deleted successfully.");
      free(t);
      break;

    default:
      break;
    }
  }
  else
    printf("Transaction not found.\n");
}

/**********************************************************
 * traverse: Prompts the user to enter a command to       *
 *         traverse the transactions tree.                *
 *********************************************************/
void traverse(void)
{
  struct transaction *t;
  cJSON *transaction = NULL;
  char *out = NULL;
  char move;

  t = first_transaction;

  printf("b - to move back, f - to move forward, m - to go back to menu\n");

  transaction = cJSON_CreateObject();
  for (;;)
  {
    if (t != NULL)
    {
      cJSON_AddNumberToObject(transaction, "RRN", t->rrn);
      cJSON_AddNumberToObject(transaction, "date", t->date);
      cJSON_AddStringToObject(transaction, "type", match_type(t->type));

      out = cJSON_Print(transaction);
      printf(out);

      free(out);
      cJSON_Delete(transaction);
    }

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
        printf("This is the first element in the list\n");
      break;

    case 'f':
    case 'F':
      if (t->next != NULL)
        t = t->next;
      else
        printf("This is the last element in the list\n");
      break;

    case 'm':
    case 'M':
      return;

    default:
      break;
    }
  }
}

/**********************************************************
 * match_type: Matches the transaction type enumeration   *
 *         its string equivalence. Prints invalid if type *
 *         doesn't exist.                                 *
 *********************************************************/
char *match_type(enum type type)
{
  switch (type)
  {
  case 0:
    return "purchase";
  case 1:
    return "withdrawal";
  case 2:
    return "deposit";
  case 3:
    return "refund";
  case 4:
    return "reversal";
  case 5:
    return "balance inquiry";
  case 6:
    return "payments";
  case 7:
    return "inter-account transfers";
  default:
    return "invalid";
  }
}

/**********************************************************
 * generate_rrn: Generates random 12-digit long integers. *
 *********************************************************/
long generate_rrn(void)
{
  srand(time(NULL));

  // generate 12 digit long numbers
  return ((long)rand() * time(NULL) % 100) % 1000000000000;
}
