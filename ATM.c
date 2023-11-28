//ATM machine code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h> 


struct Customer {
    char name[50];
    int account_number;
    double balance;
    int passlast;
    int attempts;
};

struct transaction{
    int acc_num;
    double widraw_amt;
    double balance_amt;
    char* currenttime;
};

char* getCurrentTime() {
    time_t currentTime;
    struct tm *timeInfo;
    static char timeString[100]; // Static buffer for the time string

    time(&currentTime); // Get the current time

    timeInfo = localtime(&currentTime); // Convert the current time to a more human-readable format

    // Format the current time as a string
    strftime(timeString, sizeof(timeString), "%Y-%m-%d_%H:%M:%S", timeInfo);

    return timeString;
}

//Function to Enter transaction details
void store_transaction(int Account_NUM,double Trans_amt,double currentBalance){
    struct transaction t1;
    
    
    char *currtame=getCurrentTime();
    t1.acc_num=Account_NUM;
    t1.widraw_amt=Trans_amt;
    t1.balance_amt=currentBalance;
    FILE *tranfile;
    tranfile=fopen("transactions2.txt","a");

    if(tranfile==NULL){
        perror("Error Opening the File\n");
        exit(1);
    }

    fprintf(tranfile, "%d %.2lf %s %.2lf\n",t1.acc_num,t1.widraw_amt,currtame,t1.balance_amt);
    fclose(tranfile);

    printf("transaction successfull\n");

}

int transaction_details(int account_no){
    struct transaction t1;
    int account_No=account_no;
    FILE *filedet;

    filedet=fopen("transactions2.txt","r");

    if(filedet==NULL){
        perror("File is Empty\n");
    }
    while(fscanf(filedet, "%d %lf %s %lf",&t1.acc_num,&t1.widraw_amt,t1.currenttime,&t1.balance_amt)!=EOF){
            if(account_No==t1.acc_num){
                //printf("Account No : %d \n",t1.acc_num);
                printf("\nTransaction Amount : %.2lf \n",t1.widraw_amt);
                printf("Remaining Balance : %.2lf \n",t1.balance_amt);
                printf("Time of Transaction : %s \n",t1.currenttime);
                
            }
    }
    fclose(filedet);
    printf("These are your Transactions\n");
    return 1;

}


void updateBalance(int account_number, double new_balance) {
    FILE *file, *tempFile;
    struct Customer customer;

    file = fopen("bank_data2.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    tempFile = fopen("temp_bank_data2.txt", "w");
    if (tempFile == NULL) {
        perror("Error creating temporary file");
        fclose(file);
        exit(1);
    }

    while (fscanf(file, "%s %d %lf %d", customer.name, &customer.account_number, &customer.balance, &customer.passlast) != EOF) {
        if (customer.account_number == account_number) {
            customer.balance = new_balance;
        }
        fprintf(tempFile, "%s %d %.2lf %d\n", customer.name, customer.account_number, customer.balance, customer.passlast);
    }
    fclose(file);
    fclose(tempFile);

    remove("bank_data2.txt");
    rename("temp_bank_data2.txt", "bank_data2.txt");
}

void displayCustomer(int account_number) {
    FILE *file;
    struct Customer customer;

    file = fopen("bank_data2.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    int found = 0;
    while (fscanf(file, "%s %d %lf", customer.name, &customer.account_number, &customer.balance) != EOF) {
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

void updateATMBalance(int deposit_amount) {
    int atmBalance;
    FILE *file = fopen("AtmBalance.txt", "r");
    
    if (file == NULL) {
        printf("ATM balance data not found or unable to open the file.\n");
        exit(1);
    }

    fscanf(file, "ATM Balance : %d", &atmBalance);
    fclose(file);

    int newAtmBal = atmBalance + deposit_amount;

    file = fopen("AtmBalance.txt", "w");
    if (file == NULL) {
        printf("Error updating ATM balance.\n");
        exit(1);
    }

    fprintf(file, "ATM Balance : %d", newAtmBal);
    fclose(file);
}

void Block_update(int account_number,int passw0rd) {
    struct Customer customer;
    FILE *file, *tempFile;
    int zerozcc=0;

    // Open the bank_data.txt file for reading
    file = fopen("bank_data2.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Create a temporary file for writing
    tempFile = fopen("temum_data.txt", "w");
    if (tempFile == NULL) {
        perror("Error creating temporary file");
        fclose(file);
        exit(1);
    }

    // Copy data from the original file to the temporary file while updating the balance
    while (fscanf(file, "%s %d %lf %d", customer.name, &customer.account_number, &customer.balance, &customer.passlast) != EOF) {
        if (customer.passlast==passw0rd) {
            // Update the acc No
             fprintf(tempFile, "%s %d %.2lf %d\n", customer.name, zerozcc, customer.balance, customer.passlast);
        }
        else{
        fprintf(tempFile, "%s %d %.2lf %d\n", customer.name, customer.account_number, customer.balance, customer.passlast);
        }
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    // Remove the original file and rename the temporary file to the original file
    remove("bank_data2.txt");
    rename("temum_data.txt", "bank_data2.txt");
}

//Function to Block Account
void Block_account(int account_number)
{
    struct Customer customer;
    FILE *filebl;
    FILE *fileboc;
    filebl=fopen("bank_data2.txt","a+");
    int accNO_block=account_number;
    double zerobalance=0,trialpass;
     if (filebl == NULL) {
        perror("Error opening file");
        exit(1);
    }
    while (fscanf(filebl, "%s %d %lf %d", customer.name, &customer.account_number, &customer.balance, &customer.passlast) != EOF) {
        if (customer.account_number == account_number) {

            fileboc=fopen("Blocked_accs.txt","w");
            fprintf(fileboc, "%s %d %.2lf %d\n", customer.name, customer.account_number, customer.balance, customer.passlast);
            fclose(fileboc);
            
            printf("Your Account is  add in Blocked List\n");
            trialpass=customer.passlast;
        }
    }
   
    fclose(filebl);
     Block_update(account_number,trialpass);

}

// these two functions helps to keep track of the begin of new day 
int getStoredDay() {
    int storedDay = 0;
    FILE *file = fopen("stored_day.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &storedDay);
        fclose(file);
    }
    return storedDay;
}

void updateStoredDay(int day) {
    FILE *file = fopen("stored_day.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", day);
        fclose(file);
    }
}

bool CheckATMDailyLimit(int wt_amount) {
    static int temp = 0; // Static variable to keep track of total withdrawal amount
    
    time_t currentTime = time(NULL);  // Get the current time
    struct tm *localTime = localtime(&currentTime);
    
    // Check if it's a new day, and reset the total withdrawal amount if necessary
    if (localTime->tm_mday != getStoredDay()) {
        temp = 0;
        updateStoredDay(localTime->tm_mday);
    }
    
    wt_amount = abs(wt_amount); // Take the absolute value of withdrawal amount
    temp = temp + wt_amount;

    if (temp >= 50000) {
        temp = temp - wt_amount;
        return false;
    } else {
        return true;
    }
}

int main() {
    int temp;
    int account_number,accu_num;
    int logged_in = 0;
    int atmBalance;
    struct Customer customer;
    struct transaction t1;

     int AtmBal;
    FILE *fileb = fopen("AtmBalance.txt", "r");
    fscanf(fileb,"ATM Balance : %d",&AtmBal);
    fclose(fileb);
     if (AtmBal <= 100000) {
         printf("Insufficient Balance in ATM MACHINE !!!\n Please, try Later\n");
         
         return 0;
     }


    while (1) {
        if (!logged_in) {
            printf("\nWELCOME\nPLEASE INSERT YOUR CARD\n");
            
            int passgod;
            
            printf("Enter your Debit Card Number:\n");
            scanf("%d", &account_number);
            
             if (account_number == 0) {
                 printf("\nExiting the program.\n");
                 break;
             }
            // Check if the account number exists
            struct Customer customer;
            FILE *file = fopen("bank_data2.txt", "r");
            int found = 2;
            int correct_password;
            while (fscanf(file, "%s %d %lf %d", customer.name, &customer.account_number, &customer.balance, &customer.passlast) != EOF) {
                if (customer.account_number == account_number) {
                    printf("Account found on this number\n");
                    printf("Enter your 4 Digit Password\n");
                    scanf("%d",&passgod);

                    correct_password=customer.passlast;
                    if(customer.passlast==passgod){
                    found = 1;
                    break;
                    }
                    else{
                        printf("Wrong password entered. Try again...\n");
                        found=0;
                        break;
                    }
                }
                else{
                    accu_num=0;
                }
            }
            fclose(file);

            
             if (found==2) {
                 printf("\nThis Account does not Exist\n");
                 break;
             }

            if(found==0){
                printf("Attemt 2\n");
                scanf("%d",&passgod);
                if(passgod==correct_password)
                {
                    printf("Correct password \n");
                    found=1;
                }
                else{
                    printf("Invalid Password,Please try again\n");
                    found=0;
                }
            }
            if(found==0){
                printf("Attempt 3\n");
                {
                    scanf("%d",&passgod);
                    if(passgod==correct_password)
                    {
                        printf("Correct password \n");
                        found=1;  
                    }
                    else{
                        printf("Your Account has been blocked\n");
                        printf("Please contact your bank\n");
                        Block_account(account_number);


                        break;
                    }
                }
            }

            if (found) {
                printf("Login successful.\n");
                logged_in = 1;
            } else {
                printf("Invalid account number. Please try again.\n");
            }
        } else {
            printf("\nATM Menu:\n");
            printf("1. Display Customer Information\n");
            printf("2. Deposit\n");
            printf("3. Withdraw\n");
            printf("4. Check Balance\n");
            printf("5. Mini Statement\n");
            printf("6. Logout\n");
            printf("Enter your choice:");

            int choice;
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    displayCustomer(account_number);
                    break;
                case 2:
                    printf("\nEnter the deposit amount: ");
                    double deposit_amount;
                    scanf("%lf", &deposit_amount);
                    if (deposit_amount <= 0) {
                        printf("\nInvalid deposit amount. Please enter a positive value.\n");
                    } else {
                        struct Customer customer;
                        FILE *file = fopen("bank_data2.txt", "r");
                        int found = 0;
                        double new_balance = 0.0;
                        while (fscanf(file, "%s %d %lf", customer.name, &customer.account_number, &customer.balance) != EOF) {
                            if (customer.account_number == account_number) {
                                found = 1;
                                new_balance = customer.balance + deposit_amount;
                                break;
                            }
                        }
                        fclose(file);
                        if (found) {
                            updateBalance(account_number, new_balance);
                            updateATMBalance(deposit_amount);
                            // store_transaction(t1,account_number,deposit_amount);
                            store_transaction(account_number,deposit_amount,new_balance);
                            printf("\nDeposit successful. New balance: %.2lf\n", new_balance);
                        } else {
                            printf("\nAccount not found. Logout and try again.\n");
                        }
                    }
                    break;
                case 3:

                    if (atmBalance <= 50000) {
                        printf("\nInsufficient Balance in ATM MACHINE !!!\n Can't WITHDRAW, try Later\n");
                        break;
                    }

                    printf("\nEnter the Withdrawal amount: ");
                    int wt_amount;
                    scanf("%d", &wt_amount);
                    if (wt_amount <= 0) {
                        printf("\nInvalid withdraw amount. Please enter a positive value.\n");
                        break;
                    }
                    if( wt_amount >= 50000) {
                        printf("\ncan't withdraw more than 50000 at a time\n");
                        break;
                    }

                    bool b = CheckATMDailyLimit(wt_amount);
                    if(!b) {
                        printf("\nDaily ATM transaction limit has been exceeded\n");
                        printf("TRY LATER !!\n");
                        break;
                    }

                    wt_amount = -1 * wt_amount;
                    struct Customer customer;
                    FILE *file = fopen("bank_data2.txt", "r");
                    int found = 0;
                    double new_balance = 0.0;
                    while (fscanf(file, "%s %d %lf", customer.name, &customer.account_number, &customer.balance) != EOF) {
                        if (customer.account_number == account_number ) {
                            found = 1;
                            new_balance = customer.balance + wt_amount;
                            break;
                        }
                    }
                    fclose(file);

                    if (customer.balance < (-1)*wt_amount) {
                        printf("Insufficient balance for withdrawal.\n");
                        break;
                    }

                    if (found) {
                        updateBalance(account_number, new_balance);
                        updateATMBalance(wt_amount);
                        // store_transaction(t1,account_number,wt_amount);
                        store_transaction(account_number,wt_amount,new_balance);
                        printf("\nWithdrawal successful. New balance: %.2lf\n", new_balance);
                    } else {
                        printf("Account not found. Logout and try again.\n");
                    }
                    break;
                case 4:
                    displayCustomer(account_number);
                    break;
                case 5:
                    // Mini Statement functionality can be added here
                    temp = transaction_details(account_number);
                    printf("\nThese are your Transaction Details\n");
                    break;
                case 6:
                    printf("Logout successful.\n");
                    logged_in = 0;
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    }
return 0;
}