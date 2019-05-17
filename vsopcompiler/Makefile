# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BASE = vsopc
BISON = bison
CXX = g++
FLEX = flex
XSLTPROC = xsltproc
CXXFLAGS = -fpic -std=c++14  `llvm-config --cxxflags --ldflags --libs --libfiles --system-libs --includedir`
LDFLAGS = `llvm-config --ldflags --system-libs --libs`

all: $(BASE)

%.cc %.hh %.xml %.gv: %.yy
	$(BISON) $(BISONFLAGS) --xml --graph=$*.gv -o $*.cc $<

%.cc: %.l
	$(FLEX) $(FLEXFLAGS)  -o $@ $<

%.o: %.ll
	llc  -filetype obj -relocation-model pic $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -fexceptions -c -o $@ $< $(LDFLAGS)

$(BASE): $(BASE).o  IO.o driver.o parser.o scanner.o ASTnode.o CheckPrinter.o  ParserPrinter.o ErrorHandler.o SymbolTable.o prototype.o FillPrototype.o CheckPrototype.o SymbolTable.o CheckTypeScope.o CodeGenerator.o
	$(CXX) -fpic -o $@ $^ $(LDFLAGS)
	echo $(shell pwd)

$(BASE).o: parser.hh
parser.o: parser.hh
scanner.o: parser.hh

install-tools: 	
	sudo cp IO.vsop /home
	sudo llc  -filetype obj IO.ll

html: parser.html
%.html: %.xml
	$(XSLTPROC) $(XSLTPROCFLAGS) -o $@ $$($(BISON) --print-datadir)/xslt/xml2xhtml.xsl $<

CLEANFILES =										\
  $(BASE) *.o										\
  parser.hh parser.cc parser.output parser.xml parser.html parser.gv location.hh	\
  scanner.cc 
clean:
	rm -f $(CLEANFILES)
