#!/bin/sh
RIOT_FOLDER="/media/frank/Dati/university/iot/hands-on/Tutorials/RIOT"
cd $RIOT_FOLDER
./dist/tools/tapsetup/tapsetup -c 2
sudo ip a a fec0:affe::1/64 dev tapbr0
ifconfig tapbr0