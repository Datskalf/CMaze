/**
 * Created: 2023-10-26 by som38
 * Last modified: 2023-10-27 by som38
 * Version: 1.0
 */

#include <stdlib.h>
#include "common.h"
#include "rng.h"

#if DEBUG_LEVEL >= 4
#include <stdio.h>
#endif

/*
 * Creates a random integer ranging from 0 inclusive to maxValue exclusive.
 *
 * The function uses a do ... while loop to ensure an even distribution.
 * This loop could in theory loop forever, but this is a near 0 probability.
 *
 * Finally, the function returns the resulting number, scaled down to the requested range.
 */
int randInt(int maxValue) {
    int limit = RAND_MAX - (RAND_MAX % maxValue);
    int result;
    do {
        result = rand() % maxValue; // NOLINT(cert-msc30-c, cert-msc50-cpp)
        #if DEBUG_LEVEL >= 4
        printf("RNG result: %d", result);
        #endif
    } while (result >= limit);

    return result % maxValue;
}