xtools := ${HOME}/xtools/aarch64-unknown-linux-gnu/
CXX := ${xtools}/bin/aarch64-unknown-linux-gnu-g++
CFLAGS := -I${xtools}/aarch64-unknown-linux-gnu/sysroot/usr/local/include/eigen3/ -O3 

CDIST_IMPL := cdist_eigen.o cdist_forloop.o cdist_gemm_b.o cdist_gemm_bu.o cdist_gemm_bup.o cdist_gemm_bupn.o

CDIST_OBJ := $(addprefix data/,$(CDIST_IMPL))
$(info    cdist obj is $(CDIST_OBJ))


data/cdist_eigen.o: perf_step/perf_cdist/cdist_eigen.cc
	$(CXX) $(CFLAGS) -c $< -o $@

data/cdist_forloop.o: perf_step/perf_cdist/cdist_forloop.cc
	$(CXX) $(CFLAGS) -c $< -o $@

data/cdist_gemm_b.o: perf_step/perf_cdist/cdist_gemm_b.cc
	$(CXX) $(CFLAGS) -c $< -o $@

data/cdist_gemm_bu.o: perf_step/perf_cdist/cdist_gemm_bu.cc
	$(CXX) $(CFLAGS) -c $< -o $@

data/cdist_gemm_bup.S: perf_step/perf_cdist/cdist_gemm_bup.cc
	$(CXX) $(CFLAGS) -S  -fverbose-asm $< -o $@

data/cdist_gemm_bup.o: perf_step/perf_cdist/cdist_gemm_bup.cc
	$(CXX) $(CFLAGS) -c $< -o $@

data/cdist_gemm_bupn.o: perf_step/perf_cdist/cdist_gemm_bupn.cc
	$(CXX) $(CFLAGS) -c $< -o $@

data/util.o: perf_step/perf_cdist/util.cc
	$(CXX) $(CFLAGS) -c $< -o $@

data/valid_cdist.o: perf_step/perf_cdist/main_cdist_valid.cc
	$(CXX)  $(CFLAGS)  $^    -c -o $@

data/bench_cdist.o: perf_step/perf_cdist/main_cdist_bench.cc
	$(CXX)  $(CFLAGS)  $^    -c -o $@


data/valid_cdist: $(CDIST_OBJ) data/util.o data/valid_cdist.o
	$(CXX)  $(CFLAGS)  data/valid_cdist.o data/util.o $(CDIST_OBJ)   -o $@

data/bench_cdist: $(CDIST_OBJ) data/util.o data/bench_cdist.o
	$(CXX)  $(CFLAGS)  data/bench_cdist.o data/util.o $(CDIST_OBJ)   -o $@


data/demo: perf_step/perf_cdist/kernel_4x4_l2.cc
	$(CXX)  $(CFLAGS)  $^     -o $@

.PHONY:all
all: valid_cdist bench_cdist demo
	@echo "all done"

valid_cdist: data/valid_cdist data/cdist_gemm_bup.S
	@echo "valid done"

bench_cdist: data/bench_cdist
	@echo "bench done"

demo: data/demo
	@echo "deme done"

# # # ge_cdist: perf_step/perf_cdist/ge_cdist.cc
# # # 	$(CXX)  $(CFLAGS)  $^   -o data/$@
