#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <transaction.h>

bool isSameDay(const int year, const int month, const int day, const struct transaction* t);
bool isLesserOrEqualDay(const int year, const int month, const int day, const struct transaction* t);
bool isGreaterOrEqualDay(const int year, const int month, const int day, const struct transaction* t);
long generateRRN(void);
char* matchType(enum transactionType type);
void printCommand(void);
void printGetCommand(void);
void printTypes(void);

#ifdef __cplusplus
}
#endif

#endif // UTIL_H
