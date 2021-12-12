#include "helper.h"

bool isOutOfBounds(int x, int y) {
    if ((x < 0 || x > ofGetWindowWidth()) || (y < 0 || y > ofGetWindowHeight()))
    {
        return true;
    }
    return false;
}

bool isInBounds(int x, int y) {
	if ((x > 0 && x < ofGetWindowWidth()) && (y > 0 && y < ofGetWindowHeight()))
	{
		return true;
	}
	return false;
}