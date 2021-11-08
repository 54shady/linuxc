print_usage() {
	cat << EOF
Usage: Here goes the detail description
	--help
	--args-one:
	--args-two:
	--args-3th:
	--args-4th:
EOF
}

write_file() {
	cat > /tmp/this.txt << EOF
Usage: Here goes the detail description
	--help
	--args-one:
	--args-two:
	--args-3th:
	--args-4th:
EOF
}

print_usage
write_file
