#!/bin/bash

function echo_red
{
    echo -e "\033[31m$1\033[0m"
}

function echo_green
{
    echo -e "\033[32m$1\033[0m"
}

if [ $# -ne 1 ];then
    echo_red "usage:$0 program"
    exit 1
fi

program="$1"

echo "execute main program"

$program
ret=$?
if [ $ret -ne 0 ];then
    echo_red "main program wrong,ret $ret"
    exit 1
fi

echo_green "run correctly"
