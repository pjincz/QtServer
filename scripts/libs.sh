warning()
{
	for x in "$@"; do
		printf "\033[0;31mWarning: %s\033[0m\n" "$x"
	done
}

info()
{
	for x in "$@"; do
		printf "\033[0;34mInformation: %s\033[0m\n" "$x"
	done
}
