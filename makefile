
TARGETS = $(patsubst %.c, %, $(wildcard *.c))
HEADERS = $(wildcard *.h)

all: $(TARGETS)

$(TARGETS) : % : %.c $(HEADERS)
	gcc -o $@ $<

clean:
	rm -f $(TARGETS)
