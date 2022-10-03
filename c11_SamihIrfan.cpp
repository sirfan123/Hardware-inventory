#pragma warning(disable: 4996)
#include<string>
#include<stdlib.h>
#include<time.h>
#include<iostream>
using namespace std;
#include <iostream>
using namespace std;

struct toolRec
{
    int recNum;
    char name[30];
    int quant;
    double cost;
};

int menu();
void inputRec(FILE *fPtr);
void deleteRec(FILE *fPtr);
void printAll(FILE *readPtr);

int main()
{
    int choice;
    FILE *recfPtr;
    FILE *writefPtr;
    struct toolRec inputTool = {0, "", 0, 0.0};

    while ((recfPtr = fopen("hardwares.dat", "rb+")) == NULL)
    {
        if ((writefPtr = fopen("hardwares.dat", "wb")) == NULL)
        {
            printf("File could not be opened.\n");
        }
        else
        {
            for (int i = 1; i <= 100; i++)
            {
                fwrite(&inputTool, sizeof(struct toolRec), 1, writefPtr);
            }

            fclose(writefPtr);
        }
    }

    if ((recfPtr = fopen("hardwares.dat", "rb+")) == NULL)
    {
        printf("File could not be opened.\n");
    }
    else
    {
        while ((choice = menu()) != 4)
        {
            switch (choice)
            {
            case 1:
                inputRec(recfPtr);
                break;
            case 2:
                deleteRec(recfPtr);
                break;
            case 3:
                printAll(recfPtr);
                break;
            default:
                printf("Incorrect Choice.\n");
                break;
            }
        }
    }

    fclose(recfPtr);
    system("pause");
    return 0;
}

int menu(void)
{
    int request;

    printf("\nEnter your request:\n"
           "1 - Input new tool or update an existing tool\n"
           "2 - Delete a tool\n"
           "3 - List all tools\n"
           "4 - Exit\n? ");
    scanf("%d", &request);

    return request;
}

void inputRec(FILE *fPtr)
{
    struct toolRec tool = {0, "", 0, 0.0};

    printf("Enter record number"
           " ( 1 - 100, 0 to return to main menu)\n? ");
    scanf("%d", &tool.recNum);

    if (tool.recNum < 0 || tool.recNum > 100)
        printf("Invalid record number.\n");
    else
    {

        printf("Enter the tool's name, "
               "quanitity and cost.\n? ");
        scanf("%s%d%lf", tool.name, &tool.quant, &tool.cost);

        fseek(fPtr, (tool.recNum - 1) * sizeof(struct toolRec), SEEK_SET);

        fwrite(&tool, sizeof(struct toolRec), 1, fPtr);
    }
}

void deleteRec(FILE *fPtr)
{
    struct toolRec tool;
    struct toolRec toolDel = {0, "", 0, 0.0};

    printf("Enter record number to delete"
           " (1 - 100, 0 to return to main menu)\n? ");
    scanf("%d", &tool.recNum);

    if (tool.recNum >= 1 && tool.recNum <= 100)
    {
        fseek(fPtr, (tool.recNum - 1) * sizeof(struct toolRec), SEEK_SET);
        fread(&tool, sizeof(struct toolRec), 1, fPtr);
        if (tool.recNum == 0)
        {
            printf("No value exists\n");
        }
        else if (tool.recNum < 0 || tool.recNum > 100)
        {
            printf("Invalid record number.\n");
        }
        else
        {
            fseek(fPtr, (tool.recNum - 1) * sizeof(struct toolRec), SEEK_SET);
            fwrite(&toolDel, sizeof(struct toolRec), 1, fPtr);
        }
    }
}

void printAll(FILE *readPtr)
{
    struct toolRec tool = {0, "", 0, 0.0};

    rewind(readPtr);
    printf("%-6s%-16s%-11s%10s\n", "Record #", "Tool Name",
           "Quantity", "Cost\n");
    while (1)
    {

        fread(&tool, sizeof(struct toolRec), 1, readPtr);
        if (feof(readPtr))
            break;

        if (tool.recNum != 0)
        {
            printf("%-10d%-20s%-10d%-10.2lf\n",
                   tool.recNum, tool.name,
                   tool.quant, tool.cost);
        }
    }
}