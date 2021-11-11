xtools="${HOME}/xtools/aarch64-unknown-linux-gnu/"
CXX="${xtools}/bin/aarch64-unknown-linux-gnu-g++"
FLAGS="-I${xtools}/aarch64-unknown-linux-gnu/sysroot/usr/local/include/eigen3/"


cdist_demo: perf_step/perf_cdist/cdist.cc
	$(CXX)  $(FLAGS)  $^ -o data/$@