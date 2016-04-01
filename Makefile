HOME_DIR=/Users/b/Documents/
HALIDE_PATH=${HOME_DIR}/src/MIT/halide/halide_src/
HALIDE_INC_PATH=${HALIDE_PATH}/include/
HALIDE_LIB_PATH=${HALIDE_PATH}/lib/

OPENCV_LIBS=-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio
HALIDE_LIB=libHalide.a

all:
	g++ -std=c++11 halide_pipeline.cpp ${HALIDE_LIB_PATH}/${HALIDE_LIB} -I${HALIDE_INC_PATH} -I${HALIDE_INC_PATH}/tools/ -I${HALIDE_INC_PATH}/apps/support/ -ldl -lpthread -lz -L/opt/X11/lib -lpng15 -I/opt/X11/include/libpng15   -o halide_pipeline
	./halide_pipeline
	g++ -std=c++11 demo.cpp halide_pipeline.o ${HALIDE_LIB_PATH}/${HALIDE_LIB} ${OPENCV_LIBS} -I${HALIDE_INC_PATH} -I${HALIDE_INC_PATH}/tools/ -I${HALIDE_INC_PATH}/apps/support/  -msse2 -Wall -O3 -ldl -lpthread -lz -L/opt/X11/lib -lpng15 -I/opt/X11/include/libpng15   -o demo


clean:
	rm -rf demo halide_pipeline.h halide_pipeline.o halide_pipeline *~
