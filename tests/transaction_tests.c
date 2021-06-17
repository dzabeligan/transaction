#include <stdio.h>

#include <cJSON.h>
#include <transaction.h>
#include <utils.h>

#include "minunit.h"

/**********************************************************
 * main: Prompts the user to enter a command then         *
 *         executes it while in a continious loop         *
 *         Loop terminates when 'x' or 'X' key is pressed *
 *********************************************************/
char * test()
{
  printf("Commands\n");
  printCommand();

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
      addTransaction();
      break;

    case 'g':
    case 'G':
      getTransaction();
      break;

    case 't':
    case 'T':
      traverseTransaction();
      break;

    case 'd':
    case 'D':
      deleteTransaction();
      break;

    case 'u':
    case 'U':
      updateTransaction();
      break;

    case 'm':
    case 'M':
      printCommand();
      break;

    case 'x':
    case 'X':
      freeTransaction();
      return 0;

    default:
      printCommand();
      printf("\nInvalid command\n");
      break;
    }
  }

  return NULL;
}

char *allTests() {
  mu_suite_start();

  mu_run_test(test);

  return NULL;
}

RUN_TESTS(allTests);
