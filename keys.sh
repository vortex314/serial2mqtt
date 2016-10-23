# grep -rho H\(\".*\"\) * | sort | uniq | sed 's/H("\([^"]*\)")/case H("\1") : return "\1";\/\/---/'
# find . \( -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -print | xargs gawk -f keys.awk 
KEYS_CPP=src/keys.cpp
cat >$KEYS_CPP <<EOF
#include <EventBus.h>

const char* hash2string(uint32_t hash)
{
    switch(hash)
    {
	case 0 :  return "type";
EOF
find . \( -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -exec grep --no-filename -rho H\(\"[^\"]*\"\) {} + \
	| sed 's/.*H("\([^"]*\)").*/\1/g' \
	| sort \
	| uniq \
	| sed 's/\(.*\)/case H("\1") : return "\1";/' >> $KEYS_CPP
cat >>$KEYS_CPP <<EOF
default :
        return "UNDEFINED";
    }
}
EOF
echo " $KEYS_CPP created."
