

typedef struct floppy_disk
{
        char *name;
        unsigned short number_of_fats;          // #16 size = 1
        unsigned short sectors_used_per_fat;    // #22 size = 2
        unsigned short sectors_per_cluster;     // #13 size = 1
        unsigned short root_entries;            // #17 size = 2
        unsigned short bytes_per_sector;        // #11 size = 2

} floppy;
void showfat(floppy *myfloppy);
void structure(floppy *myfloppy);
void print_file_info(unsigned char file_buff[]);
void traverse(floppy *myfloppy, int sector, int flag, char *current_directory);
void mount(floppy *myfloppy , char *name);
void help(void);


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
        printf("                number of FATs:   \t\t  %d\n", myfloppy->number_of_fats);
        printf("                number of sectors used by FAT:    %d\n", myfloppy->sectors_used_per_fat);
        printf("                number of sectors per cluster:    %d\n", myfloppy->sectors_per_cluster);
        printf("                number of ROOT entries:         %d\n", myfloppy->root_entries);
        printf("                number of bytes per sector:     %d\n", myfloppy->bytes_per_sector);
        printf("                ---Sector #---     ---Sector Types---\n");
        printf("                   0                             BOOT\n");
        printf("                %.2d -- %.2d             FAT1\n", myfloppy->sectors_per_cluster, myfloppy->sectors_used_per_fat);
        printf("                %.2d -- %.2d             FAT2\n", myfloppy->sectors_used_per_fat+1, myfloppy->sectors_used_per_fat*2);
        printf("                %.2d -- %.2d             ROOT DIRECTORY\n", (myfloppy->sectors_used_per_fat*2)+1, (myfloppy->sectors_used_per_fat*2)+14);
}


