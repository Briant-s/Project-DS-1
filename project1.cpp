#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define name_length 26

double user_Money_global = 0;
double user_Spent_global = 0;

enum class LogType{
    USER_BUY = 1,
    USER_SEARCH = 2,
    USER_VIEW_TRANSACTIONS = 3,
    USER_VIEW_SEARCH_HISTORY = 4,
    USER_VIEW_BUY_HISTORY = 5
};


// hash table for product
struct product{
    char name[name_length];
    double price;
    struct product* next;
};

product* phead[25] = {NULL};

int hash_index(char name[])
{
    int length = strlen(name);
    int index = length % 25; 
    return index;
}

void push_product(char name[], double price)
{
    product* newProduct = (product*)malloc(sizeof(product));
    newProduct->next = NULL;
    strcpy(newProduct->name, name);
    newProduct->price = price;
    int index = hash_index(name);
    if (phead[index] == NULL)
    {
        phead[index] = newProduct;
    }
    else
    {
        newProduct->next = phead[index];
        phead[index] = newProduct;
    }
}

void print_list()
{
    for (int i = 0; i < 25; i++)
    {
        product* curr = phead[i];
        while(curr != NULL)
        {
            printf("%s |", curr->name);
            curr = curr->next;
        }
        printf("\n");
    }
}

// stack for search history

struct shistory{
    char query[26];
    struct shistory* next;
    struct shistory* prev;
};

shistory* shead = NULL;
shistory* stail = NULL;

void add_searcHistory(char query[])
{
    shistory* newSearch = (shistory*)malloc(sizeof(shistory));
    newSearch->next = NULL;
    strcpy(newSearch->query, query);

    if (shead == NULL)
    {
        shead = stail = newSearch;
    }
    else 
    {
        stail->next = newSearch;
        newSearch->prev = stail;
        stail = newSearch;
    }
}


// stack for buy history

struct bhistory{
    char name[26];
    double price;
    struct bhistory* next;
    struct bhistory* prev;
};

bhistory* bhead = NULL;
bhistory* btail = NULL;

void add_buyHistory(char name[], double price)
{
    bhistory* newBuy = (bhistory*)malloc(sizeof(bhistory));
    newBuy->next = NULL;
    strcpy(newBuy->name, name);
    newBuy->price = price;

    if (bhead == NULL)
    {
        bhead = btail = newBuy;
    }
    else
    {
        btail->next = newBuy;
        newBuy->prev = btail;
        btail = newBuy;
    }
}

// functions for history

void show_SearchHistory()
{
    shistory* curr = stail;
    if (stail == NULL)
    {
        printf("Search History Empty!\n");
        return;
    }
    printf("Search History\n");
    for (int i = 0; i < 5 && curr != NULL; i++)
    {
            printf("%5d %25s\n", i+1, curr->query);
            curr = curr->prev;
    }
}


void show_BuyHistory()
{ 
    bhistory* curr = btail;
    if (btail == NULL)
    {
        printf("Buy History Empty!\n");
        return;
    }
    printf("Buy History\n");
    for (int i = 0; i < 5 && curr != NULL; i++)
    {   
            printf("%5d %25s %10.2lf\n", i+1, curr->name, curr->price);
            curr = curr->prev;
    }    
}


// things that interact with product_database

void read_Product()
{
    FILE *product_list = fopen("product_database.txt", "r");
    if (product_list == NULL)
    {
        printf("Error Opening File!\n");
        return;
    }
    char product_Name[name_length];
    double product_Price;
    while( fscanf(product_list, "%25[^#]#%lf\n", product_Name, &product_Price) != EOF)
    {
        push_product(product_Name, product_Price);
    }
    fclose(product_list);
}

void print_Product()
{
    FILE *product_list = fopen("product_database.txt", "r");
    if (product_list == NULL)
    {
        printf("Error Opening File!\n");
        return;
    }
    char product_Name[name_length];
    double product_Price;
    int number = 1;
    while( fscanf(product_list, "%25[^#]#%lf\n", product_Name, &product_Price) != EOF)
    {
        printf("%-10d %-25s $%-20.2lf\n", number, product_Name, product_Price);
        number++;
    }
    fclose(product_list);
}


// user functions

void user_Log_SB(LogType type, char product_Name[], double product_Price) // logging for search and buy
{
    FILE *logFile = fopen("userlogbook.txt", "a");
    // get current time
    time_t t = time(NULL);
    struct tm* ptr = localtime(&t);
    char curr_time[30];
    strftime(curr_time, sizeof(curr_time), "%Y-%m-%d %H:%M:%S", ptr);


    // getting the log type
    char logtype[30];
    switch (type)
    {
        case LogType::USER_BUY: strcpy(logtype, "BUY"); break;
        case LogType::USER_SEARCH: strcpy(logtype, "SEARCH"); break;
        default: strcpy(logtype, "ERROR!"); break;
    }


    // printing log
    fprintf(logFile, "%s - user_%s - %s - %.2lf\n", curr_time, logtype, product_Name, product_Price);
    fclose(logFile);
}

void user_Log_VH(LogType type) // logging for viewing histories
{
    FILE *logFile = fopen("userlogbook.txt", "a");
    // get current time
    time_t t = time(NULL);
    struct tm* ptr = localtime(&t);
    char curr_time[30];
    strftime(curr_time, sizeof(curr_time), "%Y-%m-%d %H:%M:%S", ptr);


    // getting the log type
    char logtype[30];
    switch (type)
    {
        case LogType::USER_VIEW_BUY_HISTORY: strcpy(logtype, "VIEW_BUY_HISTORY"); break;
        case LogType::USER_VIEW_TRANSACTIONS: strcpy(logtype, "VIEW_TRANSACTIONS"); break;
        case LogType::USER_VIEW_SEARCH_HISTORY: strcpy(logtype, "VIEW_SEARCH_HISTORY"); break;
        default: strcpy(logtype, "ERROR!"); break;
    }


    // printing log
    fprintf(logFile, "%s - user_%s\n", curr_time, logtype);
    fclose(logFile);
}

int is_BalanceEnough(double product_Price)
{
    if (user_Money_global < product_Price)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


void ask_buyProduct(char product_Name[], double product_Price)
{
    int user_Select = -1;
    printf("=============\n");
    printf("1. Buy\n");
    printf("2. Cancel\n");
    // input validation
    do
    {
        printf(">> ");
        scanf("%d", &user_Select);
    } while(user_Select < 1 || user_Select > 2);
    switch (user_Select) 
    {
        case 1: // if buy
            // checking balance
            if (is_BalanceEnough(product_Price) == 1)
            {
                add_buyHistory(product_Name, product_Price);
                user_Log_SB(LogType::USER_BUY, product_Name, product_Price);
                user_Money_global -= product_Price; // deducting user balance
                user_Spent_global += product_Price; // adding user spent
            }
            else // if not enough money
            {
                printf("Balance not enough\n");
                printf("Current balance = %.2lf\n", user_Money_global);
                printf("Press any key to continue..."); getchar(); getchar();
                return;
            } 
            // if enough
            printf("Product successfully purchased!\n");
            printf("Updated balance = %.2lf\n", user_Money_global);
            printf("Press any key to continue..."); getchar(); getchar();
            break;
        case 2:
            break;
    }
} 

void search_Product(char query[])
{
    int index = hash_index(query);
    product* curr = phead[index];
    while (curr != NULL)
    {
        if (strcmp(curr->name, query) == 0)
        {
            printf("Product Found!\n");
            printf("Product Name: %s | Product Price: $%-10.2lf\n", curr->name, curr->price);
            add_searcHistory(query);
            user_Log_SB(LogType::USER_SEARCH, curr->name, curr->price);
            ask_buyProduct(curr->name, curr->price);
            return;
        }
        curr = curr->next;
    }
    printf("Product not found!\n");
    printf("Press any key to continue..."); getchar();
}




// Menu Pages
void searchProduct_Menu()
{
    system("cls");
    char user_Search[27];
    int valid = -1;
    printf("============================================================================\n");
    printf("                          Shapee Product List\n");
    printf("============================================================================\n");
    printf("%-10s %-25s %-20s\n", "No", "Product Name", "Price");
    print_Product();
    printf("============================================================================\n");
    printf("Search Product (exit to leave) >> ");
    scanf("%26[^\n]", user_Search); getchar();
    if (strcmp(user_Search, "exit") == 0)
    {
        return;
    }
    search_Product(user_Search);
}

void searchHistory_Menu()
{
    system("cls");
    printf("============================================================================\n");
    printf("                          User Search History\n");
    printf("============================================================================\n");
    user_Log_VH(LogType::USER_VIEW_SEARCH_HISTORY);
    show_SearchHistory();
    printf("============================================================================\n");
    printf("Press any key to continue..."); getchar();
}

void buyHistory_Menu()
{
    system("cls");
    printf("============================================================================\n");
    printf("                          User Buy History\n");
    printf("============================================================================\n");
    user_Log_VH(LogType::USER_VIEW_BUY_HISTORY);
    show_BuyHistory();
    printf("============================================================================\n");
    printf("Press any key to continue..."); getchar();
}

void totalSpent_Menu()
{
    system("cls");
    user_Log_VH(LogType::USER_VIEW_TRANSACTIONS);
    printf("====================================\n\n");
    printf("Total User Spending = %10.2lf\n\n", user_Spent_global);
    printf("====================================\n");
    printf("Press any key to continue..."); getchar();
}

void mainMenu_Page()
{
    int menuSelect = -1;
    while (menuSelect == -1)
    {
        system("cls");
        printf("====================================\n");
        printf("           Shapee $%-10.2lf\n", user_Money_global);
        printf("====================================\n");
        printf("1. Search Product\n");
        printf("2. View Search History\n");
        printf("3. View Buy History\n");
        printf("4. View Total Spent\n");
        printf("5. Exit\n");
        printf("====================================\n");
        printf(">> ");
        scanf("%d", &menuSelect); getchar();
        switch (menuSelect) 
        {
            case 1:
                searchProduct_Menu();
                menuSelect = -1;
                break;
            case 2:
                searchHistory_Menu();
                menuSelect = -1;
                break;
            case 3:
                buyHistory_Menu();
                menuSelect = -1;
                break;
            case 4:
                totalSpent_Menu();
                menuSelect = -1;
                break;
            case 5:
                printf("Thanks for using the program!\n");
                printf("Created by Briant Sandriano | 2802453471\n");
                printf("Press any key to exit..."); getchar();
                return;
        }
    }    
}

void ask_startingMoney_Page()
{
    printf("Please input starting money: ");
    scanf("%lf", &user_Money_global);
}

int main(void)
{
    read_Product();
    //print_list();
    ask_startingMoney_Page();
    mainMenu_Page();
    //print_Product();
    //searchProduct_menu(); getchar();
}