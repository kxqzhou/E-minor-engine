
#include "engine.h"

#include <cstdio>
#include <string>

int main( int argc, char* argv[] ) {
	int screen_width = 1100;
    int screen_height = 620;
    if (argc == 3) {
        screen_width = std::stoi( argv[1] );
        screen_height = std::stoi( argv[2] );
        printf( "Using window size %d by %d\n", screen_width, screen_height );
    }

    Engine eminor( screen_width, screen_height );
    eminor.go();

    return 0;
}

