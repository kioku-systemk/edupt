#include <math.h>
#include "render.h"

int main(int argc, char **argv) {
	printf("Path tracing renderer: edupt\n\n");
    if (!edupt::sceneInit()){
        printf("Failed sceneInit\n");
        return 1;
    }
	// 640x480の画像、(2x2) * 4 sample / pixel
	edupt::render(640, 480, 16, 2);
}
