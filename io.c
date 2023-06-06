#include "io.h"

#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct buffer {
    int fd;
    int offset;
    int num_remaining;
    uint8_t a[BUFFER_SIZE];
};

Buffer *read_open(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return NULL; // Unable to open the file
    }

    Buffer *buffer = malloc(sizeof(Buffer));
    if (buffer == NULL) {
        close(fd); // Cleanup if malloc fails
        return NULL;
    }

    buffer->fd = fd;
    buffer->num_remaining = 0;
    buffer->offset = 0;
    // Initialize other fields to 0 or desired values

    return buffer;
}

void read_close(Buffer **pbuf) {
    if (pbuf == NULL || *pbuf == NULL) {
        return; // Invalid input or already freed
    }

    close((*pbuf)->fd); // Close the file using file descriptor from the Buffer
    free(*pbuf); // Free the allocated Buffer
    *pbuf = NULL; // Set the pointer to NULL to indicate it has been freed
}

bool read_uint8(Buffer *buf, uint8_t *x) {
    if (buf == NULL) {
        return false;
    }
    if (buf->num_remaining == 0) {

        ssize_t rc = read(buf->fd, buf->a, sizeof(buf->a));

        if (rc < 0) {
            fprintf(stderr, "You have passed in an invalid file");
        }
        if (rc == 0)
            return false; // end of file
        buf->num_remaining = rc;
        buf->offset = 0;
    }

    if (x != NULL) {
        *x = buf->a[buf->offset];
    }
    buf->offset++;
    buf->num_remaining--;

    return true;
}

bool read_uint16(Buffer *buf, uint16_t *x) {
    if (buf == NULL) {
        return false;
    }
    uint8_t firstByte, secondByte;

    if (!read_uint8(buf, &firstByte)) {
        return false;
    }

    if (!read_uint8(buf, &secondByte)) {
        return false;
    }

    if (x != NULL) {
        *x = (uint16_t) (secondByte << 8) | firstByte;
    }
    return true;
}

bool read_uint32(Buffer *buf, uint32_t *x) {
    uint16_t firstByte, secondByte;
    if (buf == NULL) {
        return false;
    }

    if (!read_uint16(buf, &firstByte)) {
        return false;
    }
    if (!read_uint16(buf, &secondByte)) {
        return false;
    }
    if (x != NULL) {
        *x = (uint32_t) (secondByte << 16) | firstByte;
    }
    return true;
}

#include <fcntl.h>
#include <unistd.h>

Buffer *write_open(const char *filename) {
    int fileDescriptor = creat(filename, 0664);
    if (fileDescriptor < 0) {
        return NULL;
    }

    Buffer *buffer = malloc(sizeof(Buffer));
    if (buffer == NULL) {
        close(fileDescriptor);
        return NULL;
    }

    buffer->fd = fileDescriptor;
    buffer->offset = 0;
    buffer->num_remaining = 0;

    memset(buffer->a, 0, sizeof(buffer->a));

    return buffer;
}

void write_close(Buffer **pbuf) {
    if (pbuf == NULL || pbuf == NULL) {
        return;
    }

    write((*pbuf)->fd, (*pbuf)->a, (*pbuf)->offset);
    close((*pbuf)->fd);
    free(*pbuf);
    *pbuf = NULL;
}

void write_uint8(Buffer *buf, uint8_t x) {
    if (buf->offset == BUFFER_SIZE) {
        ssize_t rc = write(buf->fd, buf->a, BUFFER_SIZE);
        if (rc < 0) {
            fprintf(stderr, "Error writing to file\n");
        }
        buf->offset = 0;
        memset(buf->a, 0, sizeof(buf->a));
    }
    buf->a[buf->offset] = x;
    buf->offset++;
    buf->num_remaining--;
}

void write_uint16(Buffer *buf, uint16_t x) {
    uint8_t firstByte = x;
    uint8_t secondByte = x >> 8;

    write_uint8(buf, (uint8_t) (firstByte));
    write_uint8(buf, (uint8_t) (secondByte));
}
void write_uint32(Buffer *buf, uint32_t x) {
    uint16_t firstByte = x;
    uint16_t secondByte = x >> 16;

    write_uint16(buf, (uint16_t) (firstByte));
    write_uint16(buf, (uint16_t) (secondByte));
}
