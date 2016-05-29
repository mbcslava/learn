#Version: 1.0.0
#�� ������ ������ ������
FROM ubuntu:14.04
#���������� ������ ������
MAINTAINER User <mbcslava@outlook.com>
#����� apt-get �� ��������� ��� ������ ��������� ���������� � ������ ��� ����� � �� �������
ENV DEBIAN_FRONTEND noninteractive
#�������� ������� (��������� ����������� APT - ������������� �����)
RUN apt-get update
RUN apt-get install -y nginx
#������� ���� nginx.conf � �������� ������� ���������� ����������
ADD nginx.conf /etc/nginx/
#�������� ����� ���� ����� �������
EXPOSE 80
#��������, ��� /www �������� ����� � ����� ��������� ����� �����, ����� � ������ ��� ������ ������������
VOLUME /www
#��������, ��� ��� ������� ������ �� ��������� ��������� Nginx
CMD /usr/share/nginx