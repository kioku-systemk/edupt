#include <math.h>
#include <stdlib.h>
#include "render.h"

int main(int argc, char **argv) {
	printf("Path tracing renderer: edupt based mrz renderer\n\n");
    if (argc < 2){
        printf("edupt [mrz filepath].\n");
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
	// 640x480の画像、(2x2) * 4 sample / pixel
	edupt::render(640, 480, spp/4, 2);
    return 0;
}
