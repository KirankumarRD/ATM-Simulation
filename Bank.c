//Bank code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Customer {
    char name[50];
    int account_number;
    double balance;
    int passlast;
};

int generateAccountNumber() {
    return rand() % 90000000 + 10000000;
}

int generate4DigitRandomNumber() {
    return rand() % 9000 + 1000; // Generates a random number between 1000 and 9999
}

void createCustomer() {
    struct Customer customer;
    FILE *file;

    printf("Enter customer name: ");
    scanf("%s", customer.name);

    customer.account_number = generateAccountNumber();
    customer.balance = 0.0;
    customer.passlast=generate4DigitRandomNumber();

    file = fopen("bank_data.txt", "a");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    fprintf(file, "%s %d %.2lf %d\n", customer.name, customer.account_number, customer.balance, customer.passlast);

    fclose(file);

    printf("\nCustomer created successfully. Account number: %d\n", customer.account_number);
    printf("Password : %d \n",customer.passlast);
}

void displayCustomer(int account_number) {
    struct Customer customer;
    FILE *file;

    file = fopen("bank_data.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    int found = 0;
    while (fscanf(file, "%s %d %lf %d", customer.name, &customer.account_number, &customer.balance, &customer.passlast) != EOF) {
        if (customer.account_number == account_number) {
            printf("Customer Name: %s\n", customer.name);
            printf("Account Number: %d\n", customer.account_number);
            printf("Account Balance: %.2lf\n", customer.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Customer not found with account number: %d\n", account_number);
    }

    fclose(file);
}

void adminLogin() {
    int EnteredPIN1, choice;
    char EnteredID1[25];
    printf("\nGive login of an authorized employee of the bank\n");
    printf("Employee's ID\n");
    scanf("%s", EnteredID1);
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("Employee's PIN\n");
    scanf("%d", &EnteredPIN1);

    FILE *file;
    file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("File not found or unable to open the file.\n");
        return;
    }

    char ID1[25];
    int PIN1;
    fscanf(file, "ID1: %s\nPIN1: %d", ID1, &PIN1);
    fclose(file);

    if (strcmp(ID1, EnteredID1) == 0 && PIN1 ==EnteredPIN1 ) {
        printf("\nEmployee LOGIN success\n");
    } else {
        printf("Wrong ID or Wrong PIN of the employee !!!\n");
        return;
    }

    int cashLoadAmount = 0;
    int atmBalance = 0;

    while (1) {
        printf("\n1: Cash Load\n2: Check ATM Balance\n3: Logout\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Implement Cash Load functionality
                printf("Enter cash amount to load into ATM: ");
                scanf("%d", &cashLoadAmount);

                file = fopen("AtmBalance.txt", "r");
                if (file == NULL) {
                    printf("ATM balance data not found or unable to open the file.\n");
                    return;
                }

                fscanf(file, "ATM Balance : %d", &atmBalance);
                fclose(file);

                atmBalance += cashLoadAmount;

                file = fopen("AtmBalance.txt", "w");
                if (file == NULL) {
                    printf("Error updating ATM balance.\n");
                    return;
                }

                fprintf(file, "ATM Balance : %d", atmBalance);
                fclose(file);

                printf("Cash loaded successfully. Updated ATM balance: %d\n", atmBalance);
                break;

            case 2:
                // Check ATM Balance functionality
                file = fopen("AtmBalance.txt", "r");
                if (file == NULL) {
                    printf("ATM balance data not found or unable to open the file.\n");
                    return;
                }

                fscanf(file, "ATM Balance : %d", &atmBalance);
                fclose(file);

                if (atmBalance < 100000) {
                    printf("ATM balance is less than 100000.\nThe ATM facility is currently Unavailable !!!");
                } else {
                    printf("ATM balance is sufficient: %d\n", atmBalance);
                }
                break;

            case 3:
                return;
                
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void reviveBlocked_Account(int blocked_acc){
    return;
}
int main() {
    srand(time(NULL));

    int choice;
    int account_number, blocked_acc;

    while (1) {
        printf("\nBanking System Menu:\n");
        printf("1. Create Customer\n");
        printf("2. Display Customer Information\n");
        printf("3. For BANK ACCESS\n");
        printf("4. Revive Account\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createCustomer();
                break;
            case 2:
                printf("Enter account number: ");
                scanf("%d", &account_number);
                displayCustomer(account_number);
                break;
            case 3:
                adminLogin();
                break;
            case 4 : 
                printf("Enter your Blocked account Number\n");
                scanf("%d",&blocked_acc);
                reviveBlocked_Account(blocked_acc);
                printf("Your Account is Unblocked\n");
                break;

            case 5:
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
