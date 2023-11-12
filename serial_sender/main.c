// Uses POSIX serial port functions to send and receive data from a Jrk G2.
// NOTE: The Jrk's input mode must be "Serial / I2C / USB".
// NOTE: The Jrk's serial mode must be set to "USB dual port" if you are
//   connecting to it directly via USB.
// NODE: The Jrk's serial mode must be set to "UART" if you are connecting to
//   it via is TX and RX lines.
// NOTE: You might need to change the 'const char * device' line below to
//   specify the correct serial port.
 
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <time.h>

 
// Opens the specified serial port, sets it up for binary communication,
// configures its read timeouts, and sets its baud rate.
// Returns a non-negative file descriptor on success, or -1 on failure.
int open_serial_port(const char * device, uint32_t baud_rate)
{
  int fd = open(device, O_RDWR | O_NOCTTY);
  if (fd == -1)
  {
    perror(device);
    return -1;
  }
 
  // Flush away any bytes previously read or written.
  int result = tcflush(fd, TCIOFLUSH);
  if (result)
  {
    perror("tcflush failed");  // just a warning, not a fatal error
  }
 
  // Get the current configuration of the serial port.
  struct termios options;
  result = tcgetattr(fd, &options);
  if (result)
  {
    perror("tcgetattr failed");
    close(fd);
    return -1;
  }
 
  // Turn off any options that might interfere with our ability to send and
  // receive raw binary bytes.
  options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
  options.c_oflag &= ~(ONLCR | OCRNL);
  options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
 
  // Set up timeouts: Calls to read() will return as soon as there is
  // at least one byte available or when 100 ms has passed.
  options.c_cc[VTIME] = 1;
  options.c_cc[VMIN] = 0;
 
  // This code only supports certain standard baud rates. Supporting
  // non-standard baud rates should be possible but takes more work.
  switch (baud_rate)
  {
  case 4800:   cfsetospeed(&options, B4800);   break;
  case 9600:   cfsetospeed(&options, B9600);   break;
  case 19200:  cfsetospeed(&options, B19200);  break;
  case 38400:  cfsetospeed(&options, B38400);  break;
  case 115200: cfsetospeed(&options, B115200); break;
  default:
    fprintf(stderr, "warning: baud rate %u is not supported, using 9600.\n",
      baud_rate);
    cfsetospeed(&options, B9600);
    break;
  }
  cfsetispeed(&options, cfgetospeed(&options));
 
  result = tcsetattr(fd, TCSANOW, &options);
  if (result)
  {
    perror("tcsetattr failed");
    close(fd);
    return -1;
  }
 
  return fd;
}
 
// Writes bytes to the serial port, returning 0 on success and -1 on failure.
int write_port(int fd, uint8_t * buffer, size_t size)
{
  ssize_t result = write(fd, buffer, size);
  if (result != (ssize_t)size)
  {
    perror("failed to write to port");
    return -1;
  }
  return 0;
}
 
// Reads bytes from the serial port.
// Returns after all the desired bytes have been read, or if there is a
// timeout or other error.
// Returns the number of bytes successfully read into the buffer, or -1 if
// there was an error reading.
ssize_t read_port(int fd, uint8_t * buffer, size_t size)
{
  size_t received = 0;
  while (received < size)
  {
    ssize_t r = read(fd, buffer + received, size - received);
    if (r < 0)
    {
      perror("failed to read from port");
      return -1;
    }
    if (r == 0)
    {
      // Timeout
      break;
    }
    received += r;
  }
  return received;
}
 
void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

int main()
{
  // Choose the serial port name.  If the Jrk is connected directly via USB,
  // you can run "jrk2cmd --cmd-port" to get the right name to use here.
  // Linux USB example:          "/dev/ttyACM0"  (see also: /dev/serial/by-id)
  // macOS USB example:          "/dev/cu.usbmodem001234562"
  // Cygwin example:             "/dev/ttyS7"
  const char * device = "/dev/cu.usbserial-2340";
 
  // Choose the baud rate (bits per second).  This does not matter if you are
  // connecting to the Jrk over USB.  If you are connecting via the TX and RX
  // lines, this should match the baud rate in the Jrk's serial settings.
  uint32_t baud_rate = 115200;
 
  int fd = open_serial_port(device, baud_rate);
  if (fd < 0) { return 1; }
 
  uint8_t command[12] = {0};
  FILE * fptr = fopen("log_5.bin", "r");
  fseek(fptr, 12 * 3000, SEEK_SET);
  while(fread(&command, sizeof(uint8_t), 12, fptr)) {
    write_port(fd, command, 12);
    printf("%02X", command[0]);
    printf("%02X", command[1]);
    printf("%02X", command[2]);
    printf("%02X", command[3]);
    for (int i = 4; i < 12; i++) {
      printf(" %02X", command[i]);
    }
    printf("\n");
    delay(10);
  }
 
  close(fd);
  return 0;
}