
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <limits.h>
#include <errno.h>

#include <libubox/avl.h>
#include <libubox/avl-cmp.h>

#define FIXME 0
#define MAX_PACKET_SIZE 65536

static int rx_frame(int ifindex)
{
	int sock = -1;
	struct sockaddr_ll addr, rx_addr;
	uint8_t buf[MAX_PACKET_SIZE];
	socklen_t addrlen = sizeof(rx_addr);
	struct ethhdr *ethhdr;
	struct ether_addr source;
	struct ether_addr dest;
	int ret = 0;

	sock = socket(FIXME);
	if (sock < 0) {
		fprintf(stderr, "Failed to create packet socket: %s\n", strerror(errno));
		goto on_exit;
	}

	memset(&addr, 0, sizeof(struct sockaddr_ll));
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = FIXME;

	ret = bind(sock, (const struct sockaddr *) &addr, sizeof(addr));
	if (ret == -1) {
		fprintf(stderr, "Failed to bind socket to ifindex %d: %s\n",
				ifindex, strerror(errno));
		goto on_exit;
	}

	ret = recvfrom(sock,
		       buf,
		       MAX_PACKET_SIZE,
		       0,
		       FIXME
		       FIXME);
	if (ret < 0) {
		fprintf(stderr,
			"Failed to receive frame on interface %d: %s\n",
			ifindex, strerror(errno));
		goto on_exit;
	}
	ethhdr = FIXME
	memcpy(&source, ethhdr->h_source, ETH_ALEN);
	memcpy(&dest, ethhdr->h_dest, ETH_ALEN);

	if (rx_addr.sll_pkttype == FIXME)
		goto on_exit;

	{
		char sa_str[20], da_str[20];
		printf("Received frame (SRC %s DST %s) on interface %u\n",
		       ether_ntoa_r(&source, sa_str),
		       ether_ntoa_r(&dest, da_str),
		       ifindex);
	}
on_exit:
	if (sock >= 0)
		close(sock);
	return ret;
}

int main(int argc, char *argv[])
{
	int ret;

	if (argc == 1) {
		printf("Specify ifindex\n");
		return 0;
	}

	ret = rx_frame(atoi(argv[1]));
	if (ret < 0)
		return 1;

	return 0;
}
