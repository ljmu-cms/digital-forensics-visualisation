#include <dirent.h> 
#include <iostream> 
#include <sys/stat.h>
#include <fstream>
#include "md5.h"
#include <stdio.h>
#include <time.h>
#include <sstream> //for stringstream


using std::cout;
using std::cin;
using std::endl;

int is_hidden(const char *name) // Linux only
{
  return (name[0] == '.' &&
         strcmp(name, ".") != 0 &&
         strcmp(name, "..") != 0);
}

std::string slurp(std::ifstream& in) { // file to binary string converter
    std::stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

int main () 
{
	
    const char* directory = "C:/LeapSDK";
    
	      
    DIR* pdir = opendir (directory); 
    
    dirent *pent = NULL;

    
    if (pdir == NULL) 
    { 
        cout << "\nERROR! pdir could not be initialised correctly";
        return 0;
    } 

    while (pent = readdir (pdir)) 
    {
        if (pent == NULL)
        {
            cout << "\nERROR! pent could not be initialised correctly";
            return 0;
        }
        
        cout << pent->d_name << " - " << pent->d_namlen << endl;
        
        char* fileName = (char*) malloc(strlen(directory) + strlen(pent->d_name) + 2);
        strcpy(fileName,directory);
        strcat(fileName, "/");
        strcat(fileName, pent->d_name);
       

       
		struct stat st; 
		
    	if (stat(fileName, &st) == 0)
        {
        
			cout << "size: " << fileName<< " modification date: " << ctime(&st.st_ctime) << " usr read permission: " << (st.st_mode & 00400 ) <<" is folder: " << (st.st_mode & S_IFDIR)  << " is regular: " << (st.st_mode & 0100000) <<endl;
        
        
	        // read the file into a char* in order to send it to md5 function
	    	std::string fdata = "";
			std::ifstream filePtr(fileName,std::ios::binary);
	    	std::string dummy = "";
	   		
	    	
	    	
	    	if(st.st_mode & 0100000) // if the file is a regular file
	    	{
	    		fdata = slurp(filePtr);
	    		const char* fileData = fdata.c_str();
	    		unsigned* d = md5(fileData, fdata.size());
	    		WBunion u;
	    		
	    		for (unsigned j = 0; j < 4; ++j)
	    		{
	    			u.w = d[j];
	    			for (unsigned k = 0; k < 4; ++k)
	    			{
	    				printf("%02x",u.b[k]);
	    			}
	    		}
	    		printf("\n");
	    		
	    	}
	
    	
        
        
        	//(st_mode & S_IRUSR) returns 0 if the file is non-readable
        	//(st_mode & S_IWUSR) returns 0 if the file is non-writable (readonly) 
        	//(st_mode & S_IXUSR) returns 0 if the file is non-executable
        	
        	/*
        	S_IRWXU     00700   mask for file owner permissions

           S_IRUSR     00400   owner has read permission
           S_IWUSR     00200   owner has write permission
           S_IXUSR     00100   owner has execute permission

           S_IRWXG     00070   mask for group permissions
           S_IRGRP     00040   group has read permission
           S_IWGRP     00020   group has write permission
           S_IXGRP     00010   group has execute permission

           S_IRWXO     00007   mask for permissions for others
                               (not in group)
           S_IROTH     00004   others have read permission
           S_IWOTH     00002   others have write permission
           S_IXOTH     00001   others have execute permission

           S_IFMT     0170000   bit mask for the file type bit fields
           S_IFSOCK   0140000   socket
           S_IFLNK    0120000   symbolic link
           S_IFREG    0100000   regular file
           S_IFBLK    0060000   block device
           S_IFDIR    0040000   directory
           S_IFCHR    0020000   character device
           S_IFIFO    0010000   FIFO
        	*/
   		}	
		else
        	cout << "size cannot be returned\n";

    
    
       free(fileName);  
       fileName = NULL;
    }

    
    closedir (pdir);
    pdir = NULL;
	pent = NULL;
	
    return 0;
}

