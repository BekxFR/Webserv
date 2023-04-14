FROM debian:latest


RUN apt-get -y update && apt-get install -y nginx fcgiwrap vim curl php-fpm php-cgi python3 python3-pip net-tools libcgi-pm-perl
# apache2 libapache2-mod-python python3-pip php-fpm php-cgi fcgiwrap
# RUN pip3 install Flask
RUN pip3 install flup
RUN pip3 install art

# Copie le fichier de configuration nginx personnalisé dans l'image Docker
COPY nginx80.conf /etc/nginx/nginx.conf

# Copie les fichiers HTML et autres ressources statiques dans l'image Docker
COPY ./site/ /usr/share/nginx/html/

RUN mkdir /var/run/php

# COPY nginx-php.sh /etc/init.d/nginx-php.sh

COPY php.ini /etc/php/7.4/fpm/php.ini

# RUN sh /etc/init.d/nginx-php.sh start

# COPY ./site/test.py  /usr/lib/cgi-bin/test.py
# COPY ./site/test.py  /usr/lib/cgi-bin/test.php

# RUN chmod +x /usr/lib/cgi-bin/test.py
RUN chown -R www-data:www-data /usr/share/nginx/html/upload/
RUN chmod -R 777 /usr/share/nginx/html
RUN chmod +x /usr/share/nginx/html/cgi-bin/*
# RUN chmod +x /usr/share/nginx/html/cgi-bin/test.py
# RUN chmod +x /usr/share/nginx/html/cgi-bin/t2.py
# RUN chmod +x /usr/share/nginx/html/t2.py
# RUN chmod +x /usr/share/nginx/html/test.py
# RUN chmod +x /usr/share/nginx/html/test.cgi
# RUN chmod +x /usr/lib/cgi-bin/test.php
# RUN chmod +x /usr/share/nginx/html/cgi-bin/test.php
# RUN chmod +x /usr/share/nginx/html/test.php
# RUN chmod +x /usr/share/nginx/html/upload.cgi
# RUN chmod +x /usr/share/nginx/html/cgi-bin/upload.cgi

# RUN service fcgiwrap start
# RUN python3 -m flup.server.fcgi -s /var/run/fcgiwrap.socket -p /usr/share/nginx/html/
# RUN spawn-fcgi -s /run/fcgiwrap.socket -u www-data -g www-data /usr/sbin/fcgiwrap
# RUN adduser --system --no-create-home --group fcgiwrap
# RUN usermod -a -G www-data fcgiwrap
# RUN chown fcgiwrap:www-data /run/fcgiwrap.socket
# RUN chmod 660 /run/fcgiwrap.socket

CMD ["/bin/bash", "-c", "service fcgiwrap start && php-fpm7.4 && chmod 777 /run/php/php7.4-fpm.sock && nginx -g 'daemon off;'"]
# CMD ["/bin/bash", "-c", "service fcgiwrap start && php-fpm7.4 && chmod 777 /run/php/php7.4-fpm.sock && chmod 755 /usr/share/nginx/html/* && nginx -g 'daemon off;'"]
# CMD ["/bin/bash", "-c", "spawn-fcgi -s /run/fcgiwrap.sock -u www-data -g www-data /usr/sbin/fcgiwrap && php-fpm7.4 && chmod 777 /run/php/php7.4-fpm.sock && chmod 755 /usr/share/nginx/html/* && nginx -g 'daemon off;'"]
# CMD ["/bin/bash", "-c", "php-fpm7.4 && chmod 777 /run/php/php7.4-fpm.sock && chmod 755 /usr/share/nginx/html/* && nginx -g 'daemon off;'"]

# CMD ["nginx", "-g", "daemon off;"]



# TEST
# tail -f /var/log/nginx/access.log
# tail -f /var/log/nginx/error.log
# curl -X POST -o output_file.jpg http://localhost:8099/42.jpg
# curl -X POST -F 'file=@/mnt/nfs/homes/chillion/42.jpg' http://localhost:8099/cgi-bin/42.jpg
# curl -X POST -F 'file=@/mnt/nfs/homes/chillion/42.jpg' http://localhost:8099/HH.jpg
# curl -X PUT -F 'file=@/mnt/nfs/homes/chillion/42.jpg' http://localhost:8099/HH.jpg
# curl -X POST -F 'file=@/mnt/nfs/homes/chillion/42.jpg' http://localhost:8099/cgi-bin/upload.cgi
