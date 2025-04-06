FROM deanayalon/vcpkg AS build
WORKDIR /app

ENV VCPKG_ROOT=/vcpkg
COPY . .
RUN ./.codecrafters/compile.sh

# Final
FROM alpine
WORKDIR /app
COPY --from=build /app/build .
ENTRYPOINT ["./server"]
