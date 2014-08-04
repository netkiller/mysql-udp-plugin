mysql-udp_sendto-plugin
=======================

UDP Sendto 

Build
-----
    # cmake .
    # make && make install

Usage
-----
    select udp_sendto('192.168.2.1','4000','hello');

Test
----
    # nc -luv 4000
