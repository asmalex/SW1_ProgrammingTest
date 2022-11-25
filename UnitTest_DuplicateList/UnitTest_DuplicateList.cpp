#include "pch.h"
#include "CppUnitTest.h"
#include "../DuplicateList/DuplicateList.c"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestDuplicateList
{
	TEST_CLASS(UnitTestDuplicateList)
	{
	public:

		//null poiiner
		TEST_METHOD(DuplicateList_NullPointer_NullPointer)
		{
			Node_t* result;
			Node_t* source = NULL;
			
			result = DuplicateList(source);

			Assert::AreEqual(source, result);
		}
	};
}
