/* create a global int array */

int main()
{
	/* create producers & consumers threads arrays */

	/* create a semaphore */
	
	/* for each index in producers array */
		/* alloacte data */
		/* handle failure */
		/* create a thread */		
		/* handle failure */
	
	/* for each index in consumers array */
		/* alloacte data */
		/* handle failure */
		/* create a thread */		
		/* handle failure */
	
	/* return SUCCESS */
}

static void* ProduceThreadFunc(void* arg)
{
	/* while 1 */
		/* call Producer() */
		
		/* wait for free space in the array */
		/* lock the mutex */
		
		/* push the value to the list */
		
		/* increment the used space semaphore by 1 with sem_post() */
		/* unlock the mutex */
	
	/* end while */
	
	/* return NULL */
}

static int Producer()
{
	/* return the value */
}

static void* ConsumeThreadFunc(void* arg)
{
	/* while 1 */
		/* wait for data to read in the array */
		/* lock the mutex */

		/* pop the value from the list */
		/* free the value */
		
		/* increment the free space semaphore by 1 with sem_post() */		
		
		/* unlock the mutex */
		/* call Consumer() */
		
	/* end while */
	
	/* return NULL */
}

static void Consumer()
{
	/* print the value */
}
