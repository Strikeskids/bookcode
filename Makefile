.PHONY: output.pdf

output.pdf:
	enscript **/{*.pseudo,*.java} -f 'Courier-Bold8' -2 -j -T 2 -r -o - | ps2pdf - output.pdf