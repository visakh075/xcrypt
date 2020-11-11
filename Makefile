cross/lib/%:lib/%.c
	mkdir -p cross/lib
	echo $<
	arm-linux-gnueabi-gcc -c $< -o $@_arm
	aarch64-linux-gnu-gcc -c $< -o $@_aarch64
	i686-linux-gnu-gcc -c $< -o $@_i686
	x86_64-linux-gnu-gcc -c $< -o $@_x86_64
	echo
libs:cross/lib/argman cross/lib/keyman cross/lib/libcrypt
cross/hexcomp:lib/hexcomp.c
	arm-linux-gnueabi-gcc -c $< -o $@_arm
	aarch64-linux-gnu-gcc -c $< -o $@_aarch64
	i686-linux-gnu-gcc -c $< -o $@_i686
	x86_64-linux-gnu-gcc -c $< -o $@_x86_64
cross/xcrypt:libs cross/hexcomp
	arm-linux-gnueabi-gcc -o $@_arm xcrypt.c cross/lib/*_arm -lpthread
	aarch64-linux-gnu-gcc -o $@_aarch64 xcrypt.c cross/lib/*_aarch64 -lpthread
	i686-linux-gnu-gcc -o $@_i686 xcrypt.c cross/lib/*_i686 -lpthread
	x86_64-linux-gnu-gcc -o $@_x86_64 xcrypt.c cross/lib/*_x86_64 -lpthread
all:libs cross/xcrypt
clean:
	rm -r cross/*