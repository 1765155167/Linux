#!/bin/bash

echo "if test -----------------------"
echo -n "Peaes input passwd"
read PASSWD
if [ "$PASSWD" = "hqf666123" ]; then
    echo "success..."
fi
echo "if test end --------------------"
echo -n "Please input passwd"
read PASSWD
count=3
while [ "$PASSWD" != "hqf666123" -a $count -ge 0 ]; do
    echo "passwd ere please input again."
    read PASSWD
done
echo ""
