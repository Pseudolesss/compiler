# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BASE = vsopc
BISON = bison
CXX = g++
FLEX = flex
XSLTPROC = xsltproc
CXXFLAGS = 

all: $(BASE)

%.cc %.hh %.xml %.gv: %.yy
	$(BISON) $(BISONFLAGS) --xml --graph=$*.gv -o $*.cc $<

%.cc: %.ll
	$(FLEX) $(FLEXFLAGS) -o$@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o$@ $<

$(BASE): $(BASE).o driver.o parser.o scanner.o ASTnode.o Visitor.o  ErrorHandler.o SymbolTable.o prototype.o FillPrototype.o
	$(CXX) -o $@ $^

$(BASE).o: parser.hh
parser.o: parser.hh
scanner.o: parser.hh

run: $(BASE)
	@echo "Type arithmetic expressions.  Quit with ctrl-d."
	./$< -
install-tools: 
	bison --version
	flex --version

html: parser.html
%.html: %.xml
	$(XSLTPROC) $(XSLTPROCFLAGS) -o $@ $$($(BISON) --print-datadir)/xslt/xml2xhtml.xsl $<

CLEANFILES =										\
  $(BASE) *.o										\
  parser.hh parser.cc parser.output parser.xml parser.html parser.gv location.hh	\
  scanner.cc 
clean:
	rm -f $(CLEANFILES)