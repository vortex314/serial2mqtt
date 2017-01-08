# grep -rho H\(\".*\"\) * | sort | uniq | sed 's/H("\([^"]*\)")/case H("\1") : return "\1";\/\/---/'
# find . \( -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -print | xargs gawk -f keys.awk
KEYS_CPP=static_labels.cpp
cat >$KEYS_CPP <<EOF
// DON'T EDIT auto-generated
#include <stdint.h>
const char* static_labels[]={
 "dst","src","request","reply","event","error"
EOF
find . \( -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -exec grep --no-filename -rho H\(\"[^\"]*\"\) {} + \
	| sed 's/.*H("\([^"]*\)").*/\1/g' \
	| sort \
	| uniq \
	| sed 's/\(.*\)/,"\1"/' >> $KEYS_CPP
cat >>$KEYS_CPP <<EOF
};
uint32_t static_labels_count = sizeof(static_labels)/sizeof(const char*);
EOF
echo " $KEYS_CPP created."
