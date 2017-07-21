#include <stdio.h>

#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

int set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                fprintf(stderr, "error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                fprintf(stderr, "error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}

void set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                fprintf(stderr, "error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                fprintf(stderr, "error %d setting term attributes", errno);
}

int main(int argc, char* argv[])
{
    if(argc != 3 || (argc == 2  && argv[1][0] == '-'
                                && argv[1][1] == '-'
                                && argv[1][2] == 'h'
                                && argv[1][3] == 'e'
                                && argv[1][4] == 'l'
                                && argv[1][5] == 'p'))
    {
        printf( "serial_to_file by Thomas Chevalier.\n"
                "(c) 2017 Thomas Chevalier - All rights reserved.\n"
                "thomasom [dot] chevalier [at] gmail [dot] com\n"
                "Usage : serial_to_file [serialPort] [fileName]\n\t"
                "serialPort = The name of the port to open.\n"
                "fileName = The file name to write the data received.\n");
        return 0;
    }

    char* portName = argv[1];
    char* fileName = argv[2];

    int fd = open (portName, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
            fprintf(stderr, "error %d opening %s: %s", errno, portName, strerror(errno));
            return 1;
    }

    set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking (fd, 0);                // set no blocking

    FILE *file = fopen(fileName, "wb");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file \"%s\"\n", fileName);
        return 1;
    }
    
    while(1)
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        int ret = select(fd+1, &fds, NULL, NULL, NULL);
        if(ret == 1)
        {
            char buf [64];
            int n = read (fd, buf, sizeof buf);  // read up to 64 characters
            if(n == -1)
            {
                fprintf(stderr, "read() : %s", strerror(errno));
            }
            fwrite(buf, n, 1, file);
            fflush(file);
        }
        else if(ret == -1)
        {
            fprintf(stderr, "select() : %s", strerror(errno));
        }
    }

    fclose(file);
}