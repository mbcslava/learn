#include "stdafx.h"
#include <iostream>
#include "HelloFunction.h"

using namespace std;

int main()
{
	cout << GetHello() + " " + GetAdressat("world") + "!" << endl;
	return 0;
}