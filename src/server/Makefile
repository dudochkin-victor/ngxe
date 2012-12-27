PACKAGE = node-gettext.js
NODEJS = $(if $(shell test -f /usr/bin/nodejs && echo "true"),nodejs,node)

test:
	$(NODEJS) ./deps/nodeunit/bin/nodeunit test

upstream:
	git push origin master:refs/heads/master

locales:
	/bin/sh -c 'cd locale; \
	for locale in `ls -1F|grep /`; do \
	(cd "$$locale"; \
	touch messages.po; \
	xgettext -L Java --force-po --no-location --from-code=ru_RU.UTF-8 -j -a ../../*js -o messages.po; \
	sed -i 's/charset=CHARSET/charset=utf-8/g' messages.po;) \
	done'

dump:
	mysqldump -u root --triggers --routines fallen > database.sql

.PHONY: test
