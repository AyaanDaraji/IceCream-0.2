#include <stdio.h>  

// This struct holds info about one ice cream flavor on our menu
struct IceCream {
char flavor[20];   // name of the flavor
float price;       // price of one scoop of this flavor
};

// This holds everything about ONE ice cream the customer orders
struct Order {
    int flavorIndex;  // which flavor they picked (position in the menu)
    int scoops;       // how many scoops (1 to 3)
    int sprinkles;    // 1 = wants sprinkles, 0 = no sprinkles
    int cone;         // 1 = cone, 0 = cup
    float cost;       // final price for this ice cream
};

// Fixed prices for the extras 
const float extraScoopPrice = 1.00f;   // each scoop after the first one
const float sprinklesPrice  = 0.50f;   // sprinkles on top
const float cupPrice        = 0.25f;   // if they want a cup
const float conePrice       = 0.75f;   // if they want a cone

// Shows all the flavors on the menu with their prices
void showMenu(struct IceCream menu[], int menuSize) {
    for (int i = 0; i < menuSize; i++) {
        // i + 1 so the list starts from 1 instead of 0 (easier for people)
        printf("%d. %s - $%.2f\n", i + 1, menu[i].flavor, menu[i].price);
    }
}

// Throws away any junk the user typed so it doesn't mess up the next input
void clear_Input(void) {
    int c;
    // keep reading characters until we reach the end of the line
    while ((c = getchar()) != '\n' && c != EOF) { }
}

// Asks the user for a number and keeps asking until it is between min and max
int removeInvalidChoice(const char *message, int min, int max) {
    int value;

    while (1) {
        printf("%s", message);

        // scanf returns 1 if it read a number properly
        if (scanf("%d", &value) != 1) {
            // user typed something that is not a number (like letters)
            clear_Input();
            printf("Oh! we don't have that. choose from our menu.\n");
        } else if (value < min || value > max) {
            // it is a number but outside the allowed range
            printf("Choose from our menu (%d-%d).\n", min, max);
        } else {
            // all well - send the number back
            return value;
        }
    }
}

// Asks a yes or no question Returns 1 for yes and 0 for no
int getYesOrNo(const char *message) {
    char answer;

    while (1) {
        printf("%s", message);
        scanf(" %c", &answer);   // the space before %c skips extra spaces/newlines

        if (answer == 'y' || answer == 'Y') return 1;
        if (answer == 'n' || answer == 'N') return 0;

        // if it was not y or n, ask again
        printf("Please type y or n.\n");
    }
}

// Works on how much one ice cream costs
float calcCost(struct IceCream menu[], struct Order order) {
    // start with the price of the chosen flavor (this covers the first scoop)
    float cost = menu[order.flavorIndex].price;

    // adds extra money for every scoop after the first one
    cost += (order.scoops - 1) * extraScoopPrice;

    // adds sprinkles price if they asked for it
    if (order.sprinkles) cost += sprinklesPrice;

    // adds the price of the cone or the cup
    cost += order.cone ? conePrice : cupPrice;

    return cost;
}

// Draws a ice cream picture using symbols
void drawIceCream(struct IceCream menu[], struct Order order) {
    // show the flavor and scoop count (adds an "s" if more than 1 scoop)
    printf("   %s (%d scoop%s)\n", menu[order.flavorIndex].flavor,
           order.scoops, order.scoops > 1 ? "s" : "");

    // draw sprinkles on top if they wanted them
    if (order.sprinkles) {
        printf("    ******\n");
    }

    // top of the ice cream
    printf("     _____\n");

    // draw one scoop for each scoop ordered
    for (int s = 0; s < order.scoops; s++) {
        printf("    {_____}\n");
    }

    // draw the bottom part: a cone or a cup
    if (order.cone) {
        printf("     \\   /\n");
        printf("      \\ /\n");
        printf("       V\n");
    } else {
        printf("    |     |\n");
        printf("     \\___/\n");
    }

    printf("\n");   // empty line so pictures don't stick together
}

int main() {
    // our menu: flavor name and price
    struct IceCream menu[] = {
        {"Vanilla",    2.50},
        {"Chocolate",  3.99},
        {"Strawberry", 3.25},
    };
    // find how many flavors we have (total size divided by size of one item)
    int menuSize = sizeof(menu) / sizeof(menu[0]);

    int maxIceCreams = 10;   // a customer can order up to 10 ice creams
    int maxScoops = 3;       // and up to 3 scoops in each

    char name[30];           // customer's name
    int howMany;             // how many ice creams they want
    float total = 0;         // total bill, starts at zero
    float moneyGiven, change;

    printf("Welcome to our Ice Cream Shop!\n");

    // ask for the name (%29s just to makes sure it can't be larger than the array)
    printf("What is your name pal? ");
    scanf("%29s", name);

    printf("\nHi %s! What Ice cream will you like today?\n\n", name);

    // show the menu and ask how many ice creams they want
    showMenu(menu, menuSize);
    howMany = removeInvalidChoice("\nHow many ice creams do you want today? ", 1, maxIceCreams);

    // make space to store one order for each ice cream
    struct Order orders[howMany];

    // go through each ice cream and ask the customer what they want
    for (int i = 0; i < howMany; i++) {
        printf("\n Ice cream -%d \n", i + 1);

        // pick a flavor {minus 1 because arrays start from 0}
        orders[i].flavorIndex = removeInvalidChoice("Pick a flavor from our menu [1-3]: ", 1, menuSize) - 1;

        // pick the number of scoops
        orders[i].scoops = removeInvalidChoice("How many scoops? [1-3]: ", 1, maxScoops);

        // ask about sprinkles
        orders[i].sprinkles = getYesOrNo("What do you think of  sprinkles? (y / n): ");

        // ask cup or cone 
        printf("1. Cup  ($%.2f)\n", cupPrice);
        printf("2. Cone ($%.2f)\n", conePrice);
        orders[i].cone = (removeInvalidChoice("Cup or cone? (1-2): ", 1, 2) == 2);

    // calculate the price of this ice cream and add it to the total bill
        orders[i].cost = calcCost(menu, orders[i]);
        total += orders[i].cost;

        printf("Your ice cream costs: $%.2f\n", orders[i].cost);
    }

    // show the final bill
    printf("\nYour total is: $%.2f\n", total);

    // ask for payment
    printf("Enter your money: $");
    scanf("%f", &moneyGiven);

    // keep asking until the customer gives enough money
    while (moneyGiven < total) {
        printf("Less money, BRING MORE MONEY KIDD!!!\n");
        printf("Enter amount: $");
        scanf("%f", &moneyGiven);
    }

    // return back  extra money
    change = moneyGiven - total;
    printf("Your change: $%.2f\n", change);

    // say thanks and show every ice cream they ordered
    printf("\nHere are your %d ice creams %s, have a nice day!\n\n", howMany, name);
    for (int i = 0; i < howMany; i++) {
        drawIceCream(menu, orders[i]);
    }

    return 0;   
}
