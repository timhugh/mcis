FROM gcc:latest as builder

WORKDIR /app
RUN apt update && apt install -y \
    libpq-dev
COPY . .
RUN make -j

FROM ubuntu:latest
WORKDIR /app
RUN apt update && apt install -y \
    libpq5
COPY --from=builder /app/build/tileserver ./
CMD ["./tileserver"]