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

/* prototypes */
struct transaction *findTransactionByRRN(long rrn);
void freeTransaction(void);
void addTransaction(void);
void getTransaction(void);
void getTransactionByRRN(void);
void getTransactionsByDate(void);
void getTransactionsByDateRange(void);
void getTransactionsByType(void);
void updateTransaction(void);
void deleteTransaction(void);
void traverseTransaction(void);

#endif // TRANSACTION_H