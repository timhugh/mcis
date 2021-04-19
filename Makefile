PG_HOST?=localhost
PG_USER?=postgres
PG_DATABASE?=mcis

.PHONY: all
all: tileserver

.PHONY: tileserver
tileserver:
	$(MAKE) -j -C tileserver

.PHONY: backup
backup:
	pg_dump -C -h ${PG_HOST} -U ${PG_USER} ${PG_DATABASE} -f backup.sql
