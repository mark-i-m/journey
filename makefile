
TARGETS = $(patsubst %.c, %, $(wildcard *.c))
HEADERS = $(wildcard *.h)

all: $(TARGETS)

$(TARGETS) : % : %.c $(HEADERS)
	gcc -std=gnu99 -o $@ $<

clean:
	rm -f $(TARGETS)
