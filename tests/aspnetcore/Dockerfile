FROM mcr.microsoft.com/dotnet-buildtools/prereqs:alpine-3.12-helix-20200908125345-56c6673 as builder

WORKDIR /app
RUN git clone --branch v6.0.0-preview.2.21154.6 --recursive https://github.com/dotnet/aspnetcore
WORKDIR /app/aspnetcore
RUN ./eng/build.sh -nobl -Configuration Release \
    --arch x64 --os-name linux-musl --pack --all \
	--no-build-nodejs --no-build-java \
	-p:AssetManifestFileName=aspnetcore-Linux_musl_x64.xml /p:SkipTestBuild=false; \
	exit 0 # ignore build errors

FROM alpine:latest

COPY --from=builder /app/aspnetcore /aspnetcore
RUN apk add build-base icu libintl