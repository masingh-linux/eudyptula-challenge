# create a .rules file under /etc/udev/rules.d folder and this infomation
#ACTION=="add", SUBSYSTEM=="usb", ATTR{idVendor}=="0781", ATTR{idProduct}=="    558a", RUN+="/usr/bin/modprobe usb_keyword"
