// Calculate your half of a restaurant bill
// Data types, operations, type casting, return value

//#include <cs50.h>
#include <stdio.h>

float half(float bill, float tax, int tip);

int main(void)
{
    float bill_amount, tax_percent;
    int tip_percent;
    
    printf("Bill before tax and tip ($): ");
    scanf("%f", &bill_amount); //gets float from user

    printf("Sale Tax Percent (%%): ");
    scanf("%f", &tax_percent); //gets float from user

    printf("Tip Percent (%%): ");
    scanf("%d", &tip_percent); //gets int from user

    printf("You will owe $%.2f each!\n", half(bill_amount, tax_percent, tip_percent));
}

// TODO: Complete the function
float half(float bill, float tax, int tip)
{
    float total, taxAmount, tipAmount;
    taxAmount = bill * (tax * 0.01); //calculates the tax amount from the percentage
    tipAmount = (bill + taxAmount) * (tip * 0.01); //calculates the tip amount from the percentage
    total = bill + taxAmount + tipAmount; //calculates the overall total

    //calculates half of the total and returns the result
    return (total / 2);
}

/*
Bill before tax and tip ($): 50
Sale Tax Percent (%): 7
Tip Percent (%): 15
You will owe $30.76 each!
*/