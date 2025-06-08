#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>

#define up 72
#define down 80
#define enterKey 13

int i = 0, key = 0, val = 0;

struct credentials {
    char fname[30], lname[30], username[80], email[60], password[50];
    struct dob {
        int day, month, year;
    } d;
};

char *SignIn_Options[] = {
    "Sign In",
    "Sign Up",
    "Forgot Password",
    "Contact Admin",
    "Exit",
};

// Function prototypes
void ThemeArt();
void SignInMenu();
void KeyPress();
void signUp();
int inputNumberInRange(const char *prompt, int min, int max, int digitLimit);
void inputStringWithValidation(char *prompt, char *input, int maxLen, int (*validationFunc)(char *));
int validateName(char *str);
int validateUsername(char *str);
int validateEmail(char *email);

// ThemeArt function (unchanged)
void ThemeArt() {
    system("cls");
    printf("      ___                             ___           ___           ___           ___     \n");
    printf("     /  /\\                           /__/\\         /__/\\         /  /\\         /  /\\    \n");
    printf("    /  /:/                          |  |::\\        \\  \\:/       /  /:/_       /  /:/_   \n");
    printf("   /  /:/                           |  |:|::\\        \\  \\:/     /  /:/ /\\     /  /:/ /\\  \n");
    printf("  /  /:/  ___                     __|__|:|\\:\\   ___  \\  \\:/   /  /:/ /::\\   /  /:/ /:/_ \n");
    printf(" /__/:/  /  /\\                   /__/::::| \\\\:\\ /__/\\  \\\\__\\:/ /__/:/ /:/\\:\\ /__/:/ /:/ /\\\n");
    printf(" \\  \\:\\ /  /:/                   \\  \\:\\~~\\__\\/ \\  \\:\\/  /:/ \\  \\:\\/:/~/:/ \\  \\:\\/:/ /:/\n");
    printf("  \\  \\:\\  /:/                     \\  \\:\\        \\  \\::/   \\  \\::/ /:/   \\  \\::/ /:/ \n");
    printf("   \\  \\:\\/:/                       \\  \\:\\        \\  \\:\\/     \\__\\/ /:/     \\  \\:\\/:/  \n");
    printf("    \\  \\::/                         \\  \\:\\        \\  \\::/        /__/:/       \\  \\::/   \n");
    printf("     \\__\\/                           \\__\\/         \\__\\/         \\__\\/         \\__\\/    \n");
    printf("\n\n==================== C based Music Library System ======================\n\n");
}

// Menu display
void SignInMenu() {
    system("cls");
    fflush(stdout);
    ThemeArt();
    printf("\nLogin Options:\n-----------------------------------------\n\n");
    for (i = 0; i < 5; i++) {
        if (val == i) {
            printf("\t-->%s<--\n", SignIn_Options[i]);
        } else {
            printf("\t%s\n", SignIn_Options[i]);
        }
    }
}

// Handle keyboard input for menu
void KeyPress() {
    key = getch();
    if (key == 224) {
        key = getch();
        if (key == up) {
            val--;
            if (val < 0) val = 4;
        } else if (key == down) {
            val++;
            if (val >= 5) val = 0;
        }
    } else if (key == enterKey) {
        switch (val) {
            case 0:
                // SignIn(); // To implement later
                break;
            case 1:
                signUp();
                break;
            case 2:
                // ResetPass(); // To implement later
                break;
            case 3:
                // AdminContact(); // To implement later
                break;
            case 4:
                exit(0);
                break;
        }
    }
}

// Input number with range and digit limit, using getch()
int inputNumberInRange(const char *prompt, int min, int max, int digitLimit) {
    int num = 0, digits = 0;
    char ch;
    printf("%s", prompt);
    while (1) {
        ch = getch();
        if (ch >= '0' && ch <= '9') {
            if (digits < digitLimit) {
                putchar(ch);
                num = num * 10 + (ch - '0');
                digits++;
            }
        } else if (ch == 8) { // backspace
            if (digits > 0) {
                printf("\b \b");
                num /= 10;
                digits--;
            }
        } else if (ch == enterKey) {
            if (num >= min && num <= max) {
                printf("\n");
                return num;
            } else {
                printf("\nInvalid input. Please enter between %d and %d: ", min, max);
                num = 0;
                digits = 0;
            }
        }
    }
}

// Input string with validation
void inputStringWithValidation(char *prompt, char *input, int maxLen, int (*validationFunc)(char *)) {
    int valid = 0;
    char temp[256];
    while (!valid) {
        printf("%s", prompt);
        fgets(temp, sizeof(temp), stdin);

        // Remove newline if present
        int j;
        for (j = 0; temp[j] != '\0'; j++) {
            if (temp[j] == '\n') {
                temp[j] = '\0';
                break;
            }
        }

        if (strlen(temp) == 0) {
            printf("Input cannot be empty. Try again.\n");
            continue;
        }
        if (strlen(temp) >= maxLen) {
            printf("Input too long (max %d chars). Try again.\n", maxLen - 1);
            continue;
        }

        if (validationFunc != NULL && !validationFunc(temp)) {
            printf("Invalid input format. Try again.\n");
            continue;
        }

        strcpy(input, temp);
        valid = 1;
    }
}

// Validate name contains only letters
int validateName(char *str) {
    int k;
    for (k = 0; str[k]; k++) {
        if (!isalpha((unsigned char)str[k])) return 0;
    }
    return 1;
}

// Validate username contains only alphanumeric
int validateUsername(char *str) {
    int k;
    for (k = 0; str[k]; k++) {
        if (!isalnum((unsigned char)str[k])) return 0;
    }
    return 1;
}

// Basic email validation: exactly one '@' and at least one '.' after '@'
int validateEmail(char *email) {
    int atCount = 0, dotCount = 0;
    int atIndex = -1, dotIndex = -1;
    int k;

    if (email[0] == '@' || email[0] == '.') return 0;

    for (k = 0; email[k]; k++) {
        if (email[k] == '@') {
            atCount++;
            atIndex = k;
        }
        if (email[k] == '.') {
            dotCount++;
            dotIndex = k;
        }
    }

    if (atCount != 1 || dotCount == 0 || atIndex > dotIndex) return 0;
    return 1;
}

// SignUp function
void signUp() {
    FILE *p;
    struct credentials input;

    p = fopen("Credentials.txt", "a");
    if (p == NULL) {
        printf("\nSignUp failed! Cannot open file.\n");
        exit(0);
    }

    printf("\n************************Sign Up************************\n");

    inputStringWithValidation("Enter First Name: ", input.fname, sizeof(input.fname), validateName);
    inputStringWithValidation("Enter Last Name: ", input.lname, sizeof(input.lname), validateName);
    inputStringWithValidation("Enter Username: ", input.username, sizeof(input.username), validateUsername);

    printf("\n--------Date of Birth--------\n");
    input.d.year = inputNumberInRange("Enter year (1900-2025): ", 1900, 2025, 4);
    input.d.month = inputNumberInRange("Enter month (1-12): ", 1, 12, 2);
    input.d.day = inputNumberInRange("Enter day (1-31): ", 1, 31, 2);
	fflush(stdin);
    inputStringWithValidation("\nEnter Email: ", input.email, sizeof(input.email), validateEmail);

    printf("\nEnter Password: ");
    fgets(input.password, sizeof(input.password), stdin);

    // Remove newline character from password input
    int j;
    for (j = 0; input.password[j] != '\0'; j++) {
        if (input.password[j] == '\n') {
            input.password[j] = '\0';
            break;
        }
    }

    fprintf(p, "%s,%s,%s,%04d-%02d-%02d,%s,%s\n",
            input.fname, input.lname, input.username,
            input.d.year, input.d.month, input.d.day,
            input.email, input.password);

    fclose(p);

    printf("\nAccount has been created successfully. Please Sign in to access.\n");
    val = 0;
    printf("Press any key to continue...");
    getch();
}

int main() {
    while (1) {
        SignInMenu();
        KeyPress();
    }
    return 0;
}
