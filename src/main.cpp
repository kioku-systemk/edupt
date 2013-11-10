#include <math.h>
#include <stdlib.h>
#include "render.h"

int main(int argc, char **argv) {
	printf("Path tracing renderer: edupt based mrz renderer\n\n");
    if (argc < 2){
        printf("edupt [mrz filepath] [spp=64] [width=640] [height=480]\n");
        return 0;
    }
    
    int spp = 64;
    if (argc >= 3){
        spp = atoi(argv[2]);
    }
    if (!edupt::sceneInit(argv[1])){
        printf("Failed model load\n");
        return 1;
    }
    int width = 640;
    int height = 480;
    if (argc >= 5){
        width = atoi(argv[3]);
        height = atoi(argv[4]);
    }
    
	// 640x480の画像、(2x2) * 4 sample / pixel
	edupt::render(width, height, spp/4, 2);
    return 0;
}

