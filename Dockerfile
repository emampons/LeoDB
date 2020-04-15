FROM alpine:latest

RUN apk add --no-cache --upgrade g++ build-base cmake make ansible bash \
    && mkdir -p leodb/

COPY . leodb

ARG ACCESS_FOLDERS="leodb /.ansible"

RUN mkdir -p ${ACCESS_FOLDERS} && chmod +x -R ${ACCESS_FOLDERS} && chmod 777 -R ${ACCESS_FOLDERS}

WORKDIR leodb

RUN cmake . -DBUILD_TESTS=1

RUN make

CMD ["ansible-playbook", "scripts/runTestSuit.yml", "-e", "path_to_leoDB=.."]
