#!/bin/bash

user=$(cut -d ':' -f1 /etc/passwd)
for username in ${user}
do
	id ${username}
done
