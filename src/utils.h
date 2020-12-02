#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

bool isSameDay(const int year, const int month, const int day, const struct transaction *t);
bool isLesserOrEqualDay(const int year, const int month, const int day, const struct transaction *t);
bool isGreaterOrEqualDay(const int year, const int month, const int day, const struct transaction *t);
long generateRRN(void);
char *matchType(enum transactionType type);
void printCommand(void);
void printGetCommand(void);
void printTypes(void);

#endif // UTIL_H