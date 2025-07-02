int main()
{
	/* create producers & consumers threads arrays */

	/* create a linked list */
	/* handle failure */
		
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
		
		/* lock the mutex */
		/* push the value to the list */
		/* increment the semaphore by 1 with sem_post() */
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
		/* wait to new message */
			/* lock the mutex */

			/* pop the value from the list */
			/* free the value */
			
			/* unlock the mutex */
		/* end while */
		
		/* call Consumer() */
		
	/* end while */
	
	/* return NULL */
}

static void Consumer()
{
	/* print the value */
}
