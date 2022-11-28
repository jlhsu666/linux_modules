cmd_/home/ubuntu/driver/Module.symvers := sed 's/\.ko$$/\.o/' /home/ubuntu/driver/modules.order | scripts/mod/modpost -m -a  -o /home/ubuntu/driver/Module.symvers -e -i Module.symvers   -T -
