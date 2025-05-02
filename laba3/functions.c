#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Общая функция проверки ввода
int correct_choice() {
    int task;
    while (1) {
        char b;
        if (scanf("%d%c", &task, &b) == 2 && b == '\n' && task >= 0) {
            break;
        } else {
            printf("Некорректный ввод. Повторите еще раз: ");
            while (getchar() != '\n');
        }
    }
    return task;
}

void inputSurname(char **surname) {
    char buffer[100];
    int valid = 0;

    while (!valid) {
        printf("Введите фамилию клиента (только буквы): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            clearerr(stdin); // Сброс ошибок ввода
            continue;
        }

        // Удаление '\n' и проверка на переполнение буфера
        char *newline = strchr(buffer, '\n');
        if (newline != NULL) {
            *newline = '\0';
        } else {
            // Очистка буфера, если введено больше 99 символов
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }

        // Проверка на пустую строку
        size_t len = strlen(buffer);
        if (len == 0) {
            printf("Фамилия не может быть пустой.\n");
            valid = 0;
            continue;
        }

        // Проверка символов
        valid = 1;
        for (size_t i = 0; i < len; i++) {
            if (!isalpha((unsigned char)buffer[i])) {
                printf("Некорректный символ: '%c'\n", buffer[i]);
                valid = 0;
                break;
            }
        }
    }

    // Выделение памяти
    *surname = malloc(strlen(buffer) + 1);
    if (*surname == NULL) {
        perror("Ошибка выделения памяти");
        exit(EXIT_FAILURE);
    }
    strcpy(*surname, buffer);
}
// Ввод суммы вклада
double inputDeposit() {
    char buffer[100];
    double deposit;
    while (1) {
        printf("Введите сумму вклада (может быть дробной): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Ошибка ввода.\n");
            continue;
        }

        char *endptr;
        deposit = strtod(buffer, &endptr);
        if (*endptr == '\0' || *endptr == '\n') {
            if (deposit >= 0) {
                return deposit;
            } else {
                printf("Сумма вклада не может быть отрицательной.\n");
            }
        } else {
            printf("Некорректный ввод. Введите только цифры и точку.\n");
        }
    }
}

// Реализация задачи №1
void task1() {
    BitField number;
    printf ("Введите целое число:");
    number.value = correct_choice();

    unsigned int halfNumber = number.value >> 1;
    printf("Число, в два раза меньше заданного: %u\n", halfNumber);
}

// Ввод клиента
void inputClient(Client *client) {
    printf("Введите номер паспорта: ");
    client->passportNumber = correct_choice();

   
    inputSurname(&client->surname);

    printf("Введите номер счёта: ");
    scanf("%s", client->account.accountNumber);
    while (getchar() != '\n'); // Очистка

    client->account.depositAmount = inputDeposit();
}

// Вывод клиента
void printClient(const Client *client) {
    printf("Номер паспорта: %d\n", client->passportNumber);
    printf("Фамилия: %s\n", client->surname);
    printf("Номер счёта: %s\n", client->account.accountNumber);
    printf("Сумма вклада: %.2f\n", client->account.depositAmount);
}

// Поиск клиента по номеру паспорта
int findClientByPassport(Client *clients, int count, int passportNumber) {
    for (int i = 0; i < count; i++) {
        if (clients[i].passportNumber == passportNumber) {
            return i;
        }
    }
    return -1; // Клиент не найден
}

// Удаление клиентов по фамилии
void deleteClientsBySurname(Client **clients, int *count, const char *surname) {
    int i = 0;
    while (i < *count) {
        if (strcmp((*clients)[i].surname, surname) == 0) {
            free((*clients)[i].surname);
            for (int j = i; j < *count - 1; j++) {
                (*clients)[j] = (*clients)[j + 1];
            }
            (*count)--;
            *clients = (Client *)realloc(*clients, (*count) * sizeof(Client));
        } else {
            i++;
        }
    }
}

// Меню задачи №2
void menuTask2() {
    Client *clients = NULL;
    int clientCount = 0, choice;

    do {
        printf("\nМеню задачи №2:\n");
        printf("1. Добавить клиента\n");
        printf("2. Просмотреть всех клиентов\n");
        printf("3. Найти клиента по номеру паспорта\n");
        printf("4. Удалить клиентов по фамилии\n");
        printf("5. Выход\n");
        printf("Введите ваш выбор: ");
        choice = correct_choice();

        switch (choice) {
            case 1:
                clientCount++;
                clients = (Client *)realloc(clients, clientCount * sizeof(Client));
                if (clients == NULL) {
                    printf("Ошибка выделения памяти!\n");
                    exit(1);
                }
                inputClient(&clients[clientCount - 1]);
                break;
            case 2:
                for (int i = 0; i < clientCount; i++) {
                    printf("\nКлиент %d:\n", i + 1);
                    printClient(&clients[i]);
                }
                break;
            case 3: {
                int passportNumber;
                printf("Введите номер паспорта для поиска: ");
                passportNumber = correct_choice();
                int index = findClientByPassport(clients, clientCount, passportNumber);
                if (index != -1) {
                    printf("\nКлиент найден:\n");
                    printClient(&clients[index]);
                } else {
                    printf("Клиент с указанным номером паспорта не найден.\n");
                }
                break;
            }
            case 4: {
                char surname[100];
                printf("Введите фамилию для удаления: ");
                scanf("%s", surname);
                while (getchar() != '\n');
                deleteClientsBySurname(&clients, &clientCount, surname);
                printf("Клиенты с фамилией \"%s\" удалены.\n", surname);
                break;
            }
            case 5:
                printf("Возврат в главное меню.\n");
                break;
        }
    } while (choice != 5);

    for (int i = 0; i < clientCount; i++) {
        free(clients[i].surname);
    }
    free(clients);
}


