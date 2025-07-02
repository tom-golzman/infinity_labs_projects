int main()
{
	/* create producers & consumers threads arrays */

	/* create a linked list */
	/* handle failure */
		
	/* create a list mutex */
	
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
		/* unlock the mutex */
		
	/* return NULL */
}

static int Producer()
{
	/* return the value */
}

static void* ConsumeThreadFunc(void* arg)
{
	/* while 1 */
		/* while no new message */
			/* lock the mutex */

			/* if the list isn't empty */
				/* pop the value from the list */
				/* free the value */
				/* update the flag - there's a new message */
			/* end if */
			
			/* unlock the mutex */
		/* end while */
		
		/* call Consumer() */
		
		/* reset the flag - no new message */
		
	/* end while */
	
	/* return NULL */
}

static void Consumer()
{
	/* print the value */
}
