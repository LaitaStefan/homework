# Papa Florin, 334CA

CFLAGS += -Wall

a.out: *.cu *.cpp
	nvcc -I$(CUDA_PATH)/samples/common/inc/ $^

plot:
	gnuplot gnu.plot


clean:
	rm a.out

.PHONY: clean
