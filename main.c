#define _CRT_SECURE_NO_WARNINGS
/*
Author: Muhammad Zulhusni Bin Zakaria
Course Name: Basic Programming
Course Code: SKJ1053
Group 6: Salary Calculator Assistant
*/
#define CURL_STATICLIB
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include <curl/curl.h>

// Struct definition of Employee with its members
typedef struct
{
    char empName[70];       // Employee Name
    char empId[10];         // Employee ID
    int workingDays;        // Numbers of working days
    int overtime;           // Overtime claim hours per month
    double rate_hours;      // Rate per hours
    double totalGSalary;    // Total Gross Salary per month
    double totalNettSalary; // Total Nett Salary per month
} Employee;

Employee emp; // Global variable

// function prototype declaration
void loading();
void displayExitMssg(char[]);
void addEntry();
int getNumOfWD();
void getRateHrs(int *);
void getOvertimeCHrs();
void calcTotalGS(Employee *);
void removeCharFromString(char *, char);

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

// Function definition to remove "" from amaun nisab string
void removeCharFromString(char *p, char c)
{
    char *pDest = p;

    while (*p)
    {
        if (*p != c)
            *pDest++ = *p;
        p++;
    }
    *pDest = '\0';
}

int main()
{
    CURL *curl;
    FILE *fp;                                             // Declaring file to write amaun nisab value received from server
    char buffer[100];                                     // Buffer to hold amaun nisab value from file in string
    char *pointer;                                        // Pointer use as reference to buffer in order to parse string into double
    double nisab = 0.00;                                  // Amaun nisab default value for zakat
    char *url = "https://www.zakat2u.com.my/nisab-value"; // URL for html file that contain amaun nisab value
    curl = curl_easy_init();                              // Starting curl session

    if (curl)
    {
        fp = fopen("amaun_nisab.txt", "w+"); // Opening file as read and write mode to read write amaun_nisab into a file
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // Callback function to writing received data from html
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);             // Write data received from response body which is in html into a file called amaun_nisab.txt

        curl_easy_perform(curl); // Perform http request by downloading html file from url

        /* always cleanup */
        curl_easy_cleanup(curl); // Closing connection from the server url
        fclose(fp);              // Closing file
    }

    FILE *fptr = fopen("amaun_nisab.txt", "r"); // Declare file and open as read mode
    if (fptr)
    {
        while (fscanf(fptr, "%s", &buffer) != EOF) // Scanning stream of input from file and assign it into buffer until end of file
            removeCharFromString(buffer, '"');     // Remove "" from amaun nisab string received from file amaun_nisab.txt
        fclose(fptr);                              // Close file
    }
    nisab = strtod(buffer, &pointer); // Parse string into double and initialize amaun nisab value for zakat

    char choice; // Menu choice
    char exitMessage[] = "Thank you for using our services.\nIt is our pleasure and privilege to serve you. We hope our service will exceed your expectations.\nWe appreciate the trust you have placed in us and look forward to serving you again.";

    loading(); // Loading screen function call
    do         // Loop input until user press right key input
    {
        system("cls");
        printf("\t=======================================\n");
        printf("\t=    WELCOME TO SALARY CALCULATOR     =\n");
        printf("\t=======================================\n");
        printf("\t=        HOW MAY I ASSIST YOU?        =\n");
        printf("\t=======================================\n");
        printf("\t    1 - ADD NEW ENTRY               \n");
        printf("\t    E - TO EXIT                        \n");
        printf("Press any key here...");
        choice = _getch(); // Pause terminal while waiting for a key to be pressed

        switch (tolower(choice)) // switch statement with choice convert to lowercase letter
        {
        case '1':
            addEntry(nisab); // Add new employee with calculate gross salary and nett salary
            break;
        case 'e':
            break;
        default:
            printf("Wrong input key!");
            Sleep(1000);
            break;
        }
    } while (choice != 'E' && choice != 'e');

    displayExitMssg(exitMessage); // Display exit message function call
}

// Function definition for loading screen
void loading()
{
    printf("Hello, thank for using our program! We can assist you by calculating total salary for each employee and print it to a file.");
    Sleep(1000);   // wait for 1 sec
    system("cls"); // Clear terminal/output screen
    printf("Please wait");

    for (int a = 0; a < 2; a++) // Loop for loading dots symbol
    {
        printf(".");
        Sleep(1000);
    }
    system("cls");
}

// Function definition for display exit program message
void displayExitMssg(char exit_msg[])
{
    system("cls");
    printf("%s", exit_msg); // Display exit message
    Sleep(2000);
}

// Function definiton to add new employee entry
void addEntry(double nisab)
{
    bool checkWDInput = false;      // To check number of working days input boolean
    bool checkRHrsInput = false;    // To check rate per hours input
    bool checkOvertimeCHrs = false; // To check overtime claim hours per month input
    int rateHrsInt = 0;             // Rate per hours in integer
    int countRHrsDigit = 0;         // Count number of digits entered by user for rate per hours to verify if its zero digit or not
    char hasOvertime;               // To check if employee has overtime claim hours per month or not (Y/N)
    int overtime = 0;
    int countOverDigit = 0; // Count overtime hours claim digits entered by user to verify if its zero digit or not
    double zakat = 0.00;    // Zakat per month
    double kwsp = 0.00;     // KSWP amount per month
    char checkForPrint;
    FILE *fileEMP = fopen("employee.txt", "a"); // File to store employee info with open as append mode only

    system("cls");
    printf("Please enter employee name: ");
    scanf(" %[^\n]s", &emp.empName);

    system("cls");
    printf("Please enter employee id: ");
    scanf("%s", &emp.empId);

    do // Loop user until user enter correct input which is integer between 0 - 31
    {
        system("cls");
        printf("Please enter number of working days (Ex: 1-31): ");
        emp.workingDays = getNumOfWD(); // Function call to get number of working days input from user
        if (emp.workingDays >= 1 && emp.workingDays <= 31)
        {
            checkWDInput = false;
        }
        else
        {
            printf("\nWrong input!"); // Display wrong input message
            Sleep(1000);
            checkWDInput = true;
        }
    } while (checkWDInput);

    do // Loop user until user enter correct input which is number in integer
    {

        system("cls");
        printf("Please enter rate per hours for the employee in ringgit (last 2 digit will become cent, Ex: 12345 -> 123.45): ");
        getRateHrs(&rateHrsInt);                        // Function call to get rate per hours input from user
        emp.rate_hours = (double)(rateHrsInt / 100.00); // Convert integer into currency format by parsing integer received from input into double with 2 decimal places

        while (rateHrsInt != 0) // Count total digits input by user
        {
            rateHrsInt = rateHrsInt / 10;
            countRHrsDigit++;
        }

        if (countRHrsDigit != 0)
        {
            checkRHrsInput = false; // Assign boolean false to exit input loop bcs user enter right input
        }
        else
        {
            printf("\nWrong input!"); // Display wrong input message
            Sleep(1000);
            checkRHrsInput = true; // Assign boolean true to continue input loop bcs user enter wrong input
        }

    } while (checkRHrsInput);

    do // Loop user until user pressed correct key input which is a character y/Y or n/N
    {
        system("cls");
        printf("Did the employee has overtime claim hours per month (Y/N)? ");

        hasOvertime = tolower(_getch()); // Convert the character pressed into lowercase letter
        if (hasOvertime == 'y' || hasOvertime == 'n')
        {
            break;
        }
        else
        {
            printf("wrong key input!");
            Sleep(1000);
            continue;
        }
    } while (1);

    do
    {
        system("cls");
        if (hasOvertime == 'y')
        {
            printf("Please enter overtime claim hours per month (in hours): ");
            getOvertimeCHrs(&overtime); // Function call to get input for overtime claim hours per month from user
            emp.overtime = overtime;

            while (overtime != 0) // Count total digits input by user
            {
                overtime = overtime / 10;
                countOverDigit++;
            }

            if (countOverDigit != 0)
            {
                checkOvertimeCHrs = false; // Assign boolean false to exit input loop bcs user enter right input
            }
            else
            {
                printf("\nWrong input!"); // Display wrong input message
                Sleep(1000);
                checkOvertimeCHrs = true; // Assign boolean true to continue input loop bcs user enter wrong input
            }
        }
        else if (hasOvertime == 'n')
        {
            emp.overtime = 0; // Assign 0 hour if the employee does not have overtime claim
            checkOvertimeCHrs = false;
        }

    } while (checkOvertimeCHrs);

    calcTotalGS(&emp); // Function call to calculate total gross salary for the employee
    system("cls");
    printf("\n=======================================================================================");
    printf("\n11%% KWSP rate");
    printf("\nCurrent Amount Nisab: RM%.2f", nisab);
    printf("\nZakat is applied if gross salary for a year is more than nisab, zakat is 2.5%% of the gross salary per month");
    printf("\n=======================================================================================");
    printf("\n\nEmployee name: %s", emp.empName);
    printf("\nEmployee ID: %s", emp.empId);
    printf("\nNumber of working days: %d", emp.workingDays);
    printf("\nOvertime claim hours per month (in hours): %d", emp.overtime);
    printf("\nRate per hours: RM%.2f", emp.rate_hours);
    printf("\nTotal Gross Salary Per Month: RM%.2f", emp.totalGSalary);

    if ((emp.totalGSalary * 12) > nisab) // If total gross salary for a year is more than amaun nisab, then employee need to pay zakat based on zakat bulanan
    {
        zakat = emp.totalGSalary * 0.025; // Zakat calculation process
        printf("\nZakat bulanan: RM%.2f  (Employee need to pay zakat)", zakat);
    }
    else // If total gross salary for a year is less than amaun nisab, then employee do not need to pay zakat
    {
        zakat = 0.00;
        printf("\nZakat bulanan: RM%2.f  (Employee does not need to pay zakat because total gross salary per year is less than amaun nisab)", zakat);
    }

    kwsp = emp.totalGSalary * 0.11;                        // Calculation process for kwsp (11%)
    emp.totalNettSalary = emp.totalGSalary - kwsp - zakat; // Calculation process for total nett salary of an employee
    printf("\nKWSP per month: RM%.2f", kwsp);
    printf("\nTotal Nett Salary Per Month: RM%.2f (Total Gross Salary - kwsp - zakat)", emp.totalNettSalary);

    do // Loop user until user pressed correct key input which is a character y/Y or n/N
    {
        printf("\n\nDo you want to print the result into a file (Y/N)? ");

        checkForPrint = tolower(_getch()); // Convert the character pressed into lowercase letter
        if (checkForPrint == 'y' || checkForPrint == 'n')
        {
            break;
        }
        else
        {
            printf("wrong key input!");
            Sleep(1000);
            continue;
        }
    } while (1);

    do
    {
        system("cls");
        if (checkForPrint == 'y')
        {
            fprintf(fileEMP, "Name: %s", emp.empName);
            fprintf(fileEMP, "\nID: %s", emp.empId);
            fprintf(fileEMP, "\nNumber of working days: %d", emp.workingDays);
            fprintf(fileEMP, "\nOvertime claim hours per month (in hours): %d", emp.overtime);
            fprintf(fileEMP, "\nRate per hours: RM%.2f", emp.rate_hours);
            fprintf(fileEMP, "\nTotal Gross Salary Per Month: RM%.2f", emp.totalGSalary);
            if ((emp.totalGSalary * 12) > nisab) // If total gross salary for a year is more than amaun nisab, then employee need to pay zakat based on zakat bulanan
            {
                fprintf(fileEMP, "\nZakat bulanan: RM%.2f  (Employee need to pay zakat)", zakat);
            }
            else // If total gross salary for a year is less than amaun nisab, then employee do not need to pay zakat
            {
                fprintf(fileEMP, "\nZakat bulanan: RM%2.f  (Employee does not need to pay zakat because total gross salary per year is less than amaun nisab)", zakat);
            }
            fprintf(fileEMP, "\nKWSP per month: RM%.2f", kwsp);
            fprintf(fileEMP, "\nTotal Nett Salary Per Month: RM%.2f\n\n", emp.totalNettSalary);
            fclose(fileEMP);
            break;
        }
        else if (checkForPrint == 'n')
        {
            break;
        }

    } while (1);

    printf("Printing result");

    for (int a = 0; a < 2; a++) // Loop for printing result dots symbol
    {
        printf(".");
        Sleep(1000);
    }

    system("cls");
    printf("Result has been printed into a file");
    Sleep(2000);
    system("cls");

    fflush(stdin); // Clear input buffer for next input
}

int getNumOfWD() // Function definition for getting number of working days input from user
{
    int n = 0, ch;
    do // Loop user input until user enter integer which is correct input
    {
        ch = _getch(); // get ASCII code value of input entered by user

        // Checking ASCII code from digit 0-9 to make sure user enter only integer
        if (ch >= 48 && ch <= 57)
        {
            printf("%c", ch);       // print a character input if its digit of 0-9
            n = n * 10 + (ch - 48); // Calculation process to make digit symbol
        }

        if (ch == 13) // if user press enter/return key (13 is ASCII code for enter), break the do while loop
            break;    // break statement to exit input loop
    } while (1);      // Repeat loop if 1 (true)
    return n;
}

void getRateHrs(int *n) // Function definition for getting rate per hours in ringgit from user
{
    int ch;
    do // Loop user input until user enter integer which is correct input
    {
        ch = _getch(); // get ASCII code value of input entered by user

        // Checking ASCII code from digit 0-9 to make sure user enter only integer
        if (ch >= 48 && ch <= 57)
        {
            printf("%c", ch);         // print a character input if its digit of 0-9
            *n = *n * 10 + (ch - 48); // Calculation process to make digit symbol
        }

        if (ch == 13) // if user press enter/return key (13 is ASCII code for enter), break the do while loop
            break;    // break statement
    } while (1);      // Repeat loop if 1 = true
}

void getOvertimeCHrs(int *n) // Function definition for getting overtime claim per month in hours from user
{
    int ch;
    do // Loop user input until user enter integer which is correct input
    {
        ch = _getch(); // get ASCII code value of input entered by user

        // Checking ASCII code from digit 0-9 to make sure user enter only integer
        if (ch >= 48 && ch <= 57)
        {
            printf("%c", ch);         // print a character input if its digit of 0-9
            *n = *n * 10 + (ch - 48); // Calculation process to make digit symbol
        }

        if (ch == 13) // if user press enter/return key (13 is ASCII code for enter), break the do while loop
            break;    // break statement
    } while (1);      // Repeat loop if 1 = true
}

void calcTotalGS(Employee *emp) // Function definition for calculate total gross salary for the employee
{
    emp->totalGSalary = (emp->workingDays * (emp->rate_hours * 8)) + (emp->overtime * (emp->rate_hours * 1.5)); // Calculation process for total gross salary of the employee
}
