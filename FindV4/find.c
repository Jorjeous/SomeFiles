#include <limits.h>
#include <string.h>
#include <sys/stat.h>
 
void nextdir(char folder[255])
{
    DIR *dfd;
    struct dirent *dp;
    dfd = opendir(folder);
    printf("Открытие папки %s \n", folder);
    while ((dp = readdir(dfd)) != NULL)
        if (dp->d_type != 4) printf("%s\n", dp->d_name); else
            if ((dp->d_type == 4) && ((strcmp(dp->d_name, ".") != 0) && (strcmp(dp->d_name, "..") != 0))) {
                folder = strcat(folder, "/");
                nextdir(strcat(folder, dp->d_name));
            }
    closedir(dfd);
 
}
 
int main(int argc, char **argv)
{
    DIR *dfd;
    struct dirent *dp;
    char filename[256];
    if (argc < 2) strcpy(filename, ".");
    else strcpy(filename, argv[1]);
    printf("Корневой каталог %s\n\n", filename);
    nextdir(filename);
    return 0;
}
