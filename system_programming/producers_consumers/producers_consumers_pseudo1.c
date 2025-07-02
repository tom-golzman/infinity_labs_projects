int main()
{
	/* create threads for producer and consumer */
	/* join the threads */

	/* return SUCCESS */
}

static void* ProduceThreadFunc(void* arg)
{	
	/* call Producer() */
	
	/* atomic function - while the value wasn't read */
		/* busy wait */
		
	/* write the shared value */
	
	/* atomic function - update that the value was written */
}

static int Producer()
{
	/* return the value */
}

static void* ConsumeThreadFunc(void* arg)
{
	/* atomic function - while the value wasn't written */
		/* busy wait */
		
	/* copy the shared value */
	
	/* atomic function - update that the value was read */
	
	/* call Consumer() */
}

static void Consumer()
{
	/* write the value */
}
