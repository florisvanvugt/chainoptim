default: run

chainseqold: chainseq.cpp readme.c designs.c aux.c
# First make the readme which will be printed by the script
#	echo "
# Then compile the script itself
	gcc -Wall readme.c chainseq.c designs.c aux.c -lgsl -lgslcblas -lm -o chainseq


chainseq: chainseq.cpp
# First make the readme which will be printed by the script
#	echo "
# Then compile the script itself
	g++ -Wall chainseq.cpp  -o chainseq  -lboost_program_options



readme.c: readme.txt readme2c.py
	python readme2c.py > readme.c


run: chainseq
	./chainseq

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
	rm -f *~
	rm -f readme.html
	rm -f readme.txt
	rm -f readme.c
	rm -f development_notes.html








