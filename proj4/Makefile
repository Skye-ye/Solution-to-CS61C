CC = gcc
CFLAGS = -g -Wall -std=c99 -fopenmp -mavx2 -mfma -pthread
LDFLAGS = -fopenmp
CUNIT = -lcunit
PYTHON = -I/usr/include/python3.10 -lpython3.10

install:
	if [ ! -f files.txt ]; then touch files.txt; fi
	rm -rf build
	xargs rm -rf < files.txt
	python3 setup.py install --record files.txt

uninstall:
	if [ ! -f files.txt ]; then touch files.txt; fi
	rm -rf build
	xargs rm -rf < files.txt

install_dumb:
	if [ ! -f files2.txt ]; then touch files2.txt; fi
	rm -rf build
	xargs rm -rf < files2.txt
	python3 setup_dumbpy.py install --record files2.txt

uninstall_dumb:
	if [ ! -f files2.txt ]; then touch files2.txt; fi
	rm -rf build
	xargs rm -rf < files2.txt

clean:
	rm -f *.o
	rm -f test
	rm -rf build
	rm -rf __pycache__

test:
	rm -f test
	$(CC) $(CFLAGS) tests/mat_test.c src/matrix.c -o test $(LDFLAGS) $(CUNIT) $(PYTHON)
	./test

.PHONY: test