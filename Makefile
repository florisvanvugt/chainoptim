default: run

CC      = g++
CFLAGS  = -I. -Wall
LDFLAGS = -lboost_program_options

objects = gamirf.o aux.o design.o chain.o chainseq.o


chainseq: $(objects)
# Linking
	$(CC) -o chainseq $(objects) $(LDFLAGS)

# Some dependencies
#design.o: aux.o irf.hpp gamirf.o

#chain.o: design.o

#gamirf.o: irf.o

$(objects): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@



test: chainseq
	./chainseq --ntp 50 --tr 1.6 --ntrials 3 --trial_duration 2 --choose max --randomseed 873 --1d afniout.1d --Xout mat.txt --history history.txt

simpletest: chainseq
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
	rm -f *.o
	rm -f tmp.* mat.txt history.txt afniout.1d
	rm -f *~
	rm -f readme.html
	rm -f readme.txt 
	rm -f development_notes.html

