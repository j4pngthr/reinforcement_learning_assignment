
FILE = Makefile main.py gambler.py
DIR  = 03_Gambler

all: 

zip:
	# directory
	if [ ! -d $(DIR) ]; then \
		mkdir $(DIR); \
	fi
	
	# copy files
	for var in $(FILE); do\
		cp $$var $(DIR)/$$var; \
	done \
	
	# zip
	zip -r $(DIR).zip $(DIR)
	rm -r $(DIR)
	
clean:
	rm *.pyc
	rm -r __pycache__
	