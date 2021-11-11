
all: logvary ori logd step
	@echo "done!"

logvary: log_vary.cpp
	g++ log_vary.cpp -O3 -march=native -o logvary

ori: impl_log.cpp
	g++ impl_log.cpp -O3 -march=native -o ori


logd: log_double.cpp
	g++ log_double.cpp -g -march=native -o logd

step: log_step.cc
	g++ log_step.cc -g -march=native -o step