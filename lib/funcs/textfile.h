// textfile.h: interface for reading and writing text files
// www.lighthouse3d.com
//
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given
//////////////////////////////////////////////////////////////////////
#ifndef TEXTFILE_H
#define TEXTFILE_H

char *textFileRead(const char *fn);
int textFileWrite(const char *fn,const char *s);

class fileReadCount
{
public:
    static int statFileReadCount;
};

class AccessFileForTest
{
private:
    static bool isEanabled;
public:
    AccessFileForTest() {
        isEanabled = true;
    }
    ~AccessFileForTest() {
        isEanabled = false;
    }
    static bool IsEnabled() {
        return isEanabled;
    }

};
#endif
