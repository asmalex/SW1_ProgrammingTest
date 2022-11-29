# Programming Test
[![Build and Test](https://github.com/asmalex/SW1_ProgrammingTest/actions/workflows/msbuild.yml/badge.svg)](https://github.com/asmalex/SW1_ProgrammingTest/actions/workflows/msbuild.yml)
<!--[![Release](/github/v/release/asmalex/SW1_ProgrammingTest)]-->

There are three sub-problems: Duplicate List, Sequence Buffer, and Reverse Words. As you can see from the status badge above, this repo will automatically build and execute unit tests against anything pushed or pull-requested into the main branch. A description of the sub-problems and their solution (as well as assumptions) is documented below:

## Duplicate List

Works by placing copy inline and preserving the reference pointer. By definition.
For example:

a=>a'=>b=>b'

Then you can use the 

## Installation
•	Clone contents into your project dev folder.
•	Build project (Cntr+Shift+B)
•	That's it!
•	To run a unit test locally on VS 2019 can be done from the menu select Window - > General -> Test Explorer
•	You should see the unit tests in the test explorer. You can run a unit test by double-clicking it

Note: this was tested on Visual Studio 2019 Version 16.11.5 on Windows 10 v10.0.19044. It'll likely work on other versions as well. 


Unit tests for checking Unity can control the movement of a 2DOF 360-capable pitch & roll flight simulator in a way that makes sense

## Test Table
Input String| Expected Result
------------ | -------------
"" |	""
"                   " | "                   "
"Supercalifragilisticexpialidocious" | "Supercalifragilisticexpialidocious"
"Hello world" |	"world Hello"
"   Hello  World " | " World  Hello   "
"The quick brown fox jumps over the lazy dog" | "dog lazy the over jumps fox brown quick The"
"Sierra Nevada Corporation. Or SNC." | "SNC. Or Corporation. Nevada Sierra"
