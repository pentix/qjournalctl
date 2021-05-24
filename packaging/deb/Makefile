all: docker-build-env
	sh -c 'echo "$DOCKER_PASSWORD" | docker login -u "$DOCKER_USERNAME" --password-stdin' || echo 'Not using docker login'
	docker run --privileged --rm -v $(shell realpath ../..)/:/source:ro -v $(shell realpath .)/:/out -e USERID=$(shell id -u) -e GROUPID=$(shell id -g) pentix/qjournalctl-deb-build-env bash /out/build_deb.sh
	
docker-build-env: Dockerfile
	docker build -t pentix/qjournalctl-deb-build-env .

push-docker-build-env: docker-build-env
	docker push pentix/qjournalctl-deb-build-env 
