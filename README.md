mysql-udp_sendto-plugin
=======================

UDP Sendto 

Build
-----
    # git clone https://github.com/netkiller/mysql-udp-plugin.git
    # cd mysql-udp-plugin
    # cmake .
    # make && make install

Install
-------
    create function udp_sendto returns string soname 'libudp_sendto.so';
    
Uninstall
---------
    drop function udp_sendto;

Usage
-----
    select udp_sendto('192.168.2.1','4000','hello');

Test
----
    # nc -luv 4000
