#include <stdbool.h>

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
