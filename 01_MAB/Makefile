
FILE = Makefile bandit_algorithm.py mba.py myio.py egreedy.py main.py mba_sim.sh
DIR  = 01_MBA

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
	