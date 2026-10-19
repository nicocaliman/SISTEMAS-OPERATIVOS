#!/bin/bash

#login:password:uid:gid:user_name:home:shell

i=0

while IFS=':' read login pass uid gid name home shell; do
    echo -e "[Entry #$i]\n"
    echo -e "\tlogin=$login\n"
    echo -e "\tenc_pass=$pass\n"
    echo -e "\tuid=$uid\n"
    echo -e "\tgid=$gid\n"
    echo -e "\tname=$name\n"
    echo -e "\thome=$home\n"
    echo -e "\tshell=$shell\n"
    ((i++))
done < /etc/passwd

