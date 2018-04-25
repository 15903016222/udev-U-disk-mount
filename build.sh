source /opt/yogurt/i.MX6-PD15.3.0/environment-setup-cortexa9hf-vfp-neon-phytec-linux-gnueabi

#!/bin/bash

$CC ReceiveUSB.c -o receive
$CC SendUSB.c -o send
$CC fifo_receive.c -o read.exe
$CC fifo_send.c -o write.exe
