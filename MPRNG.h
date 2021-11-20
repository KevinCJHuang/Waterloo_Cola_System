// Concurrent-Safe Pseudo Random-Number Generator : generate repeatable sequence of values that appear random.
//
// Interface :
//   MPRNG(s) - set starting seed 
//   seed() - read seed
//   seed(s) - reset seed
//   mprng() - generate random value in range [0,UINT_MAX]
//   mprng(u) - generate random value in range [0,u]
//   mprng(l,u) - generate random value in range [l,u]
//
// Examples : generate random number between 5-21
//   mprng() % 17 + 5	values 0-16 + 5 = 5-21
//   mprng( 16 ) + 5
//   mprng( 5, 21 )

#pragma once

_Monitor MPRNG {
	unsigned int PRNGcnt = 0;
	unsigned int seed;
	unsigned int seed_r;								// random state
  public:
	// Cannot overload _Mutex/_Nomutex routines.
	void set_seed( unsigned int seed_ ) { seed_r = seed = seed_; srand( seed ); } // set seed
	__attribute__(( warn_unused_result )) _Nomutex unsigned int get_seed() const { return seed; } // get seed
	MPRNG( unsigned int seed = 362436069 ) { set_seed( seed ); } // set seed
	__attribute__(( warn_unused_result )) unsigned int operator()() { PRNGcnt += 1; return rand_r( &seed_r ); } // [0,UINT_MAX]
	__attribute__(( warn_unused_result )) unsigned int operator()( unsigned int u ) { return operator()() % (u + 1); } // [0,u]
	__attribute__(( warn_unused_result )) unsigned int operator()( unsigned int l, unsigned int u ) { return operator()( u - l ) + l; } // [l,u]
	__attribute__(( warn_unused_result )) unsigned int calls() { return PRNGcnt; }
}; // MPRNG
