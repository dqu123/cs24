#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "ffunc.h"


/* This function takes an array of single-precision floating point values,
 * and computes a sum in the order of the inputs.  Very simple.
 */
float fsum(FloatArray *floats) {
    float sum = 0;
    int i;

    for (i = 0; i < floats->count; i++)
        sum += floats->values[i];

    return sum;
}


/* This function produces high-accuracy floating-point sums
 * by accounting for a single correction term. */
float my_fsum(FloatArray *floats) {
    // We use c as a correction term.
    float sum = 0, c = 0;
    float nextsum;
    int i;

    // We use the approach described in the assignment pdf
    for (i = 0; i < floats->count; i++) {
        nextsum = sum + floats->values[i];
        // c should be 0 algebraically, but represents the amount 
        // of the new value truncated by rounding after summing.
        // It is negative if a postive value is truncated and
        // positive if a negative value is truncated.
        // We improve our accuracy by accounting for this
        // truncation c and accummulate it in the float c.
        // Of course, we could further improve accuracy by
        // accounting for similar effects from summing
        // these values of c, but these are minor. 
        c += (nextsum - sum) - floats->values[i];
        sum = nextsum;
    }

    // We subtract c because we need to compensate the loss.
    // In the form given in the pdf, c represents the net loss.
    return sum - c;
}


int main() {
    FloatArray floats;
    float sum1, sum2, sum3, my_sum;

    load_floats(stdin, &floats);
    printf("Loaded %d floats from stdin.\n", floats.count);

    /* Compute a sum, in the order of input. */
    sum1 = fsum(&floats);

    /* Use my_fsum() to compute a sum of the values.  Ideally, your
     * summation function won't be affected by the order of the input floats.
     */
    my_sum = my_fsum(&floats);

    /* Compute a sum, in order of increasing magnitude. */
    sort_incmag(&floats);
    sum2 = fsum(&floats);

    /* Compute a sum, in order of decreasing magnitude. */
    sort_decmag(&floats);
    sum3 = fsum(&floats);

    /* %e prints the floating-point value in full precision,
     * using scientific notation.
     */
    printf("Sum computed in order of input:  %e\n", sum1);
    printf("Sum computed in order of increasing magnitude:  %e\n", sum2);
    printf("Sum computed in order of decreasing magnitude:  %e\n", sum3);

    printf("My sum:  %e\n", my_sum);

    return 0;
}
