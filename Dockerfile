FROM node:bookworm-slim

WORKDIR /app

COPY package.json package-lock.json ./
RUN npm i 

COPY . .

ENTRYPOINT [ "npm", "run" ]
CMD [ "start" ]