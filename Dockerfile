# Stage 1: Build stage
FROM alpine:3.19 AS builder

RUN apk add --no-cache \
    build-base \
    gcc \
    musl-dev \
    curl-dev \
    cmake \
    make

WORKDIR /app

# Copy source and build definitions
COPY CMakeLists.txt Makefile vcpkg.json* ./
COPY include/ ./include/
COPY src/ ./src/

# Compile binary
RUN make clean && make CFLAGS="-O3 -std=c11 -Wall -Wextra -Iinclude"

# Stage 2: Minimal runtime stage
FROM alpine:3.19 AS runner

# Minimal dependencies required at execution
RUN apk add --no-cache \
    libcurl \
    ca-certificates

WORKDIR /root

# Copy static compiled binary from builder
COPY --from=builder /app/bin/sget /usr/local/bin/sget

ENTRYPOINT ["sget"]
CMD ["--help"]