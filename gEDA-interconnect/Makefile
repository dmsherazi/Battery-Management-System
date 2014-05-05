PCBNAME=interconnect
REV=0.0

all: clean hackvana-zip

clean:
	rm -rf *.gbr *.cnc bom xy *.zip *.png *.pdf *.ps gerbers  $(PCBNAME).gbl $(PCBNAME).gbo $(PCBNAME).gbs $(PCBNAME).gtl $(PCBNAME).gto $(PCBNAME).gts $(PCBNAME).txt fab.gbr $(PCBNAME).gm1 $(PCBNAME).drl $(PCBNAME)_NPTH.drl $(PCBNAME)_make.pcb

update:
	gsch2pcb -v -v --elements-dir ./../../meatstand-common/geda-common/pcb-elements/ ${PCBNAME}.sch
common:
	gschem -q -p -o $(PCBNAME).ps -s /usr/share/gEDA/scheme/print.scm $(PCBNAME).sch
	ps2pdf $(PCBNAME).ps schematic.pdf
	rm $(PCBNAME).ps
	pdftoppm -png schematic.pdf > schematic.png
	sed -e 's/XXX/$(REV)/' < $(PCBNAME).pcb > $(PCBNAME)_make.pcb
	pcb -x png --dpi 1000 --photo-mode --outfile front.png $(PCBNAME)_make.pcb
	pcb -x png --dpi 1000 --photo-mode --photo-flip-x --outfile back.png  $(PCBNAME)_make.pcb
	convert front.png -trim front.png
	convert back.png -trim back.png
	convert -border 40x40 -bordercolor "#000000"  front.png front.png
	convert -border 40x40 -bordercolor "#000000"  back.png back.png
	convert +append front.png back.png board.png
	rm back.png front.png $(PCBNAME)_make.pcb

hackvana-zip:
	mkdir gerbers
	sed -e 's/XXX/$(REV)/' < $(PCBNAME).pcb > $(PCBNAME)_make.pcb
	pcb -x gerber --all-layers $(PCBNAME)_make.pcb
	pcb -x bom $(PCBNAME)_make.pcb
	mv $(PCBNAME)_make.bom bom
	mv $(PCBNAME)_make.xy xy
	cp $(PCBNAME)_make.bottom.gbr     $(PCBNAME).gbl
	cp $(PCBNAME)_make.bottomsilk.gbr      $(PCBNAME).gbo
	cp $(PCBNAME)_make.bottommask.gbr      $(PCBNAME).gbs
	cp $(PCBNAME)_make.top.gbr         $(PCBNAME).gtl
	cp $(PCBNAME)_make.topsilk.gbr     $(PCBNAME).gto
	cp $(PCBNAME)_make.topmask.gbr     $(PCBNAME).gts
	cp $(PCBNAME)_make.plated-drill.cnc  $(PCBNAME).drl
#	cp $(PCBNAME)_make.unplated-drill.cnc  $(PCBNAME)_NPTH.drl
	cp $(PCBNAME)_make.outline.gbr $(PCBNAME).gm1
	zip $(PCBNAME)_rev$(REV).zip $(PCBNAME).gbl $(PCBNAME).gbo \
		$(PCBNAME).gbs $(PCBNAME).gtl $(PCBNAME).gto \
		$(PCBNAME).gts $(PCBNAME).drl $(PCBNAME)_NPTH.drl \
		$(PCBNAME).gm1 README.txt
	rm  $(PCBNAME).gbl $(PCBNAME).gbo $(PCBNAME).gm1 \
		$(PCBNAME).gbs $(PCBNAME).gtl $(PCBNAME).gto \
		$(PCBNAME).gts $(PCBNAME).drl
#            $(PCBNAME).gts $(PCBNAME).drl $(PCBNAME)_NPTH.drl
	mv *.gbr gerbers/
	mv *.cnc gerbers/

