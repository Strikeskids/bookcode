.PHONY: output.pdf hackpack-sources.tex

default: hackpack.pdf
all: output.pdf hackpack.pdf

output.pdf:
	enscript **/{*.pseudo,*.java} -f 'Courier-Bold8' -2 -j -T 2 -r -o - | ps2pdf - output.pdf

hackpack-sources.tex:
	find -E . -iregex '.*[.](pseudo|java|cpp)' \
		| sed -e 's/^.\//\\hackpacklisting{}{/' -e 's/$$/}/' -e 's/_/\\string_/g' \
		> $@

hackpack.pdf: hackpack.tex hackpack-sources.tex
	pdflatex hackpack.tex