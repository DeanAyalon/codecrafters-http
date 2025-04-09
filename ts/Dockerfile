# use the official Bun image
# see all versions at https://hub.docker.com/r/oven/bun/tags
FROM oven/bun:1 AS base
WORKDIR /app

# install with --production (exclude devDependencies)
FROM base AS install
WORKDIR /temp
COPY package.json bun.lockb ./
RUN bun install --production --frozen-lockfile

FROM base AS release
COPY --from=install /temp/node_modules node_modules
COPY . .

USER bun
EXPOSE 4221
ENTRYPOINT [ "bun", "run", "dev" ]
