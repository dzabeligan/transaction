#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

bool isSameDay(int year, int month, int day, struct transaction *t);
bool isLesserOrEqualDay(int year, int month, int day, struct transaction *t);
bool isGreaterOrEqualDay(int year, int month, int day, struct transaction *t);

#endif // UTIL_H