// DO NOT CHANGE THIS FILE!!!
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "average_buffer.h"

/* Create a buffer of 100 samples (sliding window) that will calculate
 'Average', 'Average-forever', 'Upper Quarter Average' and 'Lower Quarter Average'. (Note: maximum size of buffer can vary)
- Every time a new sample enters the buffer the oldest sample will be overwritten in the buffer (if the buffer is full).
- 'Average' is the average of the 100 samples that are currently
in the buffer.
- 'Average-forever' is the average of all samples from the beginning of the run
- 'Upper Quarter Average' is the average of the newest 25 samples (if buffer size is currently 100).
- 'Lower Quarter Average' is the average of the oldest 25 samples (if buffer size is currently 100).
  For example, if samples buffer size is 8 and the last samples are 
[10,40,30,44,20,50,35,55]
- Upon adding a new sample to the buffer, a check will be made to see whether the newly-calculated average (not Average-foerver!) is above a certain threshold, and if it is, a callback function will be called (in this example the threshold is 40 and the callback function is above_threshold_cb())
- Implement all AverageBuffer API functions mentioned in the following main() function - DO NOT CHANGE THIS FILE. IMPLEMENT ALL OF YOUR FUNCTIONS IN average_buffer.c FILE.
*/

#define THRESHOLD 40

void above_threshold_cb(int avg_value, int threshold)
{
    printf("The average[%d] is above the threshold[%d]!\n", avg_value, threshold);
}

int main()
{
    int i, randNum;

    AverageBuffer *ab = allocAverageBuffer(100, above_threshold_cb, THRESHOLD);

    srand(time(NULL));

    for (i = 0; i < 200; i++)
    {
        randNum = rand() % 100;
        addSample(ab, randNum);

        printf("i = %d\n", i);
        printf("randNum = %d\n", randNum);
        printf("getAverage = %lf\n", getAverage(ab));
        printf("getAverageForever = %lf\n", getAverageForever(ab));
        printf("getUpperQuarterAverage = %lf\n", getUpperQuarterAverage(ab));
        printf("getLowerQuarterAverage = %lf\n", getLowerQuarterAverage(ab));
    }

    clearAverageBuffer(ab);
    addSample(ab, 100);

    freeAverageBuffer(ab);

    return 0;
}





