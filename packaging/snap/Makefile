all:
	docker run --rm -id --workdir=/snap --name=qjournalctl-snap --volume=$(realpath .):/out archlinux/base
	touch qjournalctl_0.6_amd64.snap
                
	docker exec -i qjournalctl-snap pacman -Syy
	docker exec -i qjournalctl-snap pacman -S git
	docker exec -i qjournalctl-snap git clone https://aur.archlinux.org/yay.gitcd yay
	docker exec -i qjournalctl-snap bash -c "cd yay ; makepkg -si"

	docker cp snapcraft.yaml qjournalctl-snap:/snap/
	docker exec -i qjournalctl-snap snapcraft
	docker exec -i qjournalctl-snap bash -c 'cp /snap/*.snap /out'

	docker stop qjournalctl-snap
