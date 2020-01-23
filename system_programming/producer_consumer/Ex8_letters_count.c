#include <sys/mman.h> /* mmap, munmap */
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> /* strtok */


hash_table_t *InsertToHashTable(hash_table_t *hash_table)
{
	int status = 0;
	char *token = NULL;
	char *str_temp = NULL;
	int fd = open("/usr/share/dict/american-english", O_RDONLY);
	size_t fsize = GetFileSizeIMP(fd);
	
	assert(hash_table);
	
    str_temp = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	
	token = strtok(str_temp, ",. !\n");
	while (NULL != token)
	{
		status |= HashInsert(hash_table, token);
		token = strtok(NULL, ",. !\n");
	}

	return hash_table;
}
