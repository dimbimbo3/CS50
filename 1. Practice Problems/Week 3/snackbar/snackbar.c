// Practice using structs
// Practice writing a linear search function

/**
 * Beach Burger Shack has the following 10 items on their menu
 * Burger: $9.5
 * Vegan Burger: $11
 * Hot Dog: $5
 * Cheese Dog: $7
 * Fries: $5
 * Cheese Fries: $6
 * Cold Pressed Juice: $7
 * Cold Brew: $3
 * Water: $2
 * Soda: $2
*/

//#include <cs50.h>
#include <ctype.h> //toupper
#include <stdio.h>
#include <string.h> //s
#include <stdbool.h>

// Number of menu items
// Adjust this value (10) to number of items input below
#define NUM_ITEMS 10
#define LENGTH 19 //longest item name is 18 characters (+1 for null terminator)

// Menu items have item name and price
typedef struct
{
    char item[LENGTH];
    float price;
}
menu_item; //user defined structure

// Global array of menu items
menu_item menu[NUM_ITEMS];

// Add items to menu
void add_items(void);

// Calculate total cost
float get_cost(char item[]);

int main(void)
{
    add_items();

    printf("\nWelcome to Beach Burger Shack!\n");
    printf("Choose from the following menu to order. Press enter when done.\n\n");

    for (int i = 0; i < NUM_ITEMS; i++)
    {
        printf("%s: $%.2f\n", menu[i].item, menu[i].price);
    }
    printf("\n");

    float total = 0;
    while (true)
    {
        char item[LENGTH];
        printf("Enter a food item: ");
        if(fgets(item, LENGTH, stdin)){
            char* newlinePtr;

            if(newlinePtr = strchr(item, '\n'))
            {
                *newlinePtr = '\0';
            }
            
            if (strlen(item) == 0)
            {
                printf("\n");
                break;
            }

            total += get_cost(item);
        }
    }

    printf("Your total cost is: $%.2f\n", total);
}

/**
 * Populates the global menu array with items and their prices.
*/
void add_items(void)
{
    strcpy(menu[0].item, "Burger");
    menu[0].price = 9.5;
    strcpy(menu[1].item, "Vegan Burger");
    menu[1].price = 11;
    strcpy(menu[2].item, "Hot Dog");
    menu[2].price = 5;
    strcpy(menu[3].item, "Cheese Dog");
    menu[3].price = 7;
    strcpy(menu[4].item, "Fries");
    menu[4].price = 5;
    strcpy(menu[5].item, "Cheese Fries");
    menu[5].price = 6;
    strcpy(menu[6].item, "Cold Pressed Juice");
    menu[6].price = 7;
    strcpy(menu[7].item, "Cold Brew");
    menu[7].price = 3;
    strcpy(menu[8].item, "Water");
    menu[8].price = 2;
    strcpy(menu[9].item, "Soda");
    menu[9].price = 2;
    return;
}

/**
 * Linearly searches through the menu array to find the given item's cost.
*/
float get_cost(char item[])
{
    //loops through all menu items
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        //compares the inputted item's name with each item in the list (despite any casing differences)
        if(strcmp(strupr(item), strupr(menu[i].item)) == 0)
        {
            return menu[i].price; //end loop and return the item's price if found
        }
    }

    return 0; //in case of item not found do not alter bill
}

/*
Welcome to Beach Burger Shack!
Choose from the following menu to order. Press enter when done.

Burger: $9.50
Vegan Burger: $11.00
Hot Dog: $5.00
Cheese Dog: $7.00
Fries: $5.00
Cheese Fries: $6.00
Cold Pressed Juice: $7.00
Cold Brew: $3.00
Water: $2.00
Soda: $2.00

Enter a food item: vegan burger
Enter a food item: fries 
Enter a food item:

Your total cost is: $16.00
*/