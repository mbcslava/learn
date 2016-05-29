#include "stdafx.h"
#include "CppUnitTest.h"
#include "HelloFunction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			
			Assert::AreEqual(GetHello(), string("Hello"));
		}
		TEST_METHOD(TestMethod2)
		{
			Assert::AreEqual(GetAdressat("World"), string("World"));
			Assert::AreNotEqual(GetAdressat("not world"),string("world"));
			
		}

	};
}