# Create deb file
all:
	docker run --rm -id --name=qjournalctl-deb --volume=$(realpath ../..)/:/source:ro --volume=$(realpath .)/:/out pentix/qjournalctl-deb-build-env
	touch qjournalctl_0.6-1.deb
	docker exec qjournalctl-deb bash /out/build_deb.sh
	docker stop qjournalctl-deb
	
docker-build-env: Dockerfile
	docker build -t pentix/qjournalctl-deb-build-env .

push-docker-build-env: docker-build-env
	docker push pentix/qjournalctl-deb-build-env 
