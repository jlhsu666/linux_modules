cmd_/home/ubuntu/driver/modules.order := {   echo /home/ubuntu/driver/hello.ko; :; } | awk '!x[$$0]++' - > /home/ubuntu/driver/modules.order
