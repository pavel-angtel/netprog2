#!/bin/bash

subnet=20
for node in a b c d; do
	ip netns add node_$node
	ip link add node_eth type veth peer name eth_$node
	ip link set dev node_eth netns node_$node
	ip netns exec node_$node ip link set dev node_eth name eth0
	ip netns exec node_$node ip addr add dev eth0 192.168.$subnet.6/24
	ip netns exec node_$node ip link set dev eth0 up
	subnet=$(expr $subnet + 1)

	ip link set dev eth_$node up
done
