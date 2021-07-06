FROM alpine
RUN apk add build-base
WORKDIR /app
COPY run-tests.c .
RUN gcc -g -o run-tests run-tests.c

FROM vtikoo/coreclr-tests:pr0-release

WORKDIR /coreclr-tests-all
COPY --from=0 /app/run-tests ./Tests/Core_Root/
COPY pr0-tests-all ./Tests/Core_Root/
COPY pr0-PASSED ./Tests/Core_Root/
COPY pr0-FAILED ./Tests/Core_Root/
