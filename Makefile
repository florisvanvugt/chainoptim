default: run

CC = g++
CFLAGS=-I. -Wall
LDFLAGS=-lboost_program_options

objects = aux.o design.o chainseq.o


chainseq: design.o chainseq.o aux.o
# Linking
	$(CC) -o chainseq $(objects) $(LDFLAGS)


$(objects): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

# Some dependencies
design.o: aux.o irf.hpp



test: chainseq
	./chainseq --ntp 12 --tr 3.3 --ntrials 2 --trial_duration 2


run: chainseq
	./chainseq --help

doc: readme.html
	xdg-open readme.html

readme.html: readme.md misc/github-pandoc.css
	pandoc -f markdown --toc -t html readme.md -s -c misc/github-pandoc.css -o readme.html

readme.txt: readme.md
	pandoc -f markdown -t plain readme.md -s -o readme.txt


development_notes.html: development_notes.md misc/github-pandoc.css
	pandoc -f markdown -t html development_notes.md -s -c misc/github-pandoc.css -o development_notes.html

develdoc: development_notes.html
	xdg-open development_notes.html


clean:
	rm -f chainseq
	rm *.o
	rm -f *~
	rm -f readme.html
	rm -f readme.txt
	rm -f development_notes.html








