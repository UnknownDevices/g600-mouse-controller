output_name := g600-mouse-controller
install_dir := /usr/local/bin

output: g600.c
	clang g600.c -w -o $(output_name)
	chown :input $(output_name)

clean:
	rm $(output_name)

install:
	cp $(output_name) $(install_dir)/$(output_name)
	chown :input $(install_dir)/$(output_name)

uninstall:
	rm $(install_dir)/$(output_name)