#include "average_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// helper func - returns (num -1) mod (mod)
// num is always >= 0
static int decByOneMod(int num, int mod)
{
	if (num - 1 >= 0)
	{
		return (num - 1) % mod;
	}

	else
	{
		return mod - 1;
	}
}


AverageBuffer* allocAverageBuffer(int buffer_capacity, above_threshold_cb_t above_threshold_cb, int threshold)
{
	AverageBuffer* ab = malloc(sizeof(AverageBuffer));
	if (ab != NULL)
	{
		ab->buffer = malloc(buffer_capacity * sizeof(int));
		if (ab->buffer == NULL)
		{
			free(ab);
			ab = NULL;
			return NULL;
		}

		memset(ab->buffer, 0, buffer_capacity * sizeof(int));

		ab->above_threshold_cb = above_threshold_cb;
		ab->threshold = threshold;
		ab->capacity = buffer_capacity;
		ab->num_added = 0;
		ab->curr_average = 0;
		ab->average_forever = 0;		
	}

	return ab;
}

float getAverage(AverageBuffer* ab)
{
	if (ab != NULL)
	{
		return ab->curr_average;
	}

	return -1;
}

float getAverageForever(AverageBuffer* ab)
{
	if (ab != NULL)
	{
		return ab->average_forever;
	}

	return -1;
}

void clearAverageBuffer(AverageBuffer* ab)
{
	if (ab != NULL)
	{
		memset(ab->buffer, 0, ab->capacity * sizeof(int));
		ab->num_added = 0;
		ab->curr_average = 0;
		ab->average_forever = 0;
	}
}


void freeAverageBuffer(AverageBuffer* ab)
{
	if (ab != NULL)
	{	
		free(ab->buffer);
		ab->buffer = NULL;
		free(ab);
		ab = NULL;
	}
}

void addSample(AverageBuffer* ab, int sample)
{
	int add_to_this_idx = (ab->num_added) % (ab->capacity);
	// new forever average
	float new_aver = (ab->num_added * ab->average_forever + sample) / (ab->num_added + 1);
	// save this for average calculations
	int to_override = ab->buffer[add_to_this_idx];
	ab->buffer[add_to_this_idx] = sample;
	ab->num_added++;
	ab->average_forever = new_aver;
	
	// we have room to add without overriding
	// here forever average = current average 
	if (ab->num_added <= ab->capacity)
	{		
		ab->curr_average = new_aver;

		if (new_aver > ab->threshold)
		{
			ab->above_threshold_cb(new_aver, ab->threshold);
		}
	}

	// need to calculate current average
	else
	{		
		// remove old elemnt from average and add the new one
		float sum = ab->curr_average * ab->capacity - to_override;
		ab->curr_average = (sum + sample) / ab->capacity;

		if (ab->curr_average > ab->threshold)
		{
			ab->above_threshold_cb(ab->curr_average, ab->threshold);
		}
	}	
}

float getUpperQuarterAverage(AverageBuffer* ab)
{
	if (ab != NULL)
	{
		// start adding from this idx and go back
		int idx = (ab->num_added - 1) % (ab->capacity);
		float aver = 0;
		int sum = 0;
		int num_to_calc = 0; // how many samples to count

		// first cycle
		if (ab->num_added <= ab->capacity)
		{
			num_to_calc = ab->num_added / 4;
			// less than 4 samples
			if (num_to_calc == 0)
			{
				return ab->curr_average;
			}
		}

		else
		{
			num_to_calc = ab->capacity / 4;
		}

		for (int i = 0; i < num_to_calc; i++)
		{
			sum += ab->buffer[idx];
			int old_idx = idx;
			idx = decByOneMod(old_idx, ab->capacity);
		}

		return (float)sum/num_to_calc;
	}

	return -1;
}

float getLowerQuarterAverage(AverageBuffer* ab)
{
	if (ab != NULL)
	{
		// start adding from this idx and go forward
		int idx = 0;
		float aver = 0;
		int num_to_calc = 0; // how many samples to count
		int sum = 0;

		// first cycle
		if (ab->num_added <= ab->capacity)
		{
			num_to_calc = ab->num_added / 4;

			if (num_to_calc == 0)
			{
				return ab->curr_average;
			}
		}
		// buffer is full
		else
		{
			idx = (ab->num_added) % (ab->capacity);
			num_to_calc = ab->capacity / 4;
		}

		for (int i = 0; i < num_to_calc; i++)
		{
			sum += ab->buffer[idx];
			idx = (idx + 1) % ab->capacity;
		}

		return (float)sum/num_to_calc;
	}

	return -1;
}