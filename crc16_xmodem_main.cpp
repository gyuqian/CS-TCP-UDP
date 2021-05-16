#include <iostream>
#include <cstring>

#define CRCMODE CRC16_XMODEM 
using namespace std;

unsigned short CRC16_XMODEM(unsigned char *puchMsg, unsigned int usDataLen)
{
    unsigned short wCRCin = 0x0000;
    unsigned short wCPoly = 0x1021;
    unsigned char wChar = 0;

    while (usDataLen--)
    {
        wChar = *(puchMsg++);
        wCRCin ^= (wChar << 8);

        for(int i = 0; i < 8; i++)
        {
            if(wCRCin & 0x8000)
            {
                wCRCin = (wCRCin << 1) ^ wCPoly;
            }
            else
            {
                wCRCin = wCRCin << 1;
            }
        }
    }
    return (wCRCin) ;
}

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
	    unsigned char data[6];
	    //string input = "00 06 80 01 00 03";
	    data[0] = 00;
	    data[1] = 06;
	    data[2] = 16 * 8;
	    data[3] = 01;
	    data[4] = 00;
	    data[5] = 02;
	    unsigned int CRC16 = CRCMODE(data, 6);
	    printf("%d\n%X\n", CRC16, CRC16);
    }
    else
    {
        unsigned int CRC16 = CRCMODE((unsigned char*)argv[1], strlen(argv[1]));
        printf("%d\n%X\n", CRC16, CRC16);
    }
    return 0;
}