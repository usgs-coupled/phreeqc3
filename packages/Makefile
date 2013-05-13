##########################################################################
# Build the tar.gz for building with configure/make.                     #
##########################################################################

NAME ?= phreeqc

UPDATE := $(shell svn up ..)

REL := $(shell svn st -v ../configure.ac | cut -b 10- | ( read one two ; echo $$one ) )

TOP_DIR := $(shell pwd)

HOST32 := lobo07

HOST64 := lobo02

M32 := $(shell ssh -q $(HOST32) 'uname -m')

M64 := $(shell ssh -q $(HOST64) 'uname -m')

CONF_PKG := $(NAME)-$(VER)-$(REL).tar.gz

PKG_32 := $(NAME)-$(VER)-$(REL).$(M32).tar.gz

PKG_64 := $(NAME)-$(VER)-$(REL).$(M64).tar.gz

TARBALL := $(NAME)-src-$(VER)-$(REL).tar.gz

REPOS_PATH ?= trunk

all : $(CONF_PKG) $(PKG_32) $(PKG_64)
	rm -rf $(NAME)-$(VER)-$(REL)
	@echo "DATE=$(DATE)"       >> history
	@echo "VER=$(VER)"         >> history
	@echo "REL=$(REL)"         >> history
	@echo "==================" >> history

conf : $(CONF_PKG)

$(CONF_PKG) : $(TARBALL)
	rm -rf $(NAME)-$(VER)-$(REL)
	tar xvzf $(TARBALL)
	cd $(NAME)-src-$(VER)-$(REL) && autoreconf -fvi
	cd $(NAME)-src-$(VER)-$(REL) && mkdir _build
	cd $(NAME)-src-$(VER)-$(REL) && cd _build && ../configure
	cd $(NAME)-src-$(VER)-$(REL) && cd _build && make distcheck
	mv $(NAME)-src-$(VER)-$(REL)/_build/$(CONF_PKG) .
	rm -rf $(NAME)-src-$(VER)-$(REL)

64 : $(PKG_64)

$(PKG_64) : $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/src/phreeqc
	rm -rf $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)
	mkdir $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)
	ssh -q $(HOST64) 'cd $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64 && make install'
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/share/doc/phreeqc $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/doc
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/doc/NOTICE $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/.
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/doc/README $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/.
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/doc/RELEASE $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/.
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/doc/database $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/.
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/doc/examples $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/.
	mkdir $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/test
	install $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/test/test.sh $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/test/.
	install $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/test/clean.sh $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/test/.
	rm -rf $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)/share
	cd $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64 && tar cvzf $(NAME)-$(VER)-$(REL).$(M64).tar.gz $(NAME)-$(VER)-$(REL)
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL).$(M64).tar.gz .

$(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/src/phreeqc : $(NAME)-$(VER)-$(REL)/_build64
	ssh -q $(HOST64) 'cd $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64 && ../configure --prefix=$(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64/$(NAME)-$(VER)-$(REL)'
	ssh -q $(HOST64) 'cd $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build64 && make LDFLAGS=-all-static LIBGMP=/usr/lib64/libgmp.a'

$(NAME)-$(VER)-$(REL)/_build64 : $(CONF_PKG)
	rm -rf $(NAME)-$(VER)-$(REL)
	tar xvzf $(CONF_PKG)
	mkdir $(NAME)-$(VER)-$(REL)/_build64


32 : $(PKG_32)

$(PKG_32) : $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/src/phreeqc
	rm -rf $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)
	mkdir $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)
	ssh -q $(HOST32) 'cd $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32 && make install'
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/share/doc/phreeqc $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/doc
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/doc/NOTICE $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/.
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/doc/README $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/.
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/doc/RELEASE $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/.
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/doc/database $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/.
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/doc/examples $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/.
	mkdir $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/test
	install $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/test/test.sh $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/test/.
	install $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/test/clean.sh $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/test/.
	rm -rf $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)/share
	cd $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32 && tar cvzf $(NAME)-$(VER)-$(REL).$(M32).tar.gz $(NAME)-$(VER)-$(REL)
	mv $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL).$(M32).tar.gz .

$(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/src/phreeqc : $(NAME)-$(VER)-$(REL)/_build32
	ssh -q $(HOST32) 'cd $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32 && ../configure --prefix=$(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32/$(NAME)-$(VER)-$(REL)'
	ssh -q $(HOST32) 'cd $(TOP_DIR)/$(NAME)-$(VER)-$(REL)/_build32 && make LDFLAGS=-all-static LIBGMP=/usr/lib/libgmp.a'

$(NAME)-$(VER)-$(REL)/_build32 : $(CONF_PKG)
	rm -rf $(NAME)-$(VER)-$(REL)
	tar xvzf $(CONF_PKG)
	mkdir $(NAME)-$(VER)-$(REL)/_build32


tarball : $(TARBALL)

$(TARBALL) : dist.sh
ifeq ($(DATE),)
	$(error usage: make DATE=<m/d/yyyy> VER=<x.y.z> [REL=x])
endif
ifeq ($(VER),)
	$(error usage: make DATE=<m/d/yyyy> VER=<x.y.z> [REL=x])
endif
	@echo "Creating $(TARBALL)"
	@rm -f $(TARBALL)
	time ./dist.sh -v $(VER) -r $(REL) -d $(DATE) -pr $(REPOS_PATH) 2>&1 | tee dist-$(REL).log

usage:
	@echo ""
	@echo "usage: make DATE=<m/d/yyyy> VER=<x.y.z> [REL=x]"
	@echo "    ie make DATE=8/12/2010 VER=2.17.3"
	@echo "    ie make DATE=8/12/2010 VER=2.17.3 REL=4000"
	@echo ""
