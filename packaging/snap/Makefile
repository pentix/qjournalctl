all:
	docker run --rm -id --workdir=/snap --name=qjournalctl-snap --volume=$(realpath .):/out ubuntu
	touch qjournalctl_0.6_amd64.snap
                
	docker exec -i qjournalctl-snap apt-get update
	docker exec -i qjournalctl-snap apt-get -y install snapd snapcraft

	docker cp snapcraft.yaml qjournalctl-snap:/snap/
	docker exec -i qjournalctl-snap snapcraft
	docker exec -i qjournalctl-snap bash -c 'cp /snap/*.snap /out'

	docker stop qjournalctl-snap

	RES=$(shell ls -lhsa *.snap) ; echo "Build succeeded: "$(RES)
