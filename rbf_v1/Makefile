.PHONY: distxt disteig

all: bench bench2
# distxt disteig
	@echo "done!"

hello: hello.cpp
	g++-9 -I/Users/junhangc/anaconda3/include  hello.cpp -o hello  -lblas -llapack -DHAVE_CBLAS=1 -g

bench: bench.cpp
	g++-9 -I/Users/junhangc/anaconda3/include -I/usr/local/include/ bench.cpp -o bench \
 	 -lblas -llapack -DHAVE_CBLAS=1


bench2: bench2.cpp
	g++-9 -I/Users/junhangc/anaconda3/include -I/usr/local/include/ bench2.cpp -o bench2 \
 	 -lblas -llapack -DHAVE_CBLAS=1 -DXTENSOR_USE_XSIMD=1

# distxt: xtdist.cpp
# 	g++-9 -I/Users/junhangc/anaconda3/include  xtdist.cpp -o distxt  -lblas -llapack -DHAVE_CBLAS=1 -g

# disteig: etdist.cpp
# 	g++-9 -I/Users/junhangc/anaconda3/include -I/usr/local/include/ etdist.cpp -o disteig \
# 	 -lblas -llapack -DHAVE_CBLAS=1 -g