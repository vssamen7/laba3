#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>

#define ACCOUNT_LENGTH 20 // Длина строки для номера счёта

// Структуры
typedef struct {
    unsigned int value : 32; // Битовое поле для хранения числа (задача №1)
} BitField;

typedef struct {
    char accountNumber[ACCOUNT_LENGTH]; // Номер счёта
    double depositAmount;               // Сумма вклада
} Account;

typedef struct {
    int passportNumber; // Номер паспорта
    char *surname;      // Указатель на фамилию
    Account account;    // Вложенная структура для счёта
} Client;

// Общие функции
int correct_choice();
void inputSurname(char **surname);
double inputDeposit();

// Задача №1
void task1();

// Задача №2
void inputClient(Client *client);
void printClient(const Client *client);
void menuTask2();

#endif // FUNCTIONS_H


