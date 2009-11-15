#!/bin/sh
clear
if [ "$1" = "" ]
then
echo "Compilation en cours"
../../bin/qbuild
echo "Compilation terminée"
fi
if [ "$1" = "x" ]
then
echo " "
echo "Changement des droits et envoi vers le freerunner"
chmod 777 qmokoplayer
scp qmokoplayer root@192.168.9.202:/home/root/
echo " "
echo "connexion au freerunner puis lancement du programme"
ssh -l root 192.168.9.202 '/home/root/remote_env.sh'
fi

if [ "$1" = "cx" ]
then
echo "Compilation en cours"
../../bin/qbuild
echo "Compilation terminée"
echo " "
echo "Changement des droits et envoi vers le freerunner"
chmod 777 qmokoplayer
scp qmokoplayer root@192.168.9.202:/home/root/
echo " "
echo "connexion au freerunner puis lancement du programme"
ssh -l root 192.168.9.202 '/home/root/remote_env.sh'
fi