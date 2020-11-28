#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

bool isSameDay(const int year, const int month, const int day, const struct transaction *t);
bool isLesserOrEqualDay(const int year, const int month, const int day, const struct transaction *t);
bool isGreaterOrEqualDay(const int year, const int month, const int day, const struct transaction *t);

#endif // UTIL_H