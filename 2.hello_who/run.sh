make
sudo insmod hello_who.ko howmany=3 whom="Mom"
sudo tail /var/log/syslog
sudo rmmod hello_who.ko
sudo tail /var/log/syslog
make clean