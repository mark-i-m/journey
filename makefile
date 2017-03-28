
# Identify everything we need to compile
BENCH_CXX=$(wildcard bench_*.cc)
BENCH_C=$(wildcard bench_*.c)

BENCH_CXX_TARGETS=$(patsubst %.cc,%,$(BENCH_CXX))
BENCH_C_TARGETS=$(patsubst %.c,%,$(BENCH_C))

DEPS_H=$(wildcard *.h)
DEPS_CXX=$(filter-out $(BENCH_CXX), $(wildcard *.cc))
DEPS_C=$(filter-out $(BENCH_C), $(wildcard *.c))

DEPS_CXX_OBJS=$(patsubst %.cc, %.o, $(DEPS_CXX))
DEPS_C_OBJS=$(patsubst %.c, %.o, $(DEPS_C))

DEPS=$(DEPS_H) $(DEPS_C_OBJS) $(DEPS_CXX_OBJS) 

# Targets

all: $(BENCH_C_TARGETS) $(BENCH_CXX_TARGETS)

$(BENCH_CXX_TARGETS): % : %.cc $(DEPS)
	g++ -O0 -o $@ $^

$(BENCH_C_TARGETS): % : %.c $(DEPS)
	gcc -O0 -std=gnu99 -o $@ $^

$(DEPS_CXX_OBJS): %.o : %.cc $(DEPS_H)
	g++ -O0 -c -o $@ $<

$(DEPS_C_OBJS): %.o : %.c $(DEPS_H)
	gcc -O0 -std=gnu99 -c -o $@ $<

clean:
	rm -f *.o $(BENCH_C_TARGETS) $(BENCH_CXX_TARGETS)
