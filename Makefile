.PHONY: output.pdf hackpack-sources.tex

default: hackpack.pdf
all: output.pdf hackpack.pdf

CXXFLAGS ?= -Wall -O2 --std=gnu++14

output.pdf:
	enscript **/{*.pseudo,*.java} -f 'Courier-Bold8' -2 -j -T 2 -r -o - | ps2pdf - output.pdf

hackpack-sources.tex:
	find -E . -iregex '.*[.](pseudo|java|cpp)' -not -iname 'test*' \
		| sed -e 's/^[.]\//\\hackpacklisting{/' \
			-e 's/$$/}{}/' \
			-e '/.java}/ s/{}$$/{Java}/' \
			-e '/.c}/ s/{}$$/{C}/' \
			-e '/.cpp}/ s/{}$$/{C++}/' \
			-e 's/_/\\string_/g' \
		> $@

hackpack.pdf: hackpack.tex hackpack-sources.tex
	pdflatex hackpack.tex