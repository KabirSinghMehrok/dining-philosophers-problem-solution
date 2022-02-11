// Kabir Singh Mehrok | 2020382
// Assignment 5 | Philosophers Problem

#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// DECLARATIONS

// philosopher number
const int phil_no = 5;
// fork number
const int fork_no = 5;
// fork number
const int bowl_no = 4;


// mutex
sem_t mutex; 

// creating an array of philosopher thread IDs
pthread_t *phil_thread_arr;
// creating an array of forks, showing their status (true, in use; false, not in use)
bool *fork_in_use_arr;
// creating an array of bowls, showing their status (true, in use; false, not in use)
bool *bowl_in_use_arr;

int msleep(unsigned int tms) {
  return usleep(tms * 1000);
}

bool hasError(int id)
{
	return id != 0;
}

// void* thread(void* arg) 
// { 
//     //wait 
//     sem_wait(&mutex); 
//     printf("\nEntered thread\n"); 
  
//     //critical section 
//     sleep(4); 
      
//     //signal 
//     printf("\n Exit thread\n"); 
//     sem_post(&mutex); 
// }

bool atleast_one_free() 
{
	for (int i=0; i<fork_no; i++)
	{
		if (!fork_in_use_arr[i]) 
		{
			// atleast one fork is free
			return true;
		}
	}
	return false;
}


void change_fork_status(int fork)
{
	if (fork_in_use_arr[fork])
	{
		fork_in_use_arr[fork] = false;	
	}
	else 
	{
		fork_in_use_arr[fork] = true;
	}
}

void change_bowl_status(int bowl)
{
	if (bowl_in_use_arr[bowl])
	{
		bowl_in_use_arr[bowl] = false;	
	}
	else 
	{
		bowl_in_use_arr[bowl] = true;
	}
}

int use_bowl()
{
	// if any bowl is available for use, then bring it in use
	// if no bowl is available for use, then return -1

	for (int i=0; i<bowl_no; i++)
	{
		if (!bowl_in_use_arr[i])
		{
			change_bowl_status(i);
			return i;
		}
	}

	// no bowl for use could be found
	return -1;
}

int get_right_fork_no(int phil)
{
	return (phil+1) % 5;
}

int get_left_fork_no(int phil)
{
	return phil;
}


bool request_right_fork(int phil)
{
	if (!fork_in_use_arr[get_right_fork_no(phil)])
	{
		int fork_to_give = get_right_fork_no(phil);
		change_fork_status(fork_to_give);

		// yaay you got the right fork!
		return true;
	}

	return false;
}


bool request_left_fork(int phil)
{
	if (atleast_one_free())
	{
		if (!fork_in_use_arr[get_left_fork_no(phil)])
		{
			int fork_to_give = get_left_fork_no(phil);
			change_fork_status(fork_to_give);
			return true;
		}
	}

	return false;
}

void print_fork_status()
{
	for (int i=0; i<fork_no; i++)
	{
		if (fork_in_use_arr[i]){
			printf("OC ");
		}
		else
		{
			printf("UN ");
		}
	}

	printf("\n");
}

void print_bowl_status()
{
	for (int i=0; i<bowl_no; i++)
	{
		if (bowl_in_use_arr[i]){
			printf("OC ");
		}
		else
		{
			printf("UN ");
		}
	}

	printf("\n");
}


void *dine(void* philosopher)
{
	// this function represents one routine, which will be executed in the form of threads
	// a while loop executes repeatedly until Ctrl + C is pressed (in which case the given process stops executing)

	while(true)
	{
		int phil = (int) philosopher;

		sem_post(&mutex);

		bool got_left = request_left_fork(phil);
		bool got_right = request_right_fork(phil);

		sem_post(&mutex);

		if (got_left && got_right)
		{
			// since he got the forks, he can now use the bowl
			int bowl_used = use_bowl();

			if (bowl_used != -1)
			{
				printf("Phil : %d is eating\n", phil);
			}
			else
			{
				printf("All bowls are being used\n");
			}

			// and now unoccupy the forks
			change_fork_status(get_left_fork_no(phil));
			change_fork_status(get_right_fork_no(phil));

			// and unoccupy the bowl
			change_bowl_status(bowl_used);
		}
		else if (got_left && !got_right)
		{
			printf("Fork is occupied { Phil : %d ; Fork %d }\n", phil, get_right_fork_no(phil));
			change_fork_status(get_left_fork_no(phil));
		}

		else if (!got_left && got_right)
		{
			printf("Fork is occupied { Phil : %d ; Fork %d }\n", phil, get_right_fork_no(phil));
			change_fork_status(get_right_fork_no(phil));
		}

		else
		{
			printf("All forks are occupied\n");
		}


		// short sleep
		msleep(100);

		print_fork_status();
		print_bowl_status();
		printf("\n");
	}
}


int main() 
{ 
	// initializing the semaphore
    sem_init(&mutex, 0, 1); 

    printf("-- CODE IS RUNNING --\n");
    printf("Status of fork occupation (UN - unoccupied fork; OC - occupied fork)\n\n");

    // malloc for phil
    phil_thread_arr = malloc(sizeof(pthread_t) * phil_no);

    //malloc for forks
    fork_in_use_arr = malloc(sizeof(bool) * fork_no);

    //malloc for bowls
    bowl_in_use_arr = malloc(sizeof(bool) * bowl_no);

    // initializing these subroutines
    for (int i=0; i<phil_no; i++)
    {
    	int return_pthread_create = pthread_create(&phil_thread_arr[i], NULL, &dine, (void *) i);

    	// creating a small gap here
    	sleep(2); 

    	// check if there was an error while creating the thread
    	if (hasError(return_pthread_create))
    	{
    		printf("Error while creating thread\n");		
    		exit(1);
    	}
    }

    // WRITE THE CODE FOR JOINING THE THREADS

    for (int i=0; i<phil_no; i++)
    {
    	int return_pthread_join = pthread_join(phil_thread_arr[i], NULL);

    	// check if there was an error while creating the thread
    	if (hasError(return_pthread_join))
    	{
    		printf("Error while joining thread\n");		
    		exit(1);
    	}
    }
    
    int return_sem_destroy = sem_destroy(&mutex); 

    if (hasError(return_sem_destroy))
    {
    	printf("Error while joining thread\n");		
    	exit(1);
    }

    // on ending, the function returns the value
    return 0; 
}