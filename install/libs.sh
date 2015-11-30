warning()
{
	/bin/echo -e -n '\e[0;31mWarning: '
	/bin/echo "$@"
	/bin/echo -e -n '\e[0m'
}

info()
{
	/bin/echo -e -n '\e[0;34mInformation: '
	/bin/echo "$@"
	/bin/echo -e -n '\e[0m'
}
