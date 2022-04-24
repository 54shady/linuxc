#!/usr/bin/env bash

# Perfect, put each block in one line
[ -n $1 ] \
	&& echo "Perfect;-)" \
	|| echo "Bad result :-("

[ -n $1 ] \
	&& { echo -n "Perfect;-)" ; echo "...!"; } \
	|| { echo "Bad result :-("; echo "."; }

# Good
[ -n $1 ] && echo "Good;-)" \
			|| echo "Bad result :-("

# not bad
#[ -n $1 ] && echo "Not bad;-)" || \
#	echo "Bad result :-("

## Bad idea
#[ -n $1 ] && \
#	echo "Bad Idea;-)" || \
#	echo "Bad result :-("
