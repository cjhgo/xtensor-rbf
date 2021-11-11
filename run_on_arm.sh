abspath="$(realpath $0)"
repopath=${abspath%/*}
echo $repopath
:<<"eof"
docker run   -it -d \
		-v $repopath:/home/linaro/workspace/rbf_repo\
		-w  /home/linaro/workspace/rbf_repo \
		-v ~/workspace/a1a2dev/xtools:/home/linaro/xtools \
    --name armenv a12base:latest
eof

#:<<eof

docker exec -it armenv bash -c "make"

rsync  -avz \
	--exclude '*.make' \
	--delete  \
	--exclude '.git' \
	--exclude 'build' \
	$repopath \
	mqarm:/home/linaro/cjh/workspace

#eof

sleep 1

ssh mqarm "/home/linaro/cjh/workspace/xtensor-rbf/data/cdist_demo"