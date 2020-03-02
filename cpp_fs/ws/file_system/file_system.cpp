#include <cstdlib>
#include <cstddef> /* size_t */
#include <unistd.h> /*  O_RDONLY | O_CREAT */
#include <iostream> /* cout */
#include <sys/types.h> /*  O_RDONLY | O_CREAT */
#include <fcntl.h>
#include <errno.h>
#include <linux/fs.h>
#include <linux/ext2_fs.h>

#define BLOCK_SIZE_t (1024)
#define BASE_OFFSET (1024)  /* location of the super-block in the first group */
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*(block_size))

/*
void CreateFileSystem()
{
    system("sudo modprobe brd");
    system("sudo mkfs.ext2 \\dev\\ram0");
    system("sudo mount -t ext2 \\dev\\ram0 -o sync");
}
*/

int main()
{
    int fd;
	struct ext2_super_block super;

	fd = open("/dev/ram0", O_RDONLY);    /* open floppy device */
    if (-1 == fd) 
    {
        std::cout << "errno = " << errno << std::endl;
    } 

	lseek(fd, 1024, SEEK_SET);                    /* position head above super-block */
	read(fd, &super, sizeof(super));  

    if (super.s_magic != EXT2_SUPER_MAGIC)
	        exit(1); /* bad file system */

      /* block size in bytes */
    unsigned int block_size = BLOCK_SIZE_t << super.s_log_block_size;
    
    /* calculate number of block groups on the disk */
    unsigned int group_count = 1 + (super.s_blocks_count-1) / 
                                    super.s_blocks_per_group;

    /* calculate size of the group descriptor list in bytes */
    unsigned int descr_list_size = group_count * 
                                   sizeof(struct ext2_group_desc);        
/**/
 //   struct ext2_super_block super;  /* the super block */
    struct ext2_group_desc group;   /* the group descriptor */
    unsigned char *bitmap;

    /* ... [read superblock and group descriptor] ... */

 /*   bitmap = (unsigned char *)malloc(block_size);    /* allocate memory for the bitmap */
/    lseek(fd, BLOCK_OFFSET(group.bg_block_bitmap), SEEK_SET);
    read(fd, bitmap, block_size);   /* read bitmap from disk */

    free(bitmap);        
}

/*
int main()
{
    int errno;
    char buf[256] = {'\0'};
    int fd = open("../../../../Downloads/fs_experiment/file.txt", O_RDWR);
    if (-1 == fd) 
    { 
        // print which type of error have in a code 
    } 

    // Read contents from file 
    read(fd, buf, 256);
    std::cout << buf << std::endl;
    close(fd);

    return 0;
}*/