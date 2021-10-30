FROM ubuntu:latest

LABEL maintainer="nefisik@mail.ru"
#Timezone
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

COPY . /usr/src/auth-web-server
WORKDIR /usr/src/auth-web-server

RUN chmod +x install.sh
RUN bash install.sh

RUN chmod +x start.sh
ENTRYPOINT ["./start.sh"]