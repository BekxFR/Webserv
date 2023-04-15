FROM debian:latest

RUN apt-get -y update && apt-get install -y nginx fcgiwrap vim curl php-fpm php-cgi python3 python3-pip net-tools libcgi-pm-perl
RUN pip3 install flup
RUN pip3 install art
RUN mkdir /var/run/php

COPY nginx80.conf /etc/nginx/nginx.conf
COPY ./site/ /usr/share/nginx/html/
COPY php.ini /etc/php/7.4/fpm/php.ini

RUN chown -R www-data:www-data /usr/share/nginx/html/upload/
RUN chmod -R 777 /usr/share/nginx/html
RUN chmod +x /usr/share/nginx/html/cgi-bin/*


CMD ["/bin/bash", "-c", "service fcgiwrap start && php-fpm7.4 && chmod 777 /run/php/php7.4-fpm.sock && nginx -g 'daemon off;'"]

# TEST
# tail -f /var/log/nginx/access.log
# tail -f /var/log/nginx/error.log
# curl -X POST -o output_file.jpg http://localhost:8099/42.jpg
# curl -X POST -F 'file=@/mnt/nfs/homes/chillion/42.jpg' http://localhost:8099/cgi-bin/42.jpg
# curl -X POST -F 'file=@/mnt/nfs/homes/chillion/42.jpg' http://localhost:8099/HH.jpg
# curl -X PUT -F 'file=@/mnt/nfs/homes/chillion/42.jpg' http://localhost:8099/HH.jpg
# curl -X POST -F 'file=@/mnt/nfs/homes/chillion/42.jpg' http://localhost:8099/cgi-bin/upload.cgi
