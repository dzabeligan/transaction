CFLAGS = -g -Wall --std=gnu99

transaction: src/transaction.exe
  gcc -o transaction src/main.c src/transaction.c src/utils.c src/cJSON.c -g -Wall --std=gnu99
