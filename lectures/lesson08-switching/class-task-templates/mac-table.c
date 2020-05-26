
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
#define MAX_SWITCH_PORTS 8

struct mac_entry {
	struct ether_addr addr;
	unsigned int port_no;
	struct avl_node avl_node;
};

struct eth_switch {
	struct avl_tree mac_table;
};

struct eth_frame {
	struct ether_addr source;
	struct ether_addr dest;

	unsigned int port_no;
};

int avl_maccmp(const void *k1, const void *k2, void *ptr)
{
	const struct ether_addr *mac1 = k1, *mac2 = k2;
	int64_t mac1_int = 0, mac2_int = 0;

	memcpy(&mac1_int, mac1, sizeof(*mac1));
	memcpy(&mac2_int, mac2, sizeof(*mac2));

	if (mac1_int - mac2_int > 0)
		return 1;
	else if (mac1_int - mac2_int < 0)
		return -1;
	else
		return 0;
}

static void dump_mac_table(struct eth_switch *sw)
{
	struct mac_entry *entry;

	printf("MAC table:\n");
	avl_for_each_element(&sw->mac_table, entry, avl_node) {
		printf("%s , port %u\n",
		       ether_ntoa(&entry->addr), entry->port_no);
	}
}

static int learn_mac_address(struct eth_switch *sw, struct eth_frame *frame)
{
	struct mac_entry *entry;

	entry = avl_find_element(&sw->mac_table,
				 &frame->source, entry, avl_node);
	if (entry) {
		FIXME;
	} else {
		FIXME;
	}
	return 0;
}

static int handle_input_frame(struct eth_switch *sw, const char *frame_str)
{
	struct eth_frame frame = {
		.dest = {
			.ether_addr_octet = {0x22, 0x22, 0x22, 0x22, 0x22, 0x00},
		},
		.source = {
			.ether_addr_octet = {0x22, 0x22, 0x22, 0x22, 0x22, 0x00},
		},
	};
	unsigned int port_no, source_lsb, dest_lsb;
	unsigned int output_ports_mask = 0;
	struct mac_entry *entry;
	int ret;
	int i;

	ret = sscanf(frame_str, "%d %d %d", &port_no, &source_lsb, &dest_lsb);
	if (ret != 3) {
		fprintf(stderr, "Invalid command. Expected "
				"<port_no> <source_lsb> <dest_lsb>\n");
		return 0;
	}
	if (port_no >= MAX_SWITCH_PORTS)
		port_no = MAX_SWITCH_PORTS - 1;
	if (source_lsb > 255)
		source_lsb = 255;
	if (dest_lsb > 255)
		dest_lsb = 255;

	frame.source.ether_addr_octet[5] = source_lsb;
	frame.dest.ether_addr_octet[5] = dest_lsb;
	frame.port_no = port_no;
	{
		char sa_str[20], da_str[20];
		printf("Received frame on port %u: SA %s DA %s\n",
		       port_no,
		       ether_ntoa_r(&frame.source, sa_str),
		       ether_ntoa_r(&frame.dest, da_str));
	}

	ret = learn_mac_address(sw, &frame);
	if (ret < 0)
		goto on_error;

	entry = avl_find_element(&sw->mac_table, &frame.dest, entry, avl_node);
	if (entry) {
		printf("Found DA in MAC table\n");
		output_ports_mask FIXME;
	} else {
		for (i = 0; i < MAX_SWITCH_PORTS; i++)
			if (i != port_no)
				output_ports_mask FIXME;
	}

	printf("Transmitting on ports:");
	for (i = 0; i < MAX_SWITCH_PORTS; i++)
		if (output_ports_mask & (1 << i))
			printf("%u ", i);
	printf("\n");
	return 0;
on_error:
	return -1;
}

static void cleanup_switch(struct eth_switch *sw)
{
}

static int init_switch(struct eth_switch *sw)
{
	memset(sw, 0, sizeof(*sw));

	avl_init(&sw->mac_table, avl_maccmp, false, NULL);
	return 0;
}

int main(int argc, char *argv[])
{
	struct eth_switch _sw, *sw = &_sw;
	int ret;

	ret = init_switch(sw);
	if (ret < 0)
		return 1;

	while (1) {
		fd_set fds;
		int max_fd = 0;

		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);
		max_fd = STDIN_FILENO;

		ret = select(max_fd + 1, &fds, NULL, NULL, NULL);
		if (ret < 0) {
			fprintf(stderr, "select() failed: %s\n", strerror(errno));
			goto on_error;
		}

		if (FD_ISSET(STDIN_FILENO, &fds)) {
			char buf[1024];

			ret = read(STDIN_FILENO, buf, sizeof(buf) - 1);
			buf[ret] = '\0';
			if (!strcmp(buf, "\n")) {
				dump_mac_table(sw);
			} else {
				ret = handle_input_frame(sw, buf);
				if (ret < 0)
					goto on_error;
			}
		}
	}

	cleanup_switch(sw);
	return 0;
on_error:
	cleanup_switch(sw);
	return 1;
}
