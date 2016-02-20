# Testing connection aborts

stopping traffic from testbed:
``iptables -I INPUT -s 192.168.1.123 -j REJECT``

reenabling traffic from testbed:
``iptables -D INPUT -s 192.168.1.123 -j REJECT``

