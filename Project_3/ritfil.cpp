//	ritfil.cpp	1996 jun 26	last modified 2014 feb 21
//
//	Modified from ritfil.c.
//	Creates masfil from masfilt or tranfil from tranfilt; for 3240as0?.
//	Input is file of text; output is file of structs.
//

#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

int
main( int argc,char **argv )
{
	void makefil( ifstream &,ofstream & );

	ifstream fi;				// input file
	ofstream fo;				// output file
	char ch;

	if ( argc != 3 ) {
		cout << "** use is >ritfil textfile structfile **\n";
		exit( 1 );
	}
	fi.open( *++argv,ios::in );
	if ( fi.fail() ) {
		cout << "** can't open " << *argv << "**\n";
		exit( 1 );
	}
	fo.open( *++argv,ios::in | ios::binary );
	if ( !fo.fail() ) {
		cout << "** \"" << *argv << "\" exists **  overwrite? ";
		cin.get( ch );
		if ( ch != '\n' ) {
			while ( cin.get() != '\n' ) {
				;
			}
		}
		if ( (ch != 'y') && (ch != 'Y') ) {
			cout << "ok: -- \"" << *argv << "\" is unchanged --\n";
			exit( 0 );
		} else {
			fo.close();
		}
	}
	fo.clear();
	fo.open( *argv,ios::out );
	if ( fo.fail() ) {
		cout << "** can't open \"" << *argv << "\" **\n";
		exit( 1 );
	}
	makefil( fi,fo );
	fi.close();
	fo.close();
	return( 0 );
}







void
makefil( ifstream & fi,ofstream & fo )	
{
	struct {
		int acct;
		char name[25];
		float dolr;
	} x;
	int i;
	char line[81],*p,*q;

	while ( fi.getline( line,81 ) ) {
		x.acct = atoi( line );
		p = line;
		while ( *p++ != ' ' ) {
			;
		}
		for ( q = x.name, i = 24;  i--; ) {
			*q++ = *p++;
		}
//		*q = '\0';
		while ( *--q == ' ' ) {		// find end of name string
			;
		}
		*++q = '\0';			// mark end of name string
		x.dolr = atof( p );
		fo.write( (char *)&x,sizeof( x ) );
	}
}
