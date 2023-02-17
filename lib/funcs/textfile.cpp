#include "textfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fileReadCount::statFileReadCount = 0;
bool AccessFileForTest::isEanabled = false;

char *textFileRead(const char *fn)
{

#ifdef TESTOWANIE_F
    if (!AccessFileForTest::IsEnabled())
        return nullptr;
#endif
    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL) {
        fp = fopen(fn,"rt");

        if (fp != NULL) {
            fileReadCount::statFileReadCount++;
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);

            if (count > 0) {
//				content = (char *)malloc(sizeof(char) * (count+1));
                content = new char[sizeof(char) * (count+1)];
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

int textFileWrite(const char *fn,const char *s)
{

    FILE *fp;
    int status = 0;

    if (fn != NULL) {
        fp = fopen(fn,"w");

        if (fp != NULL) {

            if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
                status = 1;
            fclose(fp);
        }
    }
    return(status);
}
