#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <time.h>

enum transactionType
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
  enum transactionType type;
  time_t date;
  int year;
  int month;
  int day;
  struct transaction *previous;
  struct transaction *next;
};

#endif // TRANSACTION_H