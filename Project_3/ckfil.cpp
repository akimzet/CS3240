//	ckfil.cpp	1996 jun 27	last modified 2014 feb 21
//
//	Modified from ckfil.c.
//	Checks input file of structs; see ritfil.cpp.
//

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdlib.h>

using namespace std;

int
main( int argc,char **argv )
{
	void ckfil( ifstream & );

	ifstream fi;

	if ( argc != 2 ) {
		cout << "** use is >ckfil structfile **\n";
		exit( 1 );
	}
	fi.open( *++argv,ios::in | ios::binary );
	if ( fi.fail() ) {
		cout << "** can't open \"" << *argv << "\" **\n";
		exit( 1 );
	}
	ckfil( fi );
	fi.close();
	return( 0 );
}
void
ckfil( ifstream & f )
{
	struct {
		int acct;
		char name[25];
		float dolr;
	} x;

	while ( f.read( (char *)&x,sizeof( x ) ) ) {
		cout << setfill( '0' ) << setw( 9 ) << x.acct << "  ";
		cout.setf( ios::left,ios::adjustfield );
		cout << setfill( ' ' ) << setw( 26 ) << x.name;
		cout.setf( ios::right,ios::adjustfield );
		cout.setf( ios::fixed,ios::floatfield );
		cout.setf( ios::showpoint );
		cout.precision( 2 );
		cout << setw( 10 ) << x.dolr << '\n';
	}
}
