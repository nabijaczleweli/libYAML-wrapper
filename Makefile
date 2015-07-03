# The MIT License (MIT)

# Copyright (c) 2015 nabijaczleweli

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


include configMakefile


SOURCES := $(sort $(shell $(FIND) $(SRCDIR) -name *.cpp))
LIBYAML_SOURCES := $(sort $(shell $(FIND) libyaml/$(SRCDIR) -name *.c))
TEST_SOURCES := $(filter-out $(TSTDIR)test.cpp,$(sort $(shell $(FIND) $(TSTDIR) -name *.cpp)))


.PHONY : all test clean git


all : $(OUTDIR)$(PREDLL)libyaml-wrapper$(DLL) $(SOURCES)

test : $(TSTDIR)test$(EXE)
	@cp $(OUTDIR)$(PREDLL)libyaml-wrapper$(DLL) $(dir $(filter-out all,$^))
	cd $(TSTDIR) && ./test$(EXE) -s
# -r compact
	@rm $(TSTDIR)/*$(DLL)

clean :
	rm -rf $(OUTDIR) $(TSTDIR)*$(DLL) $(TSTDIR)*$(EXE)

git :
	git submodule update --recursive --init --remote
	@$(MKDIR) catch 1>$(devnull) 2>&1 || :
	curl -s https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp -ocatch/catch.hpp


$(OUTDIR)$(PREDLL)libyaml-wrapper$(DLL) : $(subst $(SRCDIR),$(OBJDIR)libyaml/,$(subst .c,$(OBJ),$(subst libyaml/,,$(LIBYAML_SOURCES)))) $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(SOURCES)))
	$(CXX) $(CХХAR) -shared -o$@ $^
	$(STRIP) $(STRIPAR) $@

$(TSTDIR)test$(EXE) : all $(TSTDIR)test$(OBJ) $(TEST_SOURCES)
	$(CXX) $(CХХAR) -isystemsrc -I. -o$@ -L$(OUTDIR) -llibyaml-wrapper $(filter-out all,$^)

$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@$(MKDIR) -p $(dir $@) || :
	$(CXX) $(CХХAR) -c -o$@ $^

$(TSTDIR)test$(OBJ) : $(TSTDIR)test.cpp
	@$(MKDIR) -p $(dir $@) || :
	$(CXX) $(CХХAR) -isystemsrc -I. -c -o$@ $^

$(OBJDIR)libyaml/%$(OBJ) : libyaml/$(SRCDIR)%.c
	@$(MKDIR) -p $(dir $@) || :
	$(CC) $(CCAR) -c -o$@ $^