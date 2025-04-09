FROM node:bookworm-slim AS base
WORKDIR /app

FROM base AS build
COPY package.json package-lock.json ./

# FROM build as deps
# RUN npm ci --omit=dev

FROM build AS compile
RUN npm i

COPY . .
RUN npm run build

FROM base

# COPY --from=deps /app/node_modules ./node_modules
COPY --from=compile /app/app ./app

ENTRYPOINT [ "node", "app/main.js" ]
