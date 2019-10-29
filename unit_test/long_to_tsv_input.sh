#! /bin/csh

awk -F"\t" '{print $1"\t"$20"\t"$21"\t"$22}' < $1 | tail +2
