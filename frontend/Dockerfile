FROM node:10-slim

WORKDIR /usr/src/app

# ARG NODE_ENV
# ENV NODE_ENV $NODE_ENV

COPY package.json /usr/src/app/
RUN npm install

VOLUME ["/usr/src/app"]

EXPOSE 8080
CMD [ "npm", "run", "dev"]
