
#include "CGL/CGL.h"
#include "CGL/viewer.h"

using namespace CGL;

int main (int argc, char* argv[]) {
	Viewer viewer = Viewer();

	viewer.init();


	viewer.start();

	return 0;
}
