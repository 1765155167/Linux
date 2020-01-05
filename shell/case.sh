#!/bin/bash

echo "It's morning, please input yes or no."
read YES_OR_NO

case "$YES_OR_NO" in
yes|Yes|YES|y)
    echo "Good Morning !!";;
[nN][Oo])
    echo "Good Afternoon!!";;
*)
    echo "Sorry $YES_OR_NO isn't yes or no.";;
esac
