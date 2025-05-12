#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "hugo.h"

#define LEVELS 10
#define LEVEL_STEP 25 // (255 / LEVELS)
#define SERVER_PORT 1337
#define DATAGRAM_BYTES (HUGO_WIDTH * HUGO_HEIGHT)

void draw(uint8_t* image) {
	static int cycle_index = 0;
	for (int y=0; y<HUGO_HEIGHT; ++y) {
		hugo_begin_row(y);
		for (int x=0; x<HUGO_WIDTH; ++x) {
			hugo_draw(image[y * HUGO_WIDTH + x] > (cycle_index * LEVEL_STEP));
		}
		hugo_end_row(y);
	}
	cycle_index = (cycle_index + 1) % LEVELS;
}

bool receive_image(int sock, uint8_t* image) {
	static uint8_t buffer[DATAGRAM_BYTES];
	int bytes = read(sock, buffer, DATAGRAM_BYTES);
	if (bytes == DATAGRAM_BYTES) {
		memmove(image, buffer, bytes);
		draw(image);
		return true;
	}
	else {
		return false;
	}
}

int main(int argc, char *argv[]) {
	uint8_t image[HUGO_WIDTH * HUGO_HEIGHT];
	memset(image, 0, sizeof(image));

	int sock;
	struct sockaddr_in name;
	struct hostent *hp, *gethostbyname();

	/* Create socket from which to read */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)   {
		perror("opening datagram socket");
		exit(1);
	}

	fcntl(sock, F_SETFL, O_NONBLOCK | SO_REUSEADDR);

	bzero((char *) &name, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	name.sin_port = htons(SERVER_PORT);

	if (bind(sock, (struct sockaddr *) &name, sizeof(name))) {
		perror("binding datagram socket");
		exit(1);
	}

	printf("hugo listening for images on port %d\n", ntohs(name.sin_port));
	hugo_setup();

	while (true) {
		receive_image(sock, image);
		draw(image);
	}

	close(sock);
}

