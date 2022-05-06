#!/usr/bin/env bash

#using read parsing input line

# -rwxr-xr-x 1 zeroway users 209 May  6 20:57 read_lines.sh
ls -l "$1" | { read Perms Lcount Owner Group Size Crdate Crtime Time File;
				echo  $File has $Lcount 'link(s)' and is $Size bytes long;
			}
