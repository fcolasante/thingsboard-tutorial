#!/bin/sh
RIOT_FOLDER="/media/frank/Dati/university/iot/hands-on/Tutorials/RIOT"
cd $RIOT_FOLDER
./dist/tools/tapsetup/tapsetup -c 2
sudo ip a a fec0:affe::1/64 dev tapbr0
ifconfig tapbr0
#make clean all term
echo "ifconfig 5 add fec0:affe::99"
echo "con fec0:affe::1 1885"