#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

typedef struct floppy_disk
{
	char *name;
	unsigned short number_of_fats;		// #16 size = 1
	unsigned short sectors_used_per_fat;	// #22 size = 2
	unsigned short sectors_per_cluster;	// #13 size = 1
	unsigned short root_entries;		// #17 size = 2
	unsigned short bytes_per_sector;	// #11 size = 2

} floppy;
void showfat(floppy *myfloppy);
void structure(floppy *myfloppy);
void print_file_info(unsigned char file_buff[]);
void traverse(floppy *myfloppy, int sector, int flag, char *current_directory);
void mount(floppy *myfloppy , char *name);
void help(void);
/*
int main()
{
	floppy newfloppy;
	floppy *myfloppy = &newfloppy;
	mount(myfloppy, "floppy.img");
	//printf("%i %i %i %i %i\n",myfloppy->number_of_fats , myfloppy->sectors_used_per_fat, myfloppy->sectors_per_cluster, myfloppy->root_entries, myfloppy->bytes_per_sector);
	char current_directory[150] = "/";
	traverse(myfloppy, 19, 0, current_directory);
	
	showfat(myfloppy);	
}*/

//print functions
void help(void)
{
	printf("clear - clear the screen\n");
	printf("quit - exit the shell\n");	
	printf("fmount [floppyname] - mount local floppy disk\n");
	printf("fumount - unmount the current floppy disk\n");
	printf("structure - print the structure of the floppy disk\n");
	printf("traverse - print the contents of the floppy disk\n");
	printf("traverse -l - print the content of the floppy disk with additional information\n");

}


// prints the floppy disk structure
void structure(floppy *myfloppy)
{
	printf("		number of FATs:   \t\t  %d\n", myfloppy->number_of_fats);
	printf("		number of sectors used by FAT:    %d\n", myfloppy->sectors_used_per_fat);
	printf("		number of sectors per cluster:    %d\n", myfloppy->sectors_per_cluster);
	printf("		number of ROOT entries:         %d\n", myfloppy->root_entries);
	printf("		number of bytes per sector:     %d\n", myfloppy->bytes_per_sector);
	printf("		---Sector #---     ---Sector Types---\n");
	printf("		   0				 BOOT\n");
	printf("		%.2d -- %.2d		 FAT1\n", myfloppy->sectors_per_cluster, myfloppy->sectors_used_per_fat);
	printf("		%.2d -- %.2d		 FAT2\n", myfloppy->sectors_used_per_fat+1, myfloppy->sectors_used_per_fat*2);
	printf("		%.2d -- %.2d		 ROOT DIRECTORY\n", (myfloppy->sectors_used_per_fat*2)+1, (myfloppy->sectors_used_per_fat*2)+14);
}

// mounts the floppy disk
void mount(floppy *myfloppy , char *name)
{
	// Create pointer to name
	char *temp = malloc(sizeof(char) * (strlen(name) + 1));
	memcpy(temp, name, strlen(name));
	myfloppy->name = temp;
	// open the floppy disk
	int fd_disk = open(/*"floppy.img"*/name, O_RDWR);
	int i = 0;
	unsigned short buff;

	// number of fats is located at the #16 byte
	lseek(fd_disk, 16, SEEK_SET); read(fd_disk, &buff, 1);
	myfloppy->number_of_fats = buff;	// #16 size = 1

	// number of sectors used per fat is located at the #22 byte (2 bytes)
	lseek(fd_disk, 22, SEEK_SET); read(fd_disk, &buff, 2);
	myfloppy->sectors_used_per_fat = buff;	// #22 size = 2
	
	// number of sectors per cluster is located at the #13 byte
	lseek(fd_disk, 13, SEEK_SET); read(fd_disk, &buff, 1);
	myfloppy->sectors_per_cluster = buff;	// #13 size = 1
	
	//number of root entries is located at the #17 byte (2 bytes)
	lseek(fd_disk, 17, SEEK_SET); read(fd_disk, &buff, 2);
	myfloppy->root_entries = buff;		// #17 size = 2
	
	// number of bytes per sector is located at the #11 byte (2 bytes)
	lseek(fd_disk, 11, SEEK_SET); read(fd_disk, &buff, 2);

	myfloppy->bytes_per_sector = buff;	// #11 size = 2

}

//checks if the current buffer contains the end of the directory ( a string of sixteen 00) 
int end_of_directory(unsigned char *file_buff)
{
	int i;
	// if one of the first 16 bytes is non zero this isn't the end of the directory, return false
	for(i = 0 ; i  < 16 ; i++)
	{
		if(file_buff[i] != 0)	
			return 0;
	}
	// if all first 16 bytes are 0 this is the end of the directory
	return 1;

}

// prints a files extra information
void print_file_info(unsigned char file_buff[])
{
	char r_file, system_file, hidden_file, archive_file;
	r_file = system_file = hidden_file = archive_file = '-';
	// determine Read only status
	if((file_buff[11] & 0x01) == 0x01)
		r_file = 'R';
	// determine system file status
	if((file_buff[11] & 0x04) == 0x04)
		system_file = 'S';
	// determine hidden file status
	if((file_buff[11] & 0x02) == 0x02)
		hidden_file = 'H';
	// determine archive status
	if((file_buff[11] & 0x20) == 0x20)
		archive_file = 'A';
	// set date and time
	int year = 1980 + (file_buff[25] >> 1);
	int month = (file_buff[25] % 2) * 8 + (file_buff[24] >> 5);
	int day = (file_buff[24] % 32);
	int hour = (file_buff[23] >> 3);
	int minute = ((file_buff[23] & 7) << 3) + ((file_buff[22] >> 5) & 7);// mask 7 = 0x0000 0111
	int second = (file_buff[22] & 31);// mask 31 = 0x0001 1111
	// print info

	printf("-%c%c%c%c     %i/%i/%i %i:%i:%i", archive_file, hidden_file, r_file, system_file, month, day, year , hour, minute, second);
}

// traverses every directory of a floppy disk, if the -l flag is used, prints with addtional info
void traverse(floppy *myfloppy, int sector, int flag, char *current_directory)
{
	// create a buffer for the file entries (exactly 32 bytes)
	unsigned char file_buff[32];
	//fprintf(stderr, "made it |%s|", myfloppy->name);//stderr
	//open the disk
	int fd_disk = open(myfloppy->name, O_RDWR);
	
	// offset starts at the sector location
	int offset = myfloppy->bytes_per_sector * sector;
	 
	
 	int i = 0;
	// list the files and if "-l", list their information. 
	//If you hit a directory, recursively call self (traverse the directory)  
	while(i < myfloppy->root_entries)
	{
		lseek(fd_disk, offset + 32, SEEK_SET);
		//read the file entry into buffer
		read(fd_disk, file_buff, 32);
		// check to see if the file is unallocated (1st char = 0x00) 
		//or deleted (1st char = 0xe5)
		
		// Test to see if this is the end of the directory
		if(end_of_directory(file_buff))
			break;
			
		if(file_buff[0] == 0x00 || file_buff[0] == 0xe5)
		{
			offset = offset + 32;	
			continue;
		}

		
		// file name is from #0 to #7 , extension is #8 to #10.
		char filename[9]; char extension[3];
		memcpy(filename, file_buff, 8); 
		filename[8] = '\0';
		memcpy(extension, &file_buff[8], 3) ;
		memcpy(filename, file_buff, 8);

		int j = 0;
		//delete needless characters from name
		for(j = 0; j < 8 ; j++)
		{
			if(!(isalpha(filename[j]) | isdigit(filename[j])))
					filename[j] = '\0';
		}
		
		// Check if long file name if byte #11 = **** 1111  
		if(((file_buff[11] & 0x0f) == 0x0f) | (file_buff[26] == 0 && file_buff[27] == 0))
		{	//long file name or invalid file
		
		}
		// check file attributes to see if normal file() or directory 
		// this is in the bit #4 of byte #11, use 00010000 mask = 0x10 
		else if((file_buff[11] & 0x10) == 0x10 )
		{      // Is a directory, take sector location byte #26-27. Traverse.
		
			// if -l traversal, print the extra info
			if(flag == 1 && !((file_buff[11] & 0x0f) == 0x0f))
			{
				print_file_info(file_buff);
				printf("\t\t<DIR>\t\t%s%s\n", current_directory, filename);	
			}
			else
				printf("%.10s%.5s                                   <DIR>\n", current_directory, filename);	

			// find sector and traverse
			int sector2 = (file_buff[27] << 8) + file_buff[26];
			sector2 = sector2 + 31;
			if((sector2 != sector) && (sector2 > 31+19)  )
			{	
				strcat(current_directory, filename);
				strcat(current_directory,"/");
				traverse(myfloppy, sector2, flag , current_directory);
			}
		}
		else
		{	// Is a file
			// get size of the file
			int fsize; lseek(fd_disk, offset+28, SEEK_SET); read(fd_disk, fsize, 3);
			// print size and name
			if(flag == 1 && !((file_buff[11] & 0x0f) == 0x0f))
			{
				print_file_info(file_buff);
				printf("\t\t\t%.4i\t%s%s.%.3s\n",fsize, current_directory, filename, extension);
			}
			else
				printf("%s%s.%s\n", current_directory, filename, extension);

		}
		
	
		// move to the next file entry, so go to end of current file.
		i++;
		offset = offset + 32;
	}
}


void showfat(floppy *myfloppy)
{
	int fd_disk = open(myfloppy->name, O_RDWR);
	int i;
	unsigned short buff[myfloppy->bytes_per_sector];

	// number of fats is located at the #16 byte
	lseek(fd_disk, 1 * myfloppy->bytes_per_sector, SEEK_SET); read(fd_disk, &buff, myfloppy->bytes_per_sector);
	printf("         0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f");
	for(i = 0; i < myfloppy->bytes_per_sector; i++)
	{
		if(!(i % 16))
		{
			printf("\n%.4x0 ", i/16);
		}
		
		if(buff[i])	
			printf("%.4x ", buff[i]);
		else
			printf("FREE ");
	}
	printf("\n");

}





