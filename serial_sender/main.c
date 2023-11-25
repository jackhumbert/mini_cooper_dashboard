// Uses POSIX serial port functions to send and receive data from a Jrk G2.
// NOTE: The Jrk's input mode must be "Serial / I2C / USB".
// NOTE: The Jrk's serial mode must be set to "USB dual port" if you are
//   connecting to it directly via USB.
// NODE: The Jrk's serial mode must be set to "UART" if you are connecting to
//   it via is TX and RX lines.
// NOTE: You might need to change the 'const char * device' line below to
//   specify the correct serial port.
 
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include "ini.h"

struct timeval __millis_start;

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}


void init_millis() {
    gettimeofday(&__millis_start, NULL);
};

unsigned long int millis() {
    long mtime, seconds, useconds; 
    struct timeval end;
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - __millis_start.tv_sec;
    useconds = end.tv_usec - __millis_start.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    return mtime;
};
 
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

typedef struct {
    const char* monitor_port;
    uint32_t monitor_speed;
} configuration;

static int handler(void* user, const char* section, const char* name,
                   const char* value) {
    configuration* pconfig = (configuration*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("env:esp32-s3-devkitc-1-myboard", "monitor_speed")) {
        pconfig->monitor_speed = atoi(value);
    } else if (MATCH("env:esp32-s3-devkitc-1-myboard", "monitor_port")) {
        pconfig->monitor_port = strdup(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

void get_log(int fd, uint64_t id, uint8_t force) {
  uint8_t command[12] = {0x00, 0x00, 0x09, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  char log_name[256];
  char buf[9];
  sprintf(log_name, "logs/log_%08llu.crtd", id);
  // only get logs we haven't transferred yet
  if (access(log_name, F_OK) != 0 || force) {
    *(uint64_t*)(command + 4) = id;
    write_port(fd, command, 12);
    delay(1000);
    if (read_port(fd, (uint8_t*)buf, 9) != 9) {
      perror("Could not read log length");
      return;
    }
    uint64_t log_length = strtol(buf, NULL, 16);
    printf("%s: %llu bytes\n", log_name, log_length);
    if (log_length > 0) {
      uint8_t * file_buffer = malloc(log_length);
      if (read_port(fd, file_buffer, log_length) != log_length) { 
        perror("Could not read log");
        return;
      }
      FILE * file = fopen(log_name, "w");
      if (file == NULL) {
        perror("Could not open log file");
        return;
      }
      if (fwrite(file_buffer, 1, log_length, file) != log_length) {
        perror("Could not write to log file");
        return;
      }
      fclose(file);
    }
  }
}

void get_latest_logs(int fd) {
  uint8_t command[12] = {0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  write_port(fd, command, 12);
  delay(1000);
  char buf[9];
  ssize_t bytes = read_port(fd, (uint8_t*)buf, 9);
  if (bytes != 9) {
    printf("Could not read log count: %s, %lu bytes\n", buf, bytes);
    return;
  }
  uint64_t num_logs = strtol(buf, NULL, 16);
  printf("Current log: %llu\n", num_logs);
  // get_log(fd, num_logs - 1, 1);
  get_log(fd, num_logs, 1);
}

void get_logs(int fd) {
  uint8_t command[12] = {0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  write_port(fd, command, 12);
  delay(1000);
  char buf[9];
  ssize_t bytes = read_port(fd, (uint8_t*)buf, 9);
  if (bytes != 9) {
    printf("Could not read log count: %s, %lu bytes\n", buf, bytes);
    return;
  }
  uint64_t num_logs = strtol(buf, NULL, 16);
  printf("Number of logs: %llu\n", num_logs);
  for (uint64_t i = 0; i < num_logs - 1; i++) {
    get_log(fd, i, 0);
  }
}

void send_log(int fd, const char * log_name) {
  uint8_t command[30] = {0};
  FILE * file = fopen(log_name, "r");
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  if (file == NULL) {
    perror("Could not open log file");
    return;
  }

  int64_t initial = -1;

  while((read = getline(&line, &len, file)) != -1) {
    int cursor = 0;
    uint64_t seconds = strtol(&line[cursor], NULL, 10);
    while (line[cursor] != '.')
      cursor++;
    cursor++;
    uint64_t miliseconds = strtol(&line[cursor], NULL, 10) + seconds * 1000;
    if (initial == -1) {
      initial = miliseconds;
    }
    // printf("Waiting %lld miliseconds - now %lld\n", miliseconds, millis());
    while (line[cursor] != ' ')
      cursor++;
    if (line[cursor+1] == 'R' && line[cursor+2] == '1' && line[cursor+3] == '1') {
      cursor += 4;
      uint8_t index = 0;
      while(line[cursor] != 0xA) {
        if (line[cursor] == ' ')
          cursor++; // skip spaces
        char b_s[3] = { 0 };
        b_s[0] = line[cursor++];
        b_s[1] = line[cursor++];
        command[index++] = strtol(b_s, NULL, 16);
      } 
      if (miliseconds > (millis() + initial))
        delay(miliseconds - (millis() + initial));
      write_port(fd, command, index);
      printf("%08.3f R11 ", miliseconds / 1000.0);
      if (index == 12) {
        printf("%02X", command[0]);
        printf("%02X", command[1]);
        printf("%02X", command[2]);
        printf("%02X", command[3]);
        for (int i = 4; i < 12; i++) {
          printf(" %02X", command[i]);
        }
      } else {
        for (int i = 0; i < index; i++) {
          printf("%02X ", command[i]);
        }
      }
      printf("\n");
      // delay(10);
    } else {
      printf("%s", line);
    }
  }
  fclose(file);
}

int set_time(int fd) {
  uint8_t command[12] = {0x00, 0x00, 0x09, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  *((time_t*)(command + 4)) = time(NULL);
  write_port(fd, command, 12);
  return 0;
}

int main(int argc, char* argv[]) {
  configuration config;

  if (ini_parse("../platformio.ini", handler, &config) < 0) {
      perror("Couldn't load '../platformio.ini'\n");
      return 1;
  }
 
  int fd = open_serial_port(config.monitor_port, config.monitor_speed);
  if (fd < 0) { return 1; }

  init_millis();

  if (strcmp(argv[1], "send_log") == 0) {
    send_log(fd, argv[2]);
  } else if (strcmp(argv[1], "get_log") == 0) {
    get_log(fd, strtol(argv[2], NULL, 10), 1);
  } else if (strcmp(argv[1], "get_latest_logs") == 0) {
    get_latest_logs(fd);
  } else if (strcmp(argv[1], "get_logs") == 0) {
    get_logs(fd);
  } else if (strcmp(argv[1], "set_time") == 0) {
    set_time(fd);
  }

  close(fd);
  return 0;
}