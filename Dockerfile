FROM node:bookworm-slim AS base
WORKDIR /app

FROM base AS build

COPY package.json package-lock.json ./
RUN npm i 

COPY . .
RUN npm run build

FROM base

COPY --from=build /app/package.json .    
COPY --from=build /app/node_modules ./node_modules
COPY --from=build /app/app ./app

ENTRYPOINT [ "npm", "run" ]
CMD [ "start" ]