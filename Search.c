#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
 
#define MAX_DEPTH 3 
 
void FindInDir(char folder, char FindFileName, int depth)
{if(depth > MAX_DEPTH)
	return;
 else{
	
    DIR *dir;
    struct dirent *dp;
    dit = opendir(folder);
    printf("Открытие папки %s \n",folder);
        while ((dp=readdir(dir)) != NULL)
            if (dp->d_type != 4)
						{
            	printf("%s\n",dp->d_name);
							
						}
            else
                if ((dp->d_type==4) && ((strcmp(dp->d_name,".") != 0) && (strcmp(dp->d_name,"..") != 0))){
                    folder=strcat(folder,"/");          
                    nextdir(strcat(folder,dp->d_name));
                }
    closedir(dir);
 }
}
 
int main(int argc, char **argv)
{
    DIR *dir;
    struct dirent *dp;
    char filename[256];
    if ( argc < 2 ) strcpy(filename, ".");
            else strcpy(filename, argv[1]);
    printf("Корневой каталог %s\n\n", filename);
    FindInDir(filename, );
    return 0;
}
