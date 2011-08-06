VERSION=$(shell ./version.sh)

all: BigMC-$(VERSION).pkg

BigMC.pmdoc/01usr.xml: ../../config.h
	cat BigMC.pmdoc/01usr.xml.template | sed -n 's/<version>\(.*\)<\/version>/<version>$(VERSION)<\/version>/p' > BigMC.pmdoc/01usr.xml

BigMC-$(VERSION).pkg: BigMC.pmdoc/01usr.xml
	/Developer/usr/bin/packagemaker --doc BigMC.pmdoc --verbose --out BigMC-$(VERSION).pkg
