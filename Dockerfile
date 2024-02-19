FROM gcc:bullseye as development

FROM development as production
COPY . /usr/project
WORKDIR /usr/project
RUN make
ENTRYPOINT ["tail", "-f", "/dev/null"]