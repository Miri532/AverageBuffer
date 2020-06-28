/**********************************************
  Filename: average_buffer.h
  Purpose: Header file for average buffer
  Author: Miri Inbar
***********************************************/
#ifndef __AVERAGE_BUFFER_H__
#define __AVERAGE_BUFFER_H__

// func type to be called in case of above threshold average
typedef void (*above_threshold_cb_t)(int avg_value, int threshold);

typedef struct
{
	above_threshold_cb_t above_threshold_cb;
	int threshold; // the threshold above which we will call the call back function
	int capacity; // how many samples we can hold at most at the same time
	int* buffer; // array to hold the buffer
	int num_added; // number of samples added from start of run
	float curr_average; // the average of the samples that are currently in the buffer.
	float average_forever; // the average of all samples from the beginning of the run
} AverageBuffer;

// allocate and initialize average buffer struct, use freeAverageBuffer to free the memory
// returns pointer to the newly created buffer, or NULL if failed to allocate memory
AverageBuffer* allocAverageBuffer(int buffer_capacity, above_threshold_cb_t above_threshold_cb, int threshold);

// returns the average of all the samples that are currently in the buffer.
// returns -1 if ab is null
float getAverage(AverageBuffer* ab);

// returns the average of all samples from the beginning of the run
// returns -1 if ab is null
float getAverageForever(AverageBuffer* ab);

// returns the average of the newest size/4 samples in the buffer.
// if there are less than 4 samples in buffer - returns their average 
// returns -1 if ab is null
float getUpperQuarterAverage(AverageBuffer* ab);

// returns the average of the oldest size/4 samples in the buffer.
// if there are less than 4 samples in buffer - returns their average 
// returns -1 if ab is null
float getLowerQuarterAverage(AverageBuffer* ab);

// clears all the samples from buffer, also the average values
// the buffer returns to it's initial state
// if ab is null doesn't do any action
void clearAverageBuffer(AverageBuffer* ab);

// adds new sample - Every time a new sample 
// enters the buffer the oldest sample will be overwritten in the buffer (if the buffer is full).
// Upon adding a new sample to the buffer, a check will be made to see whether the newly - calculated average
// (not Average - foerver!) is above the threshold, and if it is,
// the callback function will be called
void addSample(AverageBuffer* ab, int sample);

// frees memory allocated by allocAverageBuffer
void freeAverageBuffer(AverageBuffer* ab);

#endif /* __AVERAGE_BUFFER_H__ */
