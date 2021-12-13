#include "HelperFunctions.h"

//--------------------------------------------------------------
bool IsInBounds(int x, int y) {
	if ((x > 0 && x < ofGetWindowWidth()) && (y > 0 && y < ofGetWindowHeight()))
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool IsOutOfBounds(int x, int y) {
	if ((x < 0 || x > ofGetWindowWidth()) || (y < 0 || y > ofGetWindowHeight()))
	{
		return true;
	}
	return false;
}