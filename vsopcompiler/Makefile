# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BASE = vsopc
BISON = bison
CXX = g++
FLEX = flex
XSLTPROC = xsltproc
CXXFLAGS = -std=c++14  `llvm-config --cxxflags --ldflags --libs --libfiles --system-libs --includedir`
LDFLAGS = `llvm-config --ldflags --system-libs --libs`

all: $(BASE)

%.cc %.hh %.xml %.gv: %.yy
	$(BISON) $(BISONFLAGS) --xml --graph=$*.gv -o $*.cc $<

%.cc: %.ll
	$(FLEX) $(FLEXFLAGS)  -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS)  -fexceptions -c -o $@ $< $(LDFLAGS)
	
$(BASE): $(BASE).o driver.o parser.o scanner.o ASTnode.o CheckPrinter.o  ParserPrinter.o ErrorHandler.o SymbolTable.o prototype.o FillPrototype.o CheckPrototype.o SymbolTable.o CheckTypeScope.o CodeGenerator.o
	$(CXX)  -o $@ $^ $(LDFLAGS)
	echo $(shell pwd)

$(BASE).o: parser.hh
parser.o: parser.hh
scanner.o: parser.hh

install-tools: 	
	sudo cp IO.vsop /home


html: parser.html
%.html: %.xml
	$(XSLTPROC) $(XSLTPROCFLAGS) -o $@ $$($(BISON) --print-datadir)/xslt/xml2xhtml.xsl $<

CLEANFILES =										\
  $(BASE) *.o										\
  parser.hh parser.cc parser.output parser.xml parser.html parser.gv location.hh	\
  scanner.cc 
clean:
	rm -f $(CLEANFILES)
