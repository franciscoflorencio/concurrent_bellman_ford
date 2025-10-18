CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGET = out
PERF_TARGET = perf_test
SRCS = main.c bellman.c barrier.c
PERF_SRCS = performance_test.c bellman.c barrier.c
OBJS = $(SRCS:.c=.o)
PERF_OBJS = $(PERF_SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target para o programa de teste de desempenho
$(PERF_TARGET): $(PERF_OBJS)
	$(CC) $(CFLAGS) -o $@ $(PERF_OBJS)

run: $(TARGET)
	./$(TARGET)

run-args: $(TARGET)
	./$(TARGET) $(ARGS)

# Alvos de teste
test: $(TARGET)
	@echo "Executando teste básico..."
	./$(TARGET)

# Alvo para testes de desempenho
performance: $(PERF_TARGET)
	@echo "Executando testes de desempenho..."
	./$(PERF_TARGET) | tee performance_results.txt

check: test

clean:
	rm -f $(OBJS) $(PERF_OBJS) $(TARGET) $(PERF_TARGET) performance_results.txt

.PHONY: all clean run run-args test performance check
