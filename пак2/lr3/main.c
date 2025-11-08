#include "overprintf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

void print_menu() {
    printf("\n=== ТЕСТИРОВАНИЕ OVERPRINTF ===\n");
    printf("1.  Тест римских цифр (%%Ro)\n");
    printf("2.  Тест цекендорфова представления (%%Zr)\n");
    printf("3.  Тест систем счисления - нижний регистр (%%Cv)\n");
    printf("4.  Тест систем счисления - верхний регистр (%%CV)\n");
    printf("5.  Тест парсинга строк - нижний регистр (%%to)\n");
    printf("6.  Тест парсинга строк - верхний регистр (%%TO)\n");
    printf("7.  Тест дампа памяти int (%%mi)\n");
    printf("8.  Тест дампа памяти unsigned int (%%mu)\n");
    printf("9.  Тест дампа памяти double (%%md)\n");
    printf("10. Тест дампа памяти float (%%mf)\n");
    printf("11. Тест записи в файл\n");
    printf("12. Тест overprintf\n");
    printf("0.  Выход\n");
    printf("Выберите опцию: ");
}

void test_roman() {
    char buffer[BUFFER_SIZE];
    printf("\n--- Тест римских цифр (%%Ro) ---\n");

    printf("Введите целое число (1-3999): ");
    int num;
    if (scanf("%d", &num) == 1 && num > 0 && num <= 3999) {
        printf("overfprintf: ");
        overfprintf(stdout, "%Ro\n", num);

        oversprintf(buffer, BUFFER_SIZE, "oversprintf: %Ro", num);
        printf("%s\n", buffer);
    } else {
        printf("Ошибка ввода!\n");
    }
    while (getchar() != '\n');
}

void test_zeckendorf() {
    char buffer[BUFFER_SIZE];
    printf("\n--- Тест цекендорфова представления (%%Zr) ---\n");

    printf("Введите натуральное число (>0): ");
    unsigned int num;
    if (scanf("%u", &num) == 1 && num > 0) {
        printf("overfprintf: ");
        overfprintf(stdout, "%Zr\n", num);

        oversprintf(buffer, BUFFER_SIZE, "oversprintf: %Zr", num);
        printf("%s\n", buffer);
    } else {
        printf("Ошибка ввода! Переполнение\n");
    }
    while (getchar() != '\n');
}

void test_base_conversion_lower() {
    char buffer[BUFFER_SIZE];
    printf("\n--- Тест систем счисления - нижний регистр (%%Cv) ---\n");

    printf("Введите число и основание системы счисления (2-36): ");
    int num, base;
    if (scanf("%d %d", &num, &base) == 2 && base >= 2 && base <= 36) {
        printf("overfprintf: ");
        overfprintf(stdout, "%Cv\n", num, base);

        oversprintf(buffer, BUFFER_SIZE, "oversprintf: %Cv", num, base);
        printf("%s\n", buffer);
    } else {
        printf("Ошибка ввода!\n");
    }
    while (getchar() != '\n');
}

void test_base_conversion_upper() {
    char buffer[BUFFER_SIZE];
    printf("\n--- Тест систем счисления - верхний регистр (%%CV) ---\n");

    printf("Введите число и основание системы счисления (2-36): ");
    int num, base;
    if (scanf("%d %d", &num, &base) == 2 && base >= 2 && base <= 36) {
        printf("overfprintf: ");
        overfprintf(stdout, "%CV\n", num, base);

        oversprintf(buffer, BUFFER_SIZE, "oversprintf: %CV", num, base);
        printf("%s\n", buffer);
    } else {
        printf("Ошибка ввода!\n");
    }
    while (getchar() != '\n');
}

void test_parse_string_lower() {
    char buffer[BUFFER_SIZE];
    printf("\n--- Тест парсинга строк - нижний регистр (%%to) ---\n");

    char str[50];
    int base;
    printf("Введите строку и основание системы счисления (2-36): ");
    if (scanf("%49s %d", str, &base) == 2 && base >= 2 && base <= 36) {
        printf("overfprintf: ");
        overfprintf(stdout, "%to\n", str, base);

        oversprintf(buffer, BUFFER_SIZE, "oversprintf: %to", str, base);
        printf("%s\n", buffer);
    } else {
        printf("Ошибка ввода!\n");
    }
    while (getchar() != '\n');
}

void test_parse_string_upper() {
    char buffer[BUFFER_SIZE];
    printf("\n--- Тест парсинга строк - верхний регистр (%%TO) ---\n");

    char str[50];
    int base;
    printf("Введите строку (В ВЕРХНЕМ РЕГИСТРЕ) и основание системы счисления (2-36): ");
    if (scanf("%49s %d", str, &base) == 2 && base >= 2 && base <= 36) {
        printf("overfprintf: ");
        overfprintf(stdout, "%TO\n", str, base);


        oversprintf(buffer, BUFFER_SIZE, "oversprintf: %TO", str, base);
        printf("%s\n", buffer);
    } else {
        printf("Ошибка ввода!\n");
    }
    while (getchar() != '\n');
}

void test_memory_dump_int() {
    char buffer[BUFFER_SIZE];
    printf("\n--- Тест дампа памяти int (%%mi) ---\n");

    int num;
    printf("Введите целое число: ");
    if (scanf("%d", &num) == 1) {
        printf("overfprintf: ");
        overfprintf(stdout, "%mi\n", num);

        oversprintf(buffer, BUFFER_SIZE, "oversprintf: %mi", num);
        printf("%s\n", buffer);
    } else {
        printf("Ошибка ввода!\n");
    }
    while (getchar() != '\n');
}

void test_memory_dump_unsigned() {
    char buffer[BUFFER_SIZE];
    printf("\n--- Тест дампа памяти unsigned int (%%mu) ---\n");

    unsigned int num;
    printf("Введите беззнаковое целое число: ");
    if (scanf("%u", &num) == 1) {
        printf("overfprintf: ");
        overfprintf(stdout, "%mu\n", num);

        oversprintf(buffer, BUFFER_SIZE, "oversprintf: %mu", num);
        printf("%s\n", buffer);
    } else {
        printf("Ошибка ввода!\n");
    }
    while (getchar() != '\n');
}

void test_memory_dump_double() {
    char buffer[BUFFER_SIZE];
    printf("\n--- Тест дампа памяти double (%%md) ---\n");

    double num;
    printf("Введите вещественное число: ");
    if (scanf("%lf", &num) == 1) {
        printf("overfprintf: ");
        overfprintf(stdout, "%md\n", num);

        oversprintf(buffer, BUFFER_SIZE, "oversprintf: %md", num);
        printf("%s\n", buffer);
    } else {
        printf("Ошибка ввода!\n");
    }
    while (getchar() != '\n');
}

void test_memory_dump_float() {
    char buffer[BUFFER_SIZE];
    printf("\n--- Тест дампа памяти float (%%mf) ---\n");

    float num;
    printf("Введите вещественное число: ");
    if (scanf("%f", &num) == 1) {
        printf("overfprintf: ");
        overfprintf(stdout, "%mf\n", num);

        oversprintf(buffer, BUFFER_SIZE, "oversprintf: %mf", num);
        printf("%s\n", buffer);
    } else {
        printf("Ошибка ввода!\n");
    }
    while (getchar() != '\n');
}

void test_file_output() {
    printf("\n--- Тест записи в файл ---\n");
    
    // Фиксированное имя файла
    const char *filename = "test_output.txt";
    
    // Открываем файл для записи
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка: не удалось создать файл %s\n", filename);
        return;
    }
    
    printf("Записываем данные в файл: %s\n", filename);
    
    // Тестируем все спецификаторы в файл
    overfprintf(file, "=== ТЕСТ OVERFPRINTF В ФАЙЛ ===\n\n");
    
    overfprintf(file, "1. Римские цифры: %Ro\n", 2023);
    overfprintf(file, "2. Цекендорфово: %Zr\n", 11);
    overfprintf(file, "3. Системы счисления: %Cv и %CV\n", 255, 16, 255, 16);
    overfprintf(file, "4. Парсинг строк: %to и %TO\n", "ff", 16, "FF", 16);
    overfprintf(file, "5. Дамп int: %mi\n", 255);
    overfprintf(file, "6. Дамп unsigned: %mu\n", 255U);
    overfprintf(file, "7. Дамп float: %mf\n", 3.14f);
    overfprintf(file, "8. Дамп double: %md\n", 3.14);
    
    overfprintf(file, "\n=== КОНЕЦ ТЕСТА ===\n");
    
    // Закрываем файл
    fclose(file);
    
    printf("Данные успешно записаны в файл!\n");
    printf("Откройте файл '%s' чтобы посмотреть результат\n", filename);
    
    // Дополнительно: читаем и показываем содержимое файла
    printf("\nСодержимое файла:\n");
    printf("-----------------\n");
    
    file = fopen(filename, "r");
    if (file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);
    }
}

// тест overprintf ! :)
void test_overprintf() {
    overprintf("%d   %%     %Ro", 4, 5);
}

int main() {
    int choice = -1;
    while (choice != 0) {
        print_menu();
        if (scanf("%d", &choice) != 1) break;
        while (getchar() != '\n');

        switch (choice) {
            case 1: test_roman(); break;
            case 2: test_zeckendorf(); break;
            case 3: test_base_conversion_lower(); break;
            case 4: test_base_conversion_upper(); break;
            case 5: test_parse_string_lower(); break;
            case 6: test_parse_string_upper(); break;
            case 7: test_memory_dump_int(); break;
            case 8: test_memory_dump_unsigned(); break;
            case 9: test_memory_dump_double(); break;
            case 10: test_memory_dump_float(); break;
            case 11: test_file_output(); break;
            case 12: test_overprintf(); break;
            case 0: printf("Выход.\n"); break;
            default: printf("Некорректный выбор.\n");
        }
    }

    return 0;
}
