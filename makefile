
# Identify everything we need to compile
BENCH_CXX=$(wildcard bench_*.cc)
BENCH_CXX_TARGETS=$(patsubst %.cc,%,$(BENCH_CXX))

DEPS_H=$(wildcard *.h)
DEPS_CXX=$(filter-out $(BENCH_CXX), $(wildcard *.cc))
DEPS_CXX_OBJS=$(patsubst %.cc, %.o, $(DEPS_CXX))

DEPS=$(DEPS_H) $(DEPS_CXX_OBJS) 

# Targets

all: $(BENCH_CXX_TARGETS)

$(BENCH_CXX_TARGETS): % : %.cc $(DEPS)
	g++ -Wall -Werror -O0 -o $@ $^

$(DEPS_CXX_OBJS): %.o : %.cc $(DEPS_H)
	g++ -Wall -Werror -O0 -c -o $@ $<

clean:
	rm -f *.o $(BENCH_CXX_TARGETS)
