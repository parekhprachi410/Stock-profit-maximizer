#include <stdio.h>
#include <string.h>

#define MAX_STOCKS 10
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MAX_BUDGET 10000 

int i;


struct Stock
{
    char name[5];
    float launchPrice;
    float marketValue;
    int timeLapse;
    char indicator[10];
    int owned; 
};

// indicator = share growing/falling/freeze
void calculateIndicator(struct Stock *s)
{
    if (s->marketValue > s->launchPrice)
        strcpy(s->indicator, "grow");
    else if (s->marketValue < s->launchPrice)
        strcpy(s->indicator, "fall");
    else
        strcpy(s->indicator, "hanging");
}


void displayStocks(struct Stock stocks[], int n)
{
    printf("\n%-5s %-12s %-12s %-12s %-10s %-6s %-12s\n", 
           "Name", "LaunchPrice", "MarketValue", "TimeLapse", "Indicator", "Owned", "Profit/Loss");
           
    for (i = 0; i < n; i++)
    {
        float profit_loss = (stocks[i].marketValue - stocks[i].launchPrice) * stocks[i].owned;  // Calculate Profit/Loss
        printf("%-5s %-12.2f %-12.2f %-12d %-10s %-6d %-12.2f\n", 
               stocks[i].name, stocks[i].launchPrice, stocks[i].marketValue, 
               stocks[i].timeLapse, stocks[i].indicator, stocks[i].owned, profit_loss);
    }
}


void buyStock(struct Stock stocks[], int n, float *balance)
{
    char name[5];
    int qty;
    printf("Enter stock name to buy: ");
    if (scanf("%s", name) != 1) return;
    printf("Enter quantity: ");
    if (scanf("%d", &qty) != 1) return;

    for ( i = 0; i < n; i++)
    {
        if (strcmp(stocks[i].name, name) == 0)
        {
            float cost = qty * stocks[i].marketValue; 
            if (*balance >= cost)
            {
                *balance -= cost;
                stocks[i].owned += qty;
                printf("Bought %d of %s for %.2f\n", qty, name, cost);
            }
            else
            {
                printf("Not enough balance! Cost: %.2f, Current Balance: %.2f\n", cost, *balance);
            }
            return;
        }
    }
    printf("Stock not found!\n");
}

void sellStock(struct Stock stocks[], int n, float *balance)
{
    char name[5];
    int qty;
    printf("Enter stock name to sell: ");
    if (scanf("%s", name) != 1) 
		return;
    printf("Enter quantity: ");
    if (scanf("%d", &qty) != 1) 
		return;

    for ( i = 0; i < n; i++)
    {
        if (strcmp(stocks[i].name, name) == 0)
        {
            if (stocks[i].owned >= qty)
            {
                float revenue = qty * stocks[i].marketValue;
                *balance += revenue;
                stocks[i].owned -= qty;
                printf("Sold %d of %s for %.2f\n", qty, name, revenue);
            }
            else
            {
                printf("Not enough owned quantity! Owned: %d\n", stocks[i].owned);
            }
            return;
        }
    }
    printf("Stock not found!\n");
}

int comparePortfolio(const struct Stock a, const struct Stock b) 
{
    float profitA = (a.marketValue - a.launchPrice) * a.owned;
    float profitB = (b.marketValue - b.launchPrice) * b.owned;
    
    if (profitA > profitB) 
		return 1;
    if (profitA < profitB) 
		return -1;
    return 0;
}

void sortPortfolioStocks(struct Stock stocks[], int n) 
{
    int j;
    for (i = 0; i < n - 1; i++) 
	{
        int max_idx = i;
        for (j = i + 1; j < n; j++) 
		{
            if (comparePortfolio(stocks[j], stocks[max_idx]) > 0) 
			{
                max_idx = j;
            }
        }
        struct Stock temp = stocks[max_idx];
        stocks[max_idx] = stocks[i];
        stocks[i] = temp;
    }
}

void showPortfolio(struct Stock stocks[], int n, float balance)
{
    struct Stock sorted_stocks[MAX_STOCKS];
    float net_gain_loss = 0.0f;
    for (i = 0; i < n; i++) 
	{
        sorted_stocks[i] = stocks[i];
        net_gain_loss += (stocks[i].marketValue - stocks[i].launchPrice) * stocks[i].owned;
    }

    sortPortfolioStocks(sorted_stocks, n); 
    
    printf("\n--- Portfolio (Sorted by Profit/Loss) ---\n");
    displayStocks(sorted_stocks, n); 
    
    printf("Current Balance: $%.2f\n", balance);
    printf("Net Earning:   $%.2f\n", net_gain_loss);
}


int compareStocks(struct Stock a, struct Stock b) 
{
    float profitA = a.marketValue - a.launchPrice;
    float profitB = b.marketValue - b.launchPrice;
    
    if (profitA > profitB) 
		return 1;
    if (profitA < profitB) 
		return -1;
    return 0;
}

void sortStocks(struct Stock stocks[], int n) {
    int j;
    for (i = 0; i < n - 1; i++) {
        int max_idx = i;
        for (j = i + 1; j < n; j++) {
            if (compareStocks(stocks[j], stocks[max_idx]) > 0) {
                max_idx = j;
            }
        }
        struct Stock temp = stocks[max_idx];
        stocks[max_idx] = stocks[i];
        stocks[i] = temp;
    }
}

// Greedy recommendation:-
void recommendGreedy(struct Stock stocks[], int n)
{
    float current_budget;
    printf("\n--- Greedy Recommendations (Ranked Trades) ---\n");
    printf("Enter your current available investment budget: $");
    
    if (scanf("%f", &current_budget) != 1 || current_budget < 0) {
        printf("Invalid budget entered. Using $0.00.\n");
        current_budget = 0.0f;
    }
    
    struct Stock sorted_stocks[MAX_STOCKS];
    for (i = 0; i < n; i++) {
        sorted_stocks[i] = stocks[i];
    }

    sortStocks(sorted_stocks, n); 

    printf("\nRanked Trades (Best to Worst):\n");

    int rank = 1;
    int items_listed = 0;
    
    for (i = 0; i < n; i++)
    {
        float profit = sorted_stocks[i].marketValue - sorted_stocks[i].launchPrice;
        
        if (profit > 0) 
		{
            float cost = sorted_stocks[i].marketValue;
            char affordability[20] = "";
            
            if (cost > current_budget) 
			{
                strcpy(affordability, "(Too Expensive)");
            } else 
			{
                sprintf(affordability, "(Affordable)");
            }

            printf("%d. BUY %s (Cost: $%.2f, Potential Profit: $%.2f) %s\n", 
                   rank++, sorted_stocks[i].name, cost, profit, affordability);
            items_listed = 1;
        } 
        else if (profit < 0 && sorted_stocks[i].owned > 0) 
		{
            printf("%d. SELL %s (Avoidable Loss: $%.2f, Owned: %d)\n", 
                   rank++, sorted_stocks[i].name, -profit, sorted_stocks[i].owned); 
            items_listed = 1;
        }
    }
    
    if (!items_listed) 
	{
        printf("No profitable purchases or loss-avoiding sales available.\n");
    }
}


// Dynamic Programming (knapsack):-
int knapsackUnbounded(struct Stock stocks[], int n, int budget, int selected_qty[])
{
    if (budget > MAX_BUDGET) budget = MAX_BUDGET;

    int dp[MAX_BUDGET + 1];
    int item_used_at_weight[MAX_BUDGET + 1];
    int w;

    for (w = 0; w <= budget; w++) 
	{
        dp[w] = 0;
        item_used_at_weight[w] = -1; 
    }
    
    for (i = 0; i < n; i++)
    {
        int cost = (int)stocks[i].marketValue; 
        int profit = (int)(stocks[i].marketValue - stocks[i].launchPrice); 
        
        if (profit <= 0 || cost == 0) 
			continue; 
        for (w = cost; w <= budget; w++)
        {
            if (dp[w - cost] + profit > dp[w]) 
			{
                dp[w] = dp[w - cost] + profit;
                item_used_at_weight[w] = i; 
            }
        }
    }

    w = budget;
    while (w > 0) 
	{
        int item_index = item_used_at_weight[w];
        if (item_index == -1) 
		{
            break;
        }
        
        selected_qty[item_index]++; 
        // Use integer marketValue for tracing back the budget/weight
        w -= (int)stocks[item_index].marketValue; 
    }

    return dp[budget];
}

void recommendDP(struct Stock stocks[], int n, float balance)
{
    int budget_int;
    
    printf("\n--- DP Optimization (Best Buys) ---\n");
    printf("Enter your investment capital (budget, max %d): ", MAX_BUDGET);
    
    if (scanf("%d", &budget_int) != 1 || budget_int <= 0) 
	{
        printf("Invalid budget entered. Must be a positive integer.\n");
        while (getchar() != '\n'); 
        return;
    }

    if (budget_int > MAX_BUDGET)
	{
        printf("Warning: Budget exceeds maximum DP calculation capacity (%d). Using %d.\n", MAX_BUDGET, MAX_BUDGET);
        budget_int = MAX_BUDGET;
    }

    int selected_qty[MAX_STOCKS] = {0}; 
    
    int maxProfit_int = knapsackUnbounded(stocks, n, budget_int, selected_qty);
    
    printf("\nMaximum achievable approximate profit with budget $%d = $%d\n", budget_int, maxProfit_int);
    printf("Recommended purchases:\n");
    
    float totalCost = 0.0f;
    int itemsFound = 0;
    for (i = 0; i < n; i++) 
	{
        if (selected_qty[i] > 0) 
		{
            float cost = selected_qty[i] * stocks[i].marketValue;
            float expected_profit = selected_qty[i] * (stocks[i].marketValue - stocks[i].launchPrice);
            
            printf(" -> BUY %d units of %s (Cost: $%.2f, Expected Profit: $%.2f)\n", 
                   selected_qty[i], stocks[i].name, cost, expected_profit);
            totalCost += cost;
            itemsFound = 1;
        }
    }
    
    if (!itemsFound) 
	{
        printf(" -> No profitable purchases found within the budget (or prices are too high/low to be included in integer DP).\n");
    } 
	else 
	{
        printf("\nTotal Investment Cost: $%.2f (This should be close to your budget.)\n", totalCost);
    }
}


int main()
{
    // Already provided stocks data
    struct Stock stocks[MAX_STOCKS] = 
	{
        {"A", 50.00, 50.00, 10, "", 0},
        {"B", 10.00, 500.00, 10, "", 0}, 
        {"C", 2.55, 70.00, 10, "", 0},
        {"D", 100.00, 1000.00, 10, "", 0}, 
        {"E", 5.00, 1.55, 10, "", 0}, 
        {"F", 120.00, 140.00, 10, "", 0},
        {"G", 80.00, 50.00, 10, "", 0}, 
        {"H", 25.00, 275.00, 10, "", 0},
        {"I", 108.00, 110.00, 10, "", 0}
    };

    int n = 9;
    float balance = 0.0;

    printf("\n--- Welcome to the Stock Market Profit Maximizer ---\n");
    printf("Please enter your initial starting cash balance: $");
    if (scanf("%f", &balance) != 1 || balance < 0) {
        printf("Invalid input. Starting with default balance of $1000.00.\n");
        balance = 1000.0;
    }
    
    for ( i = 0; i < n; i++)
        calculateIndicator(&stocks[i]);

    int choice;
    do
    {
        printf("\n--- Stock Market Profit Maximizer ---\n");
        printf("1. Show all stocks\n2. Buy stock\n3. Sell stock\n4. Show portfolio\n5. Greedy Recommendation\n6. DP Optimization\n7. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) 
		{
            printf("Invalid input. Please enter a number.\n");
            choice = 0; 
            while (getchar() != '\n'); 
        }

        switch (choice)
        {
            case 1: displayStocks(stocks, n); 
					break;
            case 2: buyStock(stocks, n, &balance); 
					break;
            case 3: sellStock(stocks, n, &balance); 
					break;
            case 4: showPortfolio(stocks, n, balance); 
					break;
            case 5: recommendGreedy(stocks, n); 
					break;
            case 6: recommendDP(stocks, n, balance); 
					break;
            case 7: printf("Exiting...\n"); 
					break;
            default: printf("Invalid choice!\n");
        }
    } 
	while (choice != 7);

    return 0;
}
