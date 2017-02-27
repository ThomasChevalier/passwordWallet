#ifndef FILEHEADER_H
#define FILEHEADER_H

#pragma pack(1)

class FileHeader
{
public:
    FileHeader():
	fileSize(0), rsv(0), offsetImg(0)
    {
        reset();
    }

    void reset()
    {
        signature[0] = '*';
        signature[1] = '*';
        fileSize = 0;
        rsv = 0;
        offsetImg = 0;
    }
    char signature[2];      // Signature caracter
	int fileSize;           // Size of file
	int rsv;                // 4 bytes to 0
	int offsetImg;          // Where the image begin
};


#endif // FILEHEADER_H
