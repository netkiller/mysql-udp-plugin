/*
 * Author neo chen <netkiller@msn.com>
 * Homepage http://netkiller.github.io
 * FYI http://knez.weebly.com/
 
 to compile
    gcc -shared -fPIC -o udp_sendto.so udp_sendto.c -I /usr/include/mysql/
  
 setup udf plugin
    sudo cp udp_sendto.so /usr/lib/mysql/plugin/
    you didn't need to restart your mysql 
 
 install
    CREATE FUNCTION udp_sendto RETURNS STRING SONAME 'udp_sendto.so';
 uninstall
    DROP FUNCTION udp_sendto;
 
 useage
  SELECT udp_sendto("192.168.2.15","4000","Hello from mysql!");
 
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <mysql.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

extern int errno;

extern my_bool udp_sendto_init(UDF_INIT *initid,
          UDF_ARGS *args,
          char *message);
extern char *udp_sendto(UDF_INIT *initid,
         UDF_ARGS *args,
         char *result,
         unsigned long *length,
         char *is_null,
         char *error);
extern void udp_sendto_deinit(UDF_INIT *initid);



my_bool udp_sendto_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
  initid->ptr= NULL;
  return 0;
}

char *udp_sendto(UDF_INIT *initid,
        UDF_ARGS *args,
        char *result,
        unsigned long *length,
        char *is_null, char *error)
{
  char hoststr[16]; //only ipv4
  char portstr[16]; //any
  int rv;
  int sok;
  int optval = 1;
  int broadcastOn = 1;
  int port;
  struct sockaddr_in si_other;
  int slen=sizeof(si_other);
  
  
  memcpy(hoststr, args->args[0], args->lengths[0]);
  hoststr[args->lengths[0]]= '\0';
  memcpy(portstr, args->args[1], args->lengths[1]);
  portstr[args->lengths[1]]= '\0';
  
   port=atoi(portstr);
   si_other.sin_family = AF_INET;
   si_other.sin_port = htons(port);
   
   if (inet_aton(hoststr, &si_other.sin_addr)==0) {
         *length= 0;
  return result;
   }
  sok= socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  
  if (sok < 0) {
     snprintf(error, MYSQL_ERRMSG_SIZE, "fail socket(%d, %d, %d): %s",
       (int) AF_INET,
       (int) SOCK_DGRAM,
       (int) 0,
       strerror(errno));
    *length= strlen(error);
    return error;
   
  }

   if( strcmp("255.255.255.255", args->args[0] ) == 0 ){
       setsockopt( sok, SOL_SOCKET, SO_BROADCAST, &broadcastOn, sizeof(broadcastOn) );
   }
   
   setsockopt(sok, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

   rv= sendto(sok,
        args->args[2],
        args->lengths[2],
        0,
        (struct sockaddr*)&si_other,
        slen );

   
  
  if (rv < 0) {
    snprintf(error, MYSQL_ERRMSG_SIZE, "fail sendto(%d, %p, %d): %s",
       (int) sok,
       args->args[2],
       (int)args->lengths[2],
       strerror(errno));
    close(sok);
    *length= strlen(error);
    return error;
   }

  
  close(sok);
  *length= 0;
  return result;
}

void udp_sendto_deinit(UDF_INIT *initid)
{
  return;
}

