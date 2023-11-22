// #include "log.h"

// #define NUM_BUFFERS 4
// #define BUFFER_SIZE 4096

// static char buffers[NUM_BUFFERS][BUFFER_SIZE];

// static char ** current_buffer = &buffers[0];
// static size_t current_buffer_index = 0;
// static size_t current_buffer_length = 0;

// void log_setup() {

// }

// void log_loop() {

// }

// void log_printf(const char * format, ...) {
//     char buffer[256];
//     va_list args;
//     va_start(args, format);
//     int length = vsnprintf(buffer, 255, format, args);

//     for (int i = 0; i < length; i++) {
//         if (current_buffer_length + length > BUFFER_SIZE) {
//             current_buffer_index = (current_buffer_index + 1) % NUM_BUFFERS;
//             current_buffer = &buffers[current_buffer_index];
//         }
//         *current_buffer[current_buffer_length++] = buffer[i];
//     }

//     va_end (args);
// }