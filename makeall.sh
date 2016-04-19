for type in unique bincount parity; do
	for foo in table; do
		echo "Making $type ($foo)"
		gcc -o $type-$foo $type.c $foo.c -g
	done
done
