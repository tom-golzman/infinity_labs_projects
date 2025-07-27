/**
	Written By: Tom Golzman
	Date: 22/07/2025
	Reviewed By: 
**/

/************************************ Includes *************************************/
#define _POSIX_C_SOURCE 200112L
#include <assert.h>		/* assert() */
#include <pthread.h>	/* pthread_t, pthread_mutex_t, pthread_create(), pthread_join() */
#include <fcntl.h>		/* open(), O_RDONLY */
#include <sys/mman.h>	/* mmap(), PROT_READ, MAP_PRIVATE */
#include <sys/stat.h>	/* fstat(), struct stat */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

#include "sorts.h"		/* QuickSort() */
#include "shuffle_sort.h"

/************************************* Defines *************************************/
enum
{
	NUM_WORDS = 521670,
	WORDS_ARRAY_COPIES = 5,
	MAX_THREADS = 20
};

typedef struct
{
	char** m_section_start;
	char** m_section_end;
	char** m_merged_array;
	size_t* m_merged_arr_size;
	pthread_mutex_t* m_merge_mutex;
} thread_args_t;

/******************************** Static Functions *********************************/
int ShuffleSort(const char* file_path_, size_t num_threads_);
static void* ThreadFunc(void* arg_);
static char* LoadFile(const char* file_path_, size_t* file_size_out_);
static void FillWordsArray(char** words_, char* mapped_file_);
static void DuplicateArray(char** dest_, char** src_, size_t num_words_, size_t num_copies_);
static void ShuffleArray(char** arr_, size_t size_);
static int CompareWords(const void* word1_, const void* word2_);
static int RandomCompare(const void* word1_, const void* word2_);
static void MergeArrays(char** dest_, char** left_, char** right_, size_t left_size_, size_t right_size_);
static void Cleanup(char** words_buffer, char** big_array, char** merged_array);

/************************************ Functions ************************************/
int ShuffleSort(const char* file_path_, size_t num_threads_)
{
	size_t i = 0;
	size_t file_size = 0;
	size_t section_size = 0;
	size_t merged_arr_size = 0;
	
	char* mapped_file = NULL;
	char** words_buffer = NULL;
	char** big_array = NULL;
	char** merged_array = NULL;

	pthread_mutex_t merge_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_t threads[MAX_THREADS];
	thread_args_t thread_args[MAX_THREADS];

	/* assert */
	assert(NULL != file_path_);
	assert(num_threads_ <= MAX_THREADS);

	/* allocate memory for the arrays */
	words_buffer = (char**)calloc(NUM_WORDS, sizeof(char*));
	big_array = (char**)calloc(NUM_WORDS * WORDS_ARRAY_COPIES, sizeof(char*));
	merged_array = (char**)calloc(NUM_WORDS * WORDS_ARRAY_COPIES, sizeof(char*));
	RET_IF_BAD(NULL != words_buffer && NULL != big_array && NULL != merged_array, FAIL, "ShuffleSort(): calloc() FAILED!\n");
				
	/* load file into buffer and get file size */
	mapped_file = LoadFile(file_path_, &file_size);
	RET_IF_BAD(NULL != mapped_file, FAIL, "ShuffleSort(): LoadFile() FAILED!\n");
	
	/* fill words array from the mapped file */
	FillWordsArray(words_buffer, mapped_file);
	
	/* duplicate words array into a big array */
	DuplicateArray(big_array, words_buffer, NUM_WORDS, WORDS_ARRAY_COPIES);
	
	/* shuffle big array */
	ShuffleArray(big_array, NUM_WORDS * WORDS_ARRAY_COPIES);
	
	/* calculate section size */
	section_size = (NUM_WORDS * WORDS_ARRAY_COPIES) / num_threads_;
	
	/* for each number of threads */
	for (i = 0; i < num_threads_; ++i)
	{
		/* assign in arguments array in index i all the struct fields */
		thread_args[i].m_section_start = big_array + (i * section_size);
		thread_args[i].m_section_end = (big_array + (i * section_size)) + section_size;
		thread_args[i].m_merged_array = merged_array;
		thread_args[i].m_merged_arr_size = &merged_arr_size;
		thread_args[i].m_merge_mutex = &merge_mutex;
		
		/* create a thread */
		pthread_create(&threads[i], NULL, ThreadFunc, &thread_args[i]);
	}
	
	/* for each number of threads */
	for (i = 0; i < num_threads_; ++i)
	{
		/* join the thread */
		pthread_join(threads[i], NULL);
	}
	
	/* cleanup */
	Cleanup(words_buffer, big_array, merged_array);
	
	/* return SUCCESS */
	return SUCCESS;
}

static void* ThreadFunc(void* arg_)
{
	thread_args_t* args = (assert(NULL != arg_), (thread_args_t*)arg_);
	
	char** local_sorted_arr = NULL;

	int status = 0;
	size_t section_size = 0;
	size_t merged_arr_start = 0;

	/* assert */
	assert(NULL != args->m_section_start);
	assert(NULL != args->m_section_end);
	assert(NULL != args->m_merged_array);
	assert(NULL != args->m_merged_arr_size);

	/* allocate memory for local sorted array */
	local_sorted_arr = (char**)calloc(NUM_WORDS * WORDS_ARRAY_COPIES, sizeof(char*));
	ExitIfBad(NULL != local_sorted_arr, FAIL, "ThreadFunc(): calloc(local_sorted_arr) FAILED!\n"); 
	
	/* calculate section size */
	section_size = args->m_section_end - args->m_section_start;
		
	merged_arr_start = *(args->m_merged_arr_size);

	/* copy the section into local array */
	memcpy(local_sorted_arr, args->m_section_start, section_size * sizeof(char*));
	
	/* qsort() with CompareWords() the current section */
	QuickSort(local_sorted_arr, section_size, sizeof(char*), CompareWords);
	
	/* lock mutex */
	status = pthread_mutex_lock(args->m_merge_mutex);
	ExitIfBadSC(status, FAIL, "ThreadFunc(): mutex_lock() FAILED!\n");
	
	/* merge sorted section into merged_array */
	MergeArrays(args->m_merged_array + merged_arr_start, args->m_merged_array, local_sorted_arr, merged_arr_start, section_size);

	/* update merged_array size */
	*(args->m_merged_arr_size) += section_size;
	
	/* unlock mutex */
	status = pthread_mutex_unlock(args->m_merge_mutex);
	ExitIfBadSC(status, FAIL, "ThreadFunc(): mutex_unlock() FAILED!\n"); 
	
	/* free the local sorted array */
	free(local_sorted_arr);
	local_sorted_arr = NULL;
	
	/* return NULL */
	return NULL;
}

static int CompareWords(const void* word1_, const void* word2_)
{
	int cmp_result = 0;

	const char* word1 = NULL;
	const char* word2 = NULL;
	
	assert(NULL != word1_);
	assert(NULL != word2_);
	
	word1 = *(const char**)word1_;
	word2 = *(const char**)word2_;
	
	/* strcmp() between the words */
	cmp_result = strcmp(word1, word2);
	
	/* if the words are the same */
	if (0 == cmp_result)
	{
		/* return the smaller pointer */
		return (word1 < word2) ? -1 : 1;
	}
	
	return cmp_result;
}

static char* LoadFile(const char* file_path_, size_t* file_size_out_)
{
	int fd = -1;
	int status = -1;
	char* buffer = NULL;
	struct stat st = {0};
	
	/* assert */
	assert(NULL != file_path_);
	assert(NULL != file_size_out_);
	
	/* open the file & handle failure */
	fd = open(file_path_, O_RDONLY);
	RET_IF_BAD(-1 != fd, NULL, "LoadFile(): open() FAILED!\n");
	
	/* calculate file size */
	status = fstat(fd, &st);
	RET_IF_BAD(-1 != status, NULL, "LoadFile(): fstat() FAILED!\n");
	
	*file_size_out_ = st.st_size;
	
	/* mmap with the file descriptor */
	buffer = (char*)mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	RET_IF_BAD((void*)-1 != buffer, NULL, "LoadFile(): mmap() FAILED!\n");
	
	/* close the file */
	close(fd);
	
	/* return the buffer */
	return buffer;
}

static void FillWordsArray(char** words_, char* mapped_file_)
{
	char** words_curr = words_;
	char* mapped_file_curr = mapped_file_;

	/* assert */
	assert(NULL != words_);
	assert(NULL != mapped_file_);
	
	/* assign the first word */
	*words_curr = mapped_file_curr;
	++words_curr;
	
	/* while current char is not the end of the file */
	while (*mapped_file_curr != '\0')
	{
		/* if the current char is new line */
		if (*mapped_file_curr == '\n')
		{
			/* replace it with 0 */
			*mapped_file_curr = '\0';

			/* assign in words array a pointer to the next word */
			*words_curr = mapped_file_curr + 1;
			
			/* increment words runner */
			++words_curr;
		}
		
		/* increment mapped_file runner */
		++mapped_file_curr;
	}
}

static void DuplicateArray(char** dest_, char** src_, size_t num_words_, size_t num_copies_)
{
	size_t i = 0;
	
	/* assert */
	assert(NULL != dest_);
	assert(NULL != src_);
	
	/* for each num of copies */
	for (i = 0; i < num_copies_; ++i)
	{
		/* memcpy() number of words */
		memcpy(dest_ + (i * num_words_), src_, num_words_ * sizeof(char*));
	}
}

static void ShuffleArray(char** arr_, size_t size_)
{
	/* assert */
	assert(NULL != arr_);

	/* qsort() with RandomCompare() */
	QuickSort(arr_, size_, sizeof(char*), RandomCompare);
}

static int RandomCompare(const void* word1_, const void* word2_)
{
	/* return -1 or 0 or 1 randomly */
	return (rand() % 3) - 1;

	(void)word1_;
	(void)word2_;
}

static void MergeArrays(char** dest_, char** left_, char** right_, size_t left_size_, size_t right_size_)
{
	int cmp_result = 0;
	char** dest_curr = dest_;
	char** left_curr = left_;
	char** right_curr = right_;
	char** left_end = left_ + left_size_;
	char** right_end = right_ + right_size_;

	/* assert */
	assert(NULL != dest_);
	assert(NULL != left_);
	assert(NULL != right_);
	
	/* while current left is not end and current right is not end */
	while (left_curr < left_end && right_curr < right_end)
	{
		cmp_result = strcmp(*left_curr, *right_curr);
		
		/* if words are the same */
		if (0 == cmp_result)
		{
			/* copy the smaller pointer into dest */
			if (*left_curr < *right_curr)
			{
				*dest_curr = *left_curr;
				++left_curr;
			}
			
			else
			{
				*dest_curr = *right_curr;
				++right_curr;
			}
		}
		
		/* if left word is before the right word */
		else if (cmp_result < 0)
		{
			/* copy left word into dest */
			*dest_curr = *left_curr;

			/* increment left_curr */
			++left_curr;
		}
		
		/* else */
		{
			/* copy right word into dest */
			*dest_curr = *right_curr;

			/* increment right_curr */
			++right_curr;
		}
		
		/* increment dest_curr */
		++dest_curr;
	}

	/* copy the remaining words: */
	
	/* while current left is not end */
	while (left_curr < left_end)
	{
		/* copy left word into dest */
		*dest_curr = *left_curr;

		/* increment left_curr */
		++left_curr;
			
		/* increment dest_curr */
		++dest_curr;
	}
	
	/* while current right is not end */
	while (right_curr < right_end)
	{
		/* copy right word into dest */
		*dest_curr = *right_curr;

		/* increment right_curr */
		++right_curr;
		
		/* increment dest_curr */
		++dest_curr;
	}
}

static void Cleanup(char** words_buffer, char** big_array, char** merged_array)
{
	free(words_buffer);
	words_buffer = NULL;

	free(big_array);
	big_array = NULL;
	
	free(merged_array);
	merged_array = NULL;
}
