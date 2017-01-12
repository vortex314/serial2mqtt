# grep -rho H\(\".*\"\) * | sort | uniq | sed 's/H("\([^"]*\)")/case H("\1") : return "\1";\/\/---/'
# find . \( -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -print | xargs gawk -f keys.awk
OUTPUT=$1_labels.h
cat >$OUTPUT <<EOF
// DON'T EDIT auto-generated
static const char* labels[]={"dummy" 
EOF
# find . \( -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -exec grep --no-filename -rho H\(\"[^\"]*\"\) {} + \
find . \( -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -exec grep --no-filename -rho H\(\"[^\"]*\"\) {} + \
	| sed 's/.*H("\([^"]*\)").*/\1/g' \
	| sort \
	| uniq \
	| sed 's/\(.*\)/"\1"/' \
	| tr '\n' ',' >> $OUTPUT
cat >>$OUTPUT <<EOF
};
#define LABEL_COUNT (sizeof(labels)/sizeof(const char*))
EOF
