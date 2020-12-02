#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "transaction.h"
#include "utils.h"

/**********************************************************
 * isSameDay: Compares two dates. Returns true if dates *
 *           are equal, false otherwise                   *
 *********************************************************/
bool isSameDay(const int year, const int month, const int day, const struct transaction *t)
{
  return year == t->year && month == t->month && day == t->day;
}

/**********************************************************
 * isGreaterOrEqualDay: Compares two dates. Returns   *
 *           true if date in transaction is greater or    *
 *           equal to test date, false otherwise          *
 *********************************************************/
bool isGreaterOrEqualDay(const int year, const int month, const int day, const struct transaction *t)
{
  return t->year > year 
        || (t->year == year && t->month > month) 
        || (t->year == year && t->month == month && t->day >= day);
}

/**********************************************************
 * isLesserOrEqualDay: Compares two dates. Returns    *
 *           true if date in transaction is lesser or     *
 *           equal to test date, false otherwise          *
 *********************************************************/
bool isLesserOrEqualDay(const int year, const int month, const int day, const struct transaction *t)
{
  return t->year < year 
        || (t->year == year && t->month <= month) 
        || (t->year == year && t->month == month && t->day <= day);
}

/**********************************************************
 * generateRRN: Generates random 12-digit long integers. *
 *********************************************************/
long generateRRN(void)
{
  srand(time(NULL));

  // generate 12 digit long numbers
  return ((long)rand() * time(NULL) % 100) % 1000000000000;
}

/**********************************************************
 * matchType: Matches the transaction type enumeration   *
 *         its string equivalence. Prints invalid if type *
 *         doesn't exist.                                 *
 *********************************************************/
char *matchType(enum transactionType type)
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
 * printCommand: Prints the available commands to console*
 *********************************************************/
void printCommand(void)
{
  printf(
      "\n a - add transaction,"
      "\n g - get transaction,"
      "\n d - delete transaction by RRN,"
      "\n u - update transaction by RRN"
      "\n t - traverse transactions"
      "\n x - terminate program\n");
}

/**********************************************************
 * printGetCommand: Prints the available commands for   *
 *           getting transactions to console              *
 *********************************************************/
void printGetCommand(void)
{
  printf(
      "\n 1 - get transaction by RRN,"
      "\n 2 - get transaction by date,"
      "\n 3 - get transaction by date range,"
      "\n 4 - get transaction by type,"
      "\n q - go back to menu,"
      "\n x - terminate program\n");
}

/**********************************************************
 * printTypes: Prints the available transaction    *
 *           types to console                             *
 *********************************************************/
void printTypes(void)
{
  printf("\n0 - purchase,\n"
         "1 - withdrawal,\n"
         "2 - deposit,\n"
         "3 - refund,\n"
         "4 - reversal,\n"
         "5 - balance inquiry,\n"
         "6 - payments,\n"
         "7 - inter-account transfer\n\n");
}
