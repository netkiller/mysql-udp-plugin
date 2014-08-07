<?php
$socket = stream_socket_server("udp://192.168.2.1:4000", $errno, $errstr, STREAM_SERVER_BIND);
if (!$socket) {
      die("$errstr ($errno)");
}

do {

  $line = stream_socket_recvfrom($socket, 4096, STREAM_OOB, $peer)){
  printf("%s\n",$line);
  echo "$peer\n";
} while (true);

?>
