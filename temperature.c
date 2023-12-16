#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main() {
    srand(time(NULL));

    int timeStep = 0;
    double trend = 0.0;

    while (1) {
        // Generate a sinusoidal wave for periodicity
        double periodicity = sin(2 * M_PI * timeStep / 50.0);

        // Generate a cosine wave for seasonality
        double seasonality = cos(2 * M_PI * timeStep / 100.0);

        // Introduce a random walk for trend
        trend += ((rand() % 10) - 5) / 10.0;

        // Add random noise
        double noise = ((rand() % 10) - 5) / 10.0;

        // Simulate generating an output value with periodicity, seasonality, trend, and noise
        int outputValue = 50 + 20 * periodicity + 15 * seasonality + trend + noise;

        // Write the output value to the standard output
        printf("%d\n", outputValue);
        fflush(stdout);

        // Introduce a delay
        sleep(1);

        // Increment time step
        timeStep++;
    }

    return 0;
}