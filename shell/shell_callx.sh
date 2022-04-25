#!/usr/bin/env bash

set -euo pipefail

sayhi::ruby() {
ruby - $1  <<'EOF_RB'
puts "Ruby: Hello #{ARGV[0]}";
EOF_RB
}

sayhi::perl() {
perl - $1 <<'EOF_PL'
print "Perl: Hello $ARGV[0]";
EOF_PL
}

sayhi::python() {
python - $1 <<'EOF_PY'
from __future__ import print_function
import sys
print('Python: Hello %s' % sys.argv[1])
EOF_PY
}

sayhi=""
name=${1:-'Everyone'}
candidates=(python perl ruby)

# chose the first avaliable candidate
for candidate in ${candidates[@]}; do
	if command -v $candidate 1>/dev/null; then
		sayhi="sayhi::$candidate"
		break
	fi
done

if [ -z "$sayhi" ]; then
	echo "could not find any environment to run" >&2
	echo "tried: ${candidates[*]}" >&2
	exit 1
fi

status=$($sayhi $name)
echo $status
