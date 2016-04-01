HALIDE_ROOT=$(error Set your Halide path)
OPENCV_LIBS=-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio

HOST=$(shell hostname -s)
MPICXX=
CXXFLAGS := -std=c++11 $(CXXFLAGS)

ifneq (,$(findstring lanka,$(HOST)))
	MPICXX=mpicxx
endif
ifneq (,$(findstring salike,$(HOST)))
	MPICXX=mpicxx
endif
ifneq (,$(findstring cori,$(HOST)))
	MPICXX=CC
endif
ifneq (,$(findstring edison,$(HOST)))
	MPICXX=CC
endif

halide_pipeline_aot: halide_pipeline_aot.cpp
	$(CXX) $(CXXFLAGS) halide_pipeline_aot.cpp $(HALIDE_ROOT)/lib/libHalide.a -I$(HALIDE_ROOT)/include -I$(HALIDE_ROOT)/include/tools/ -I$(HALIDE_ROOT)/include/apps/support/ -ldl -lpthread -lz -L/opt/X11/lib -lpng15 -I/opt/X11/include/libpng15   -o halide_pipeline_aot
	./halide_pipeline_aot

livedemo: halide_pipeline_aot livedemo.cpp
	g++ -std=c++11 livedemo.cpp halide_pipeline_aot.o $(HALIDE_ROOT)/lib/libHalide.a $(OPENCV_LIBS) -I$(HALIDE_ROOT)/include -I$(HALIDE_ROOT)/include/tools/ -I$(HALIDE_ROOT)/include/apps/support/  -msse2 -Wall -O3 -ldl -lpthread -lz -L/opt/X11/lib -lpng15 -I/opt/X11/include/libpng15   -o livedemo

distributed: halide_pipeline_jit.cpp
	$(MPICXX) $(CXXFLAGS) $< -o $@ -O3 -ffast-math -Wall -I $(HALIDE_ROOT)/include $(HALIDE_ROOT)/bin/libHalide.a -lpthread -ldl -lz $(LDFLAGS) -DDEMO_DISTRIBUTED

cpu: halide_pipeline_jit.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ -O3 -ffast-math -Wall -I $(HALIDE_ROOT)/include $(HALIDE_ROOT)/bin/libHalide.a -lpthread -ldl -lz $(LDFLAGS) -DDEMO_CPU

gpu: halide_pipeline_jit.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ -O3 -ffast-math -Wall -I $(HALIDE_ROOT)/include $(HALIDE_ROOT)/bin/libHalide.a -lpthread -ldl -lz $(LDFLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS) $(OPENGL_LDFLAGS) -DDEMO_GPU


clean:
	rm -rf livedemo halide_pipeline_aot.h halide_pipeline_aot.o halide_pipeline_aot distributed cpu gpu *~
