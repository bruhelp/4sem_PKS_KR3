# Окружение Linux
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Тут зависимости
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Копируются исходники
COPY . .

# Сборка проекта
RUN cmake -S . -B build
RUN cmake --build build

# Тестики
RUN ctest --test-dir build --output-on-failure

# Вторая стадия
FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# И ещё стадия сборки
COPY --from=builder /app/build/ElevatorSimulator .

ENTRYPOINT ["./ElevatorSimulator"]