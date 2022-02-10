default:

clean:
	rm -f check_argc0_lsm.service

install:
	sed -e 's!DESTDIR!${DESTDIR}!' check_argc0_lsm.service.tmpl > check_argc0_lsm.service
	install --mode=0644 check_argc0_lsm.service /etc/systemd/system/
