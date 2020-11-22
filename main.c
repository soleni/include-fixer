#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include "Windows.h"
#include "Winbase.h"

char ** files;
int filesCount = 0;

void ExploreDir (char * dir)
{
    char fDir[MAX_PATH];
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    strcpy(fDir, dir);
    strcat(fDir, "\\*");
    hFind = FindFirstFile(fDir, &ffd);

    if (INVALID_HANDLE_VALUE == hFind) 
        return;

    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if(ffd.cFileName[0] != '.')
            {
                char nextDir[MAX_PATH];
                sprintf(nextDir, "%s%c%s", dir, '\\', ffd.cFileName);
                
                ExploreDir(nextDir);
            }
        }
        else
        {
            ++filesCount;
            files = (char **) realloc (files, filesCount * sizeof(char **));
            files[filesCount - 1] = (char **) malloc (MAX_PATH);

            sprintf(files[filesCount - 1], "%s%c%s", dir, '\\', ffd.cFileName);
        }
    }
    while (FindNextFile(hFind, &ffd) != 0);

    free(fDir);
    CloseHandle(hFind);
}

int main (int argc, char ** argv)
{
    if(argc < 3)
        return 0;
    
    files = (char **) malloc (1 * sizeof(char **));

    char curDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, curDir);
    ExploreDir(curDir);

    for(int i = 0 ; i < filesCount ; ++i)
        printf("%s\n", files[i]);

    free(files);
    return 0;
}
