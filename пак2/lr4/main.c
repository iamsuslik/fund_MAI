#include "overscanf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void demo_roman_interactive(void) {
    printf("\n=== Римские цифры (%%Ro) ===\n");
    printf("Введите римское число (I, V, X, L, C, D, M): ");
    
    char input[100];
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;
        
        int value;
        int result = oversscanf(input, "%Ro", &value);
        
        if (result == 1) {
            printf("Преобразование успешно: %s = %d\n", input, value);
        } else {
            printf("Ошибка: неверный формат римского числа\n");
            printf("Код ошибки: %d\n", result);
        }
    }
}

void demo_zekendorf_interactive(void) {
    printf("\n=== Цекендорфово представление (%%Zr) ===\n");
    printf("Введите цекендорфово представление (только 0 и 1): ");
    
    char input[100];
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;
        
        unsigned int value;
        int result = oversscanf(input, "%Zr", &value);
        
        if (result == 1) {
            printf("Преобразование успешно: %s = %u\n", input, value);
        } else {
            printf("Ошибка: неверный формат цекендорфова представления\n");
            printf("Код ошибки: %d\n", result);
            if (result == OVERSCANF_TOO_LONG) {
                printf("(Слишком длинная последовательность)\n");
            }
        }
    }
}

void demo_custom_base_interactive(void) {
    printf("\n=== Произвольная система счисления (%%Cv/%%CV) ===\n");
    
    char input[100];
    int base;
    char format_choice;
    
    printf("Выберите регистр (L - нижний, U - верхний): ");
    scanf("%c", &format_choice);
    clear_input_buffer();
    
    printf("Введите основание системы счисления (2-36): ");
    scanf("%d", &base);
    clear_input_buffer();

    if (base < 2 || base > 36) {
        printf("Основание %d недопустимо. Установлено base=10\n", base);
        base = 10;
    }
    
    printf("Введите число: ");
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;
        
        int value;
        int result;
        
        if (format_choice == 'U' || format_choice == 'u') {
            result = oversscanf(input, "%CV", &value, base);
        } else {
            result = oversscanf(input, "%Cv", &value, base);
        }
        
        if (result == 1) {
            printf("Преобразование успешно: %s (base %d) = %d\n", input, base, value);
        } else {
            printf("Ошибка преобразования\n");
            printf("Код ошибки: %d\n", result);
        }
    }
}

void demo_mixed_interactive(void) {
    printf("\n=== Смешанный формат ===\n");
    printf("Пример формата: XIV 101 1A\n");
    printf("Введите данные (римские цекендорф шестнадцатеричные): ");
    
    char input[100];
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;
        
        int roman_val = 0, hex_val = 0;
        unsigned int zek_val = 0;
        
        printf("Введена строка: '%s'\n", input);
        
        int result = oversscanf(input, "%Ro %Zr %CV", &roman_val, &zek_val, &hex_val, 16);
        
        printf("Результат oversscanf = %d\n", result);
        
        if (result == 3) {
            printf("Преобразование успешно:\n");
            printf("  Римские: %d\n", roman_val);
            printf("  Цекендорф: %u\n", zek_val);
            printf("  Шестнадцатеричные: %d\n", hex_val);
        } else {
            printf("✗ Ошибка: ожидалось 3 значения, получено %d\n", result);
            printf("  Убедитесь, что вводите числа через пробелы: 'XIV 101 FF'\n");
            if (result > 0) {
                printf("  Успешно прочитано: %d значений\n", result);
                if (result >= 1) printf("  Римские: %d\n", roman_val);
                if (result >= 2) printf("  Цекендорф: %u\n", zek_val);
                if (result >= 3) printf("  Шестнадцатеричные: %d\n", hex_val);
            }
        }
    }
}

void demo_file_operations(void) {
    printf("\n=== Чтение из файла ===\n");
    
    // Создаем тестовый файл
    FILE *file = fopen("test_data.txt", "w");
    if (file) {
        fprintf(file, "XIV 101 FF 42\n");
        fprintf(file, "XLII 1001 1A 123\n");
        fprintf(file, "XC 10001 CD 99\n");
        fprintf(file, "MMXXIII 1010101 C 2023\n");
        fclose(file);
        printf("Создан тестовый файл test_data.txt\n");
    }
    
    // Читаем из файла
    file = fopen("test_data.txt", "r");
    if (file) {
        printf("Содержимое файла:\n");
        printf("================\n");
        
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        
        printf("\nПарсинг с overfscanf:\n");
        printf("====================\n");
        
        rewind(file);
        
        int roman_val, dec_val, hex_val;
        unsigned int zek_val;
        int line_count = 0;
        int success_count = 0;

        while (fgets(line, sizeof(line), file)) {
            line_count++;


            line[strcspn(line, "\n")] = 0;

            if (strlen(line) == 0) continue; // Пропускаем пустые строки
            
            printf("Строка %d: '%s'\n", line_count, line);
            

            int result = oversscanf(line, "%Ro %Zr %CV %d", &roman_val, &zek_val, &hex_val, 16, &dec_val);
            
            if (result == 4) {
                printf("  УСПЕХ:\n");
                printf("    Римские: %d\n", roman_val);
                printf("    Цекендорф: %u\n", zek_val);
                printf("    Шестнадцатеричные: %d\n", hex_val);
                printf("    Десятичные: %d\n", dec_val);
                success_count++;
            } else {
                printf("  ✗ ОШИБКА\n");
                
                // Покажем что удалось прочитать
                if (result >= 1) printf("    римские=%d\n", roman_val);
                if (result >= 2) printf("    цекендорф=%u\n", zek_val);
                if (result >= 3) printf("    hex=%d\n", hex_val);
                if (result >= 4) printf("    десятичные=%d\n", dec_val);
                
                if (result < 0) {
                    printf("    код ошибки: %d\n", result);
                }
            }
            printf("  ---\n");
        }
        
        fclose(file);
        printf("Успешно обработано %d/%d строк\n", success_count, line_count);
        
        remove("test_data.txt");
        
    } else {
        printf("Ошибка открытия файла\n");
    }
}


void test_overscanf() {
    printf("Начало test_overscanf...\n");
    
    int a = 0;
    int b = 0;
    
    printf("Введите число и римскую цифру: ");
    fflush(stdout);
    
    int result = overscanf("%d %Ro", &a, &b);
    printf("Результат: %d, a=%d, b=%d\n", result, a, b);
}


void show_menu(void) {
    printf("\n=========================================\n");
    printf("           ИНТЕРАКТИВНАЯ ДЕМОНСТРАЦИЯ\n");
    printf("=========================================\n");
    printf("1. Римские цифры (%%Ro)\n");
    printf("2. Цекендорфово представление (%%Zr)\n");
    printf("3. Произвольная система счисления (%%Cv/%%CV)\n");
    printf("4. Смешанный формат\n");
    printf("5. Чтение из файла\n");
    printf("6. Тест overscanf\n");
    printf("0. Выход\n");
    printf("=========================================\n");
    printf("Выберите опцию: ");
}



int main(void) {
    int choice;
    do {
        show_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Неверный ввод!\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        
        switch (choice) {
            case 1:
                demo_roman_interactive();
                break;
            case 2:
                demo_zekendorf_interactive();
                break;
            case 3:
                demo_custom_base_interactive();
                break;
            case 4:
                demo_mixed_interactive();
                break;
            case 5:
                demo_file_operations();
                break;
            case 6:
                test_overscanf();
                break;
            case 0:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Неверный выбор! Попробуйте снова.\n");
                break;
        }
        
        if (choice != 0) {
            printf("\nНажмите Enter для продолжения...");
            clear_input_buffer();
        }
        
    } while (choice != 0);
    
    return 0;
}
