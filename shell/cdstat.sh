#!/usr/bin/env bash

# https://mroach.com/2020/09/checking-cd-rom-status-with-bash/
# https://github.com/mroach/misc

set -euo pipefail

cdstat::ruby() {
ruby - $1  <<'EOF_RB'
File.open(ARGV[0], File::NONBLOCK) { |fd| print fd.ioctl(0x5326) }
EOF_RB
}

cdstat::perl() {
perl - $1 <<'EOF_PL'
sysopen(my $fd, $ARGV[0], 2048) or die("unable to open device");
print ioctl($fd, 0x5326, 1);
close($fd);
EOF_PL
}

cdstat::python() {
python - $1 <<'EOF_PY'
import os, sys, fcntl
fd = os.open(sys.argv[1], os.O_NONBLOCK) or os.exit(1)
print fcntl.ioctl(fd, 0x5326)
os.close(fd)
EOF_PY
}

cdstat=""
device=${1:-/dev/cdrom}
candidates=(ruby perl python)

for candidate in ${candidates[@]}; do
	if command -v $candidate 1>/dev/null; then
		cdstat="cdstat::$candidate"
		break
	fi
done

if [ -z "$cdstat" ]; then
	echo "could not find any environment to run" >&2
	echo "tried: ${candidates[*]}" >&2
	exit 1
fi

statuses=(none no_disc tray_open not_ready ready)
status=$($cdstat $device)

echo "${statuses[$status]}"
