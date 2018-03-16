#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void to_lower_string(char *s) {
    int i; for (i = 0; s[i]; i++) s[i] = tolower(s[i]);
}
int is_number(char *s) {
    double num;
    char buff[40];
    strcpy(buff, s);
    int i, dotCount, negative = 0;
    // Check negative
    if (buff[0] == '-') negative = 1;
    if (negative) {
        int i;
        for (i = 0; i <  strlen(buff) - 1;i++) {
            buff[i] = buff[i + 1];
        }
        buff[strlen(buff)] = '\0';
    }

    // Check digit
    for (i=0; i < strlen(buff); i++) {
        if (isdigit(buff[i])==0 && buff[i] != '.')
            return 0;
    }
    // Check format for double or float
    i = dotCount = 0;
    for (i = 0; i < strlen(buff); i++) {
        if (buff[i] == '.') dotCount++;
    }
    if (dotCount < 2) return 1;
    else return 0;
}

void add_space (char *s) {
    int i,j;
    char temp[200] = "", old[200] = "";
    for (i=0,j=strlen(s); i < j;i++) {
        if (strchr(",+-*/^()", s[i]) != NULL)
            sprintf(temp,"%s %c ", old, s[i]);
        else
            sprintf(temp,"%s%c",old, s[i]);
        strcpy(old, temp);
    }
    strcpy(s,temp);
}

void remove_space_at_border (char * s) {
    while(s[strlen(s) - 1] == ' ') s[strlen(s) - 1] = '\0'; // Remove space in back
    while (s[0] == ' ') strcpy(s, s + 1); // Remove space in front
}

void space_split (char *s, char data[][40], int *count) {
    char * word;
    word = strtok(s, " ");
    while (word != NULL) {
        strcpy(data[*count], word);
        *count = *count + 1;
        //printf("data[%d] = %s\n", *count - 1, word);
        word = strtok(NULL, " ");
    }
    data[*count][0] = '\0';
}

int is_function (char *s) {
    const char fnList[11][10] = {"sin", "cos", "tan", "asin", "acos", "atan", "sqrt", "log", "exp", "pow", "abs"};
    int i;
    for (i=0;i<11;i++) {
        if (strcmp(s, fnList[i])==0)
            return 1;  // If string is equal function name return 1(TRUE)
    }
    return 0;
}

int is_sign (char * s) {
    int i;
    char list[4][10] = {"ans", "pi", "g", "e"};
    for (i = 0; i < 4; i++) {
        if (strcmp(s, list[i]) == 0) return 1;
    }
    return 0;
}


int is_operation (char *s) {
    int i;
    for (i = 0; i < strlen(s); i++) {
        if (strchr(",+-*/^", s[i]) != NULL)
            return 1;
    }
    return 0;
}

void space_combine (char data[][40], int *count) {
    int i;
    for (i = 0; i < *count; i++) {
        if (data[i] != '\0') {
            if (strcmp(data[i], "-") == 0) {
                if (
                    is_number(data[i+1]) &&
                        (
                         (i - 1 >= 0 && strcmp(data[i-1],"(") == 0) || i - 1 < 0
                        )
                    ) {
                    char buff[40];
                    strcpy(buff, "-");
                    strcat(buff, data[i+1]);
                    strcpy(data[i], buff);
                    int j;
                    for (j = i + 1; j < *count - 1; j++ ) {
                        strcpy(data[j], data[j+1]);
                    }
                    data[*count][0] = '\0';
                    *count = *count - 1;
                }
            }
        }
    }
}

int valid_parentheses (char data[][40], int *count) {
    int i, open, close;
    for (i = 0, open = close = 0; i < *count; i++) {
        if (strcmp(data[i], "(") == 0) open++;
        if (strcmp(data[i], ")") == 0) {
            close++;
            if (close > open) {
                return 0; // Invalid
            }
        }
    }
    if (open != close) {
        return 0; // Invalid
    }
    return 1; // valid
}

int check_next_valid (char * c, char * next) {
    if (is_number(c)) {
        //printf("%s, number\n", c);
        if (next == NULL) return 1;
        if ((is_operation(next) || strcmp(next, ")") == 0 || strcmp(next, "") == 0) && strcmp(next, "(") != 0 ) return 1;
    } else if (is_operation(c)) {
        //printf("%s, operation\n", c);
        if (next == NULL) return 0; // Invalid
        if (is_number(next) || strcmp(next, "(") == 0 || is_function(next) || is_sign(next)) return 1;
    } else if (is_function(c)) {
        //printf("%s, function\n", c);
        if (next == NULL) return 0; // Invalid
        if (strcmp(next, "(") == 0) return 1;
    } else if (is_sign(c)) {
        //printf("%s, sign\n", c);
        if (next == NULL) return 1;
        if (is_number(next) || is_function(next) || strcmp(next, "(") == 0 || strcmp(next, ")") == 0) return 1;
    } else if (strcmp(c, "(") == 0) {
        //printf("%s, (\n", c);
        if (next == NULL) return 0; // Invalid
        if (strcmp(next, "(") == 0 || is_function(next) || is_number(next) || is_sign(next)) return 1;
    } else if (strcmp(c, ")") == 0) {
        //printf("%s, )\n", c);
        if (next == NULL) return 1;
        if (is_operation(next) || strcmp(next, ")") == 0 || strcmp(next, "") == 0 || strcmp(next, "\0") == 0) return 1;
    }
    // Invalid
    //printf("%s -> %s\n", c, next);
    return 0;
}

void print_data (char data[][40], int *count) {
    int i;
    for (i = 0; i < *count; i++) {
        printf("data[%d] = %s\n", i, data[i]);
    }
}

int main () {

    char input[100], data[100][40];
    do
    {
        int dataCount = 0;
        rewind(stdin);
        strcpy(input, "");
        printf("expression> ");
        gets(input);
        add_space(&input);
        remove_space_at_border(&input); // Remove space
        to_lower_string(&input); // Fix case insensitive
        if (strcmp(input, "help") == 0) {
            printf("answer> token = sin,cos,tan,asin,acos,atan,sqrt,log,exp,abs+,-,*,/,^,(,),pi,ans\n");
        } else if (strcmp(input, "end") != 0){
            space_split(&input, data, &dataCount);
            space_combine(data, &dataCount);
            //print_data(data, &dataCount);
            int i = 0, error = 0;
            for (i = 0; strcmp(data[i], "") != 0; i++) {
                if (i + 1 >= dataCount) {
                    error += !check_next_valid(data[i], NULL);
                } else {
                    error += !check_next_valid(data[i], data[i + 1]);
                }
            }
            if (valid_parentheses(data, &dataCount) == 0) {
                //printf("parentheses\n");
                error += 1;
            }
            if (error > 0) {
                printf("answer> Error\n");
            } else {
                printf("answer> Ok\n");
            }
        }

    } while(strcmp(input, "end") != 0);

    printf("End program\n");
    printf("Program writen by 60070501029 Nitipoom Unrrom");

    return 1;
}
