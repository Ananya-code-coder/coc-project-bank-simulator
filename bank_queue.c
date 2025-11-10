#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TOTAL_MINUTES 480   // 8 hours = 480 minutes
#define MAX_CUSTOMERS 1000  // upper limit of customers

int main() {
    srand(time(0)); // random seed

    int minute;
    float lambda; // average arrivals per minute
    printf("Enter average number of customer arrivals per minute (lambda): ");
    scanf("%f", &lambda);

    // Queues and variables
    int arrivalTime[MAX_CUSTOMERS];
    int serviceStart[MAX_CUSTOMERS];
    int waitTime[MAX_CUSTOMERS];
    int serviceTime[MAX_CUSTOMERS];
    int totalCustomers = 0;
    int tellerFree[2] = {0, 0}; // 2 tellers, 0 means free at start

    // Simulation for 480 minutes
    for (minute = 0; minute < TOTAL_MINUTES; minute++) {
        // Simulate number of arrivals this minute
        int arrivals = (rand() % 100 < (lambda * 100)) ? 1 : 0; // approx Poisson logic
        for (int i = 0; i < arrivals; i++) {
            if (totalCustomers < MAX_CUSTOMERS) {
                arrivalTime[totalCustomers] = minute;
                serviceTime[totalCustomers] = 2 + rand() % 2; // 2–3 mins service
                totalCustomers++;
            }
        }

        // Check tellers
        for (int t = 0; t < 2; t++) {
            if (tellerFree[t] > 0) {
                tellerFree[t]--; // teller is busy
            } else {
                // find next waiting customer
                for (int j = 0; j < totalCustomers; j++) {
                    if (serviceStart[j] == 0 && arrivalTime[j] <= minute) {
                        serviceStart[j] = minute;
                        waitTime[j] = minute - arrivalTime[j];
                        tellerFree[t] = serviceTime[j];
                        break;
                    }
                }
            }
        }
    }

    // Calculate stats
    float sum = 0, mean = 0, variance = 0, stddev = 0;
    int maxWait = 0, served = 0;

    for (int i = 0; i < totalCustomers; i++) {
        if (serviceStart[i] > 0) {
            sum += waitTime[i];
            if (waitTime[i] > maxWait)
                maxWait = waitTime[i];
            served++;
        }
    }

    mean = (served > 0) ? (sum / served) : 0;

    for (int i = 0; i < served; i++) {
        variance += pow(waitTime[i] - mean, 2);
    }
    stddev = (served > 1) ? sqrt(variance / (served - 1)) : 0;

    printf("\n--- Bank Queue Simulation Report ---\n");
    printf("Total Customers Arrived: %d\n", totalCustomers);
    printf("Total Customers Served: %d\n", served);
    printf("Average Wait Time: %.2f minutes\n", mean);
    printf("Maximum Wait Time: %d minutes\n", maxWait);
    printf("Standard Deviation of Wait Time: %.2f\n", stddev);

    return 0;
}
