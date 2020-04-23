#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "header.h"

//возвращает True, если символ является оператором
int is_operator(char c){
    return (c == '-' || c == '+' || c == '*' || c == '/' || c == '^' || c == '%');
}

//возвращает True, если символ является числом
int is_digit(char c){
    return (c >= '0' && c <= '9');
}

//возвращает приоритет операции
int priority(char c){
    if (c == '+' || c == '-')
        return 1;
    else if (c == '*' || c == '/' || c == '%')
        return 2;
    else if (c == '^')
        return 3;
    else
        return 0;
}

//так как на линуксе необходимо самостоятельно подключать библиотеку math, то
// я написала свою функцию, чтобы не делать это каждый раз
float ft_powf(float a, float b){
    while (--b){
        a = a * a;
    }
    return a;
}

//Высчитываем простое действие
float score(float a, float b, char c){
    if (c == '+')
        return a+b;
    else if (c == '-')
        return a-b;
    else if (c == '*')
        return a*b;
    else if (c == '/')
        return a/b;
    else if (c == '^')
        return ft_powf(a, b);
    else if (c == '%')
        return ((float)((int)a%(int)b));
    else{
        printf("%s", "Вероятно, вы ввели некорректное выражение.\n");
        return 0;
    }
}

//Сдвигаем указатель в нашем выражении
char *new_input(char *input){
    while (is_digit(*input) || *input == '.')
        input++;
    return input;
}

//Добавляем число в стек
void push_digit(float *digit, float num){
    int i = -1;
    while (i++ < SIZE)
        if (!(digit[i])){
            digit[i] = num;
            return;
        }
}

//Добавляем оператор в стек
void push_sign(char *sign, char c){
    int i = -1;
    while (i++ < SIZE)
        if (!(sign[i])){
            sign[i] = c;
            return;
        }
}

//Достает число из стека
float pop_digit(float *digit){
    int i = 0;
    float tmp;

    if (!digit[0])
        return 0;
    while(digit[i])
        i++;
    tmp = digit[i-1];
    digit[i-1] = 0;
    return tmp;
}

//Достаем оператор из стека
char pop_sign(char *sign){
    int i = 0;
    char tmp;

    if (!sign[i])
        return (char)0;
    while(sign[i])
        i++;
    tmp = sign[i-1];
    sign[i-1] = 0;
    return tmp;
}

//Выполняет действия в зависимости от приоритета операции
void case_operator(float *digit, char *sign, char str)
{
    char tmp;
    float a, b;

    tmp = pop_sign(sign);
    if (!tmp){
        push_sign(sign, str);
    }
    else if (priority(tmp) < priority(str)){
        push_sign(sign, tmp);
        push_sign(sign, str);
    }
    else{
        while(priority(tmp) >= priority(str)){
            a = pop_digit(digit);
            b = pop_digit(digit);
            push_digit(digit, score(b, a, tmp));
            tmp = pop_sign(sign);
        }
        push_sign(sign, tmp);
        push_sign(sign, str);
    }
}

// Досчитывает остатки в стеках
void score_expr(float *digit, char *sign){
    float a, b;

    while (sign[0]){
        a = pop_digit(digit);
        b = pop_digit(digit);
        push_digit(digit, score(b, a, pop_sign(sign)));
    }
}

//Основная функция для вычисления выражения
float eval_expr(char *str){
    float rez, a, b;
    char tmp;
    float digit[SIZE] = {0};
    char sign[SIZE] = {0};

    while (*str != '\0'){
        if (is_digit(*str)){
            push_digit(digit, atof(str));
            str = new_input(str);
        }
        else if (is_operator(*str)){
            case_operator(digit, sign, *str);
            str++;
        }
        else if (*str == '(')
            push_sign(sign, *str++);
        else if (*str == ')'){
            while ((tmp = pop_sign(sign)) != '(') {
                a = pop_digit(digit);
                b = pop_digit(digit);
                push_digit(digit, score(b, a, tmp));
            }
            str++;
        }
        else
            str++;
    }

//    Если стек операторов не пуст, то нужно досчитать
    if (sign[0])
        score_expr(digit, sign);
    rez = pop_digit(digit);

//    Если в стеках еще что-то есть, значит в выражении ошибка
    if (sign[0] || digit[0]){
        printf("%s", "Вероятно, вы ввели некорректное выражение.\n");
        return 0;
    }
    return rez;
}


int main(int ac, char **av)
{
    if (ac == 2){
        printf("%.2f\n", eval_expr(av[1]));
//        printf("%.2f\n", eval_expr("1+2*(3+4/2-(1+2))*2+1"));  // 10
//        printf("%.2f\n", eval_expr("1+2*(3-5)%2"));  // 1
//        printf("%.2f\n", eval_expr("1+2*(3-5)*2"));  // -7
//        printf("%.2f\n", eval_expr("1 + 2 * (3 - 42) % 5"));  // -2
//        printf("%.2f\n", eval_expr("1 + 2 * (3 - 42) * 5"));  // -389
//        printf("%.2f\n", eval_expr("1 + 2 * (3 - 5) * 5"));  // -19
//        printf("%.2f\n", eval_expr("33 + 9 * 3"));  //60
//        printf("%.2f\n", eval_expr("33+9*3-10"));  //50
//        printf("%.2f\n", eval_expr("(12+2)^2"));  //196
//        printf("%.2f\n", eval_expr("9*3+10"));  // 37
//        printf("%.2f\n", eval_expr("9*3 * 2"));  // 54
//        printf("%.2f\n", eval_expr("(2+4)*(10-1)"));  // 54
//        printf("%.2f\n", eval_expr("(12+2)*2"));  // 28
//        printf("%.2f\n", eval_expr("((3.5+5.5)*(4-3))/(7-3)"));  // 2.25
//        printf("%.2f\n", eval_expr("((3.5+55.76)*(4-3))/(77-3.33)"));  // 0.80
        return 0;
    }
    printf("%s", "Usage: ./a.out '1+(2+3)*2/4'\n\nAll operators: + - * / ^ % ( )\n"
                 "Allowed type of numbers: 25б, 100, 23.2\n"
                 "Please, input your expression without spaces.\n");
    return 0;
}
