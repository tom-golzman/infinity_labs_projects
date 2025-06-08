priority_queue_t* PQCreate(pq_comparer_t comparer, void* param)
{
	/* allocate memory of pq */
	/* if failed - return NULL */
	/* assert */
	/* create dynamic vector */
	/* if failed - free the pq and return NULL */
}

void PQDestroy(priority_queue_t* pq)
{
	/* if pq is NULL - return */
	/* destroy the dynamic vector */
	/* free the pq */
}

int PQEnqueue(priority_queue_t* pq, void* data)
{
	/* assert */
	/* PushBack to the vector */
	/* handle failure */
	/* while */
		/* */
}

void PQDequeue(priority_queue_t* pq)
{
	/* assert */
	/* copy the last element of the vector in the beginning */
	/* PopBack to the vector */
	/* while now the biggest of top3 */
		/* swap with the largest child */
}

void* PQPeek(const priority_queue_t* pq)
{
	/* assert */
	/* return the element in index 1 */
}

size_t PQSize(const priority_queue_t* pq)
{
	/* assert */
	/* return VectorSize */
}

int PQIsEmpty(const priority_queue_t* pq)
{
	/* assert */
	/* return if the size is zero */
}
