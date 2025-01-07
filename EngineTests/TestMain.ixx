module;

#include "catch.hpp"

export module TestMain;

export int main(int argc, char* argv[]) 
{
	Catch::Session session;
	
	
	int returnCode = session.applyCommandLine( argc, argv );
	
	if( returnCode != 0 ) { return returnCode; } 
	
	return session.run();
}