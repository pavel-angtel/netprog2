#!/bin/bash

var_addr=20
for node in a b c d; do
	ip netns add node_$node
	ip link add node_eth type veth peer name eth_$node
	ip link set dev node_eth netns node_$node
	ip netns exec node_$node ip link set dev node_eth name eth0
	ip netns exec node_$node ip addr add dev eth0 192.168.$var_addr.6/22
	ip netns exec node_$node ip link set dev eth0 up
	var_addr=$(expr $var_addr + 1)

	ip link set dev eth_$node up
done
