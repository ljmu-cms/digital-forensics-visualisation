#include <dirent.h> 
#include <iostream> 
#include <sys/stat.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#define is_regular 0100000

using std::cout;
using std::cin;
using std::endl;

void scan (const char* directory)
{
	DIR* pdir = opendir (directory); 
    
    dirent *pent = NULL;

    
 /*   if (pdir == NULL) 
    { 
        cout << "\nERROR! pdir could not be initialised correctly";
        return;
    } 
*/
    while (pent = readdir (pdir)) 
    {

		if (pent->d_name[0] == '.')
			continue;
		
        //cout << pent->d_name << " - " << pent->d_namlen << endl;
        char* fileName = (char*) malloc(strlen(directory) + strlen(pent->d_name) + 2);
        strcpy(fileName,directory);
        strcat(fileName, "/");
        strcat(fileName, pent->d_name);

        struct stat st;
        stat(fileName, &st);
        
		if (st.st_mode & is_regular)
		{
			cout << fileName << endl; //get the file
		}
		else
		{
			scan (fileName);
		}
        
    }
}




int main()
{
	const char* dir = "C:/";
	scan(dir);
	cout << "\n\n\n\n\n#####################END\n\n\n\n\n";
	return 0;
}





