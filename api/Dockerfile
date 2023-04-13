FROM ubuntu

RUN apt-get update -y && \
	apt-get install -y libsqlite3-dev &&\
	apt-get install -y build-essential

WORKDIR /app

COPY . .

RUN make

EXPOSE 8000

CMD ["./api"]
